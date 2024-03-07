/*****************************************************
 * Copyright Grégory Mounié 2008-2015                *
 *           Simon Nieuviarts 2002-2009              *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/
 
#include <glob.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wordexp.h>
#include <sys/wait.h>
#include <sys/resource.h>

#include "variante.h"
#include "readcmd.h"

#ifndef VARIANTE
#error "Variante non défini !!"
#endif

/* Guile (1.8 and 2.0) is auto-detected by cmake */
/* 
 * To disable Scheme interpreter (Guile support), comment the
 * following lines.  You may also have to comment related pkg-config
 * lines in CMakeLists.txt.
*/

struct Job {
	pid_t pid;
	char **cmd;
	struct timeval start_time;
	struct Job *next;
};
struct Job *jobs = NULL;
int limit_initialized = 0;
struct rlimit limit;

int calculer_cmd_len(char** cmd);
int calculer_seq_len(struct cmdline *l);
int open_output_fd(struct cmdline *l);
int open_input_fd(struct cmdline *l);

char* process_wordexp(const char *line);
char* process_glob(const char *line);
char* etendre_line(char* line);
char* etendre_mot(char* mot);

void display_cmdline(struct cmdline *l);
void ajouter_job(pid_t pid, char** cmd);
void supprimer_job(struct Job *job);
void executer_line(char *line);

void setup_signal_handler();
void sigchld_handler();
void sigxcpu_handler();
void supprimer_jobs();
void afficher_jobs();

void error_message(char* msg);
void terminate(char *line);

#if USE_GUILE == 1
#include <libguile.h>

int question6_executer(char *line){
	/* Question 6: Insert your code to execute the command line
	 * identically to the standard execution scheme:
	 * parsecmd, then fork+execvp, for a single command.
	 * pipe and i/o redirection are not required.
	 */
		
	executer_line(line);
	return 0;
}

SCM executer_wrapper(SCM x){
    return scm_from_int(question6_executer(scm_to_locale_stringn(x, 0)));
}
#endif

/* Fixer la limite de temps du processus enfant */
/*		//if(limit_initialized){
			printf("Se limita el tiempo\n");
			if (setrlimit(RLIMIT_CPU, &limit) != 0) error_message("error en setrlimit");
			limit_initialized = 0;
		//}
*/

int main(){
    setup_signal_handler();
	printf("Variante %d: %s\n", VARIANTE, VARIANTE_STRING);

	#if USE_GUILE == 1
    scm_init_guile();
    /* register "executer" function in scheme */
    scm_c_define_gsubr("executer", 1, 0, 0, executer_wrapper);
	#endif

	while(1){
		char *line = 0;
		char *prompt = "ensishell>";

		/* Readline use some internal memory structure that
		   can not be cleaned at the end of the program. Thus
		   one memory leak per command seems unavoidable yet */
		line = readline(prompt);
		if(line == 0 || !strncmp(line,"exit", 4)){
			terminate(line);
		}

		#if USE_GNU_READLINE == 1
		add_history(line);
		#endif

		#if USE_GUILE == 1
		/* The line is a scheme command */
		if(line[0] == '('){
			char catchligne[strlen(line) + 256];
			sprintf(catchligne, "(catch #t (lambda () %s) (lambda (key . parameters) (display \"mauvaise expression/bug en scheme\n\")))", line);
			scm_eval_string(scm_from_locale_string(catchligne));
			free(line);
            continue;
        }
		#endif

		if(!strncmp(line,"jobs", 4)){
			afficher_jobs();
		}else if(!strncmp(line,"ulimit", 6)){
			limit.rlim_cur = atoi(line+7);
			limit.rlim_max = atoi(line+7) + 5;
			limit_initialized = 1;
		}else{
			executer_line(line);		
		}

	}
	return 0;
}

void setup_signal_handler(){
	// Configurar el manejador de la señal SIGCHLD
    struct sigaction sa_chld;
    sa_chld.sa_handler = sigchld_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa_chld, NULL);

    // Configurar el manejador de la señal SIGXCPU
    struct sigaction sa_xcpu;
    sa_xcpu.sa_handler = sigxcpu_handler;
    sigemptyset(&sa_xcpu.sa_mask);
    sa_xcpu.sa_flags = SA_RESTART;
    sigaction(SIGXCPU, &sa_xcpu, NULL);
}

/*
	Fonction qui gère les signaux SIGXCPU
*/
void sigxcpu_handler(){
	printf("Recibió SIGXCPU por %d\n", getpid());
}

/*
	Fonction qui gère les signaux SIGCHLD
*/
void sigchld_handler(){
    int status;
    pid_t child_pid;

	/* Évaluer les processus qui ont terminé */
    while ((child_pid = waitpid(-1, &status, WNOHANG)) > 0) {  
        
		/* Mesurer le temps de fin */
		struct timeval end_time;
        gettimeofday(&end_time, NULL);

		struct Job *current = jobs;
		while(current != NULL){
			if(current->pid == child_pid){
				/* Calculer le temps en millisecondes */
    			long temps_en_milisecondes = (end_time.tv_sec - current->start_time.tv_sec) * 1000 +
                                   (end_time.tv_usec - current->start_time.tv_usec) / 1000;
    			printf("Le temps de durée: %ld milisecondes\n", temps_en_milisecondes);
				supprimer_job(current);
				break;
			}
			current = current->next;
		}
    }
}

/*
	Fonction pour exécuter une ligne
*/
void executer_line(char *line){

	/* Étendre la commande et appeler parsecmd */
	char* extended_cmd = etendre_line(line);
	//printf("extended_cmd: '%s'\n", extended_cmd);
	struct cmdline *l = parsecmd( & extended_cmd);
	free(extended_cmd);

	if(!l) terminate(0);
	//display_cmdline(l);

	int seq_len = calculer_seq_len(l);
	int pipes[seq_len - 1][2];
	pid_t child_pids[seq_len];

	int input_fd = open_input_fd(l);
	int output_fd = open_output_fd(l);

	/* Parcourir la liste des commandes et les exécuter */
	for(int i = 0; i < seq_len; i++){
		char **cmd = l->seq[i];
	
		/* Initialiser les tuyaux */
		if(i < seq_len - 1){
        	if(pipe(pipes[i]) == -1) error_message("error en pipe");
    	}

		int pid = fork();
		if(pid == -1) error_message("error en fork");
		if(pid == 0){	

			/* Rediriger l'entrée standard vers le tuyau i-1 */
			if(i > 0) dup2(pipes[i - 1][0], 0);
					
			/* Rediriger la sortie standard vers le tuyau i */
			if(i < seq_len - 1) dup2(pipes[i][1], 1);

			/* Redirection vers les fichiers */
			if(input_fd != -1 && i == 0) dup2(input_fd, 0);
			if(output_fd != -1 && i == seq_len - 1) dup2(output_fd, 1);
					
			/* Fermer tous les descripteurs des tuyaux */
			for (int j = 0; j < seq_len - 1; j++) {
        	    close(pipes[j][0]);
        	    close(pipes[j][1]);
        	}

			/* Fermer, s'ils ont été ouverts, les descripteurs de fichier */
        	if (output_fd != -1) close(output_fd);
			if (input_fd != -1) close(input_fd);

			if(limit_initialized){
				if (setrlimit(RLIMIT_CPU, &limit) != 0) error_message("error en setrlimit");
			}

			execvp(cmd[0], cmd);
			error_message("error en execvp");
		}

		/* Stocker le PID du processus fils */
		child_pids[i] = pid;

		if(l->bg){
			/* Ajouter le processus à la liste des jobs */
			ajouter_job(pid, cmd);
		}
	}

	/* Fermer tous les descripteurs des tuyaux */
	for (int j = 0; j < seq_len - 1; j++) {
        close(pipes[j][0]);
        close(pipes[j][1]);
    }

	/* Fermer, s'ils ont été ouverts, les descripteurs de fichier */
    if (output_fd != -1) close(output_fd);
	if (input_fd != -1) close(input_fd);

	/* Attendre les processus enfants s'il n'y a pas d'arrière-plan */
	if(!l->bg){
		int status;
    	for(int i = 0; i < seq_len; i++){
    		waitpid(child_pids[i], &status, 0);
    	}
	}
}

/*
	Fonction qui étend une ligne de commande
*/
char* etendre_line(char* line){
	char *expanded_cmd = 0;
	char *init = line;
	char *next_mot;

	do{
		next_mot = strchr(init, ' ');
		if (next_mot == NULL) {
			next_mot = init + strlen(init);
		}
		
		size_t length_to_extract = next_mot - init;
		char *substring = strndup(init, length_to_extract);
		if(substring == NULL) error_message("error en strndup");
		char *result = etendre_mot(substring);
		free(substring);

		if(expanded_cmd){
			expanded_cmd = realloc(expanded_cmd, (strlen(expanded_cmd) + strlen(result) + 2) * sizeof(char));
			if (expanded_cmd == NULL) error_message("error en realloc");
			expanded_cmd[strlen(expanded_cmd) + strlen(result)+1] = '\0';
			strcat(expanded_cmd, " ");
			strcat(expanded_cmd, result); 
		}else{
			expanded_cmd = strdup(result);
		}
		
		free(result);
		init = next_mot + 1;

	}while(*next_mot != '\0');

	return expanded_cmd;
}

/*
	Fonction qui étend un mot
*/
char* etendre_mot(char* mot){
    char *debut_variable = strchr(mot, '$');
    if(debut_variable == NULL){
		return process_glob(mot);
	}

	char *fin_variable;
    if(*(debut_variable+1) == '('){
		fin_variable = strchr(debut_variable, ')');
        if(fin_variable == NULL) error_message("Error en la palabra");
		fin_variable = fin_variable + 1;
	}else{
		fin_variable = strchr(debut_variable, '/');
        if(fin_variable == NULL){
			fin_variable = debut_variable + strlen(debut_variable);
		}
	}

	/* Étendre la variable avec wordexp */
	size_t variable_len = fin_variable - debut_variable;   
    char *substring = strndup(debut_variable, variable_len); 
	if(substring == NULL) error_message("error en strndup");
    char *expanded_substring = process_wordexp(substring);
	free(substring);

    /* Incorporer la partie étendue dans le mot d'origine */
    char* expanded_cmd = malloc((strlen(mot) - variable_len + strlen(expanded_substring) + 1) * sizeof(char));
    if(expanded_cmd == NULL) error_message("error en malloc");
    strncpy(expanded_cmd, mot, debut_variable - mot);
    expanded_cmd[debut_variable - mot] = '\0';
    strcat(expanded_cmd, expanded_substring);
    strcat(expanded_cmd, fin_variable);

	/* Étendre le mot avec glob */
    char *reponse = process_glob(expanded_cmd);

    free(expanded_cmd);
    free(expanded_substring);

    return reponse;
}

/*
	Fonction pour étendre un mot à l'aide de wordexp
*/
char* process_wordexp(const char *line){
    wordexp_t p;
    char* expanded_cmd;
    if(wordexp(line, &p, 0) == 0 && p.we_wordc != 0){
        size_t expanded_length = 0;
        for (size_t i = 0; i < p.we_wordc; i++) {
            expanded_length += strlen(p.we_wordv[i]);
        }

	    expanded_cmd = (char *) malloc((expanded_length + p.we_wordc) * sizeof(char));
        if (expanded_cmd == NULL) error_message("error en malloc");

	    expanded_cmd[0] = '\0'; // Inicializar el string como vacío
        for (size_t i = 0; i < p.we_wordc; i++) {
            strcat(expanded_cmd, p.we_wordv[i]);
	    	if(i != p.we_wordc-1){
				strcat(expanded_cmd, " ");
			}
        }
		expanded_cmd[expanded_length + p.we_wordc - 1] = '\0';
	    wordfree(&p);
    
    }else{
		size_t line_len = strlen(line);
        expanded_cmd = (char*) malloc((line_len+1)*sizeof(char));
        if(expanded_cmd == NULL) error_message("malloc");
		strcpy(expanded_cmd, line);
		expanded_cmd[line_len] = '\0';
    }

    return expanded_cmd;
}

/*
	Fonction pour étendre un mot à l'aide de glob
*/
char* process_glob(const char *line){
    glob_t glob_result;
    char* expanded_cmd;
    if (glob(line, GLOB_TILDE | GLOB_BRACE, NULL, &glob_result) == 0 && glob_result.gl_pathc != 0) {
        size_t expanded_length = 0;
        for (size_t i = 0; i < glob_result.gl_pathc; i++) {
            expanded_length += strlen(glob_result.gl_pathv[i]);
        }

	    expanded_cmd = malloc((expanded_length + glob_result.gl_pathc) * sizeof(char));
        if (expanded_cmd == NULL) error_message("error en malloc");

	    expanded_cmd[0] = '\0'; // Inicializar el string como vacío
        for (size_t i = 0; i < glob_result.gl_pathc; i++) {
            strcat(expanded_cmd, glob_result.gl_pathv[i]);
			if(i != glob_result.gl_pathc-1){
				strcat(expanded_cmd, " ");
			}
		}
		expanded_cmd[expanded_length + glob_result.gl_pathc - 1] = '\0';
	    globfree(&glob_result);

    } else {
		size_t line_len = strlen(line);
        expanded_cmd = (char*) malloc((line_len+1)*sizeof(char));
        if(expanded_cmd == NULL) error_message("malloc");
        strcpy(expanded_cmd, line);
		expanded_cmd[line_len] = '\0';
    }
    return expanded_cmd;
}

/*
	Fonction pour ouvrir le fichier d'entrée
*/
int open_input_fd(struct cmdline *l){
	int input_fd = -1;  
	if(l->in != NULL){
	    input_fd = open(l->in, O_RDONLY);
	    if(input_fd == -1) error_message("error en open");
	}
	return input_fd;
}

/*
	Fonction pour ouvrir le fichier de sortie
*/
int open_output_fd(struct cmdline *l){
	int output_fd = -1;
	if(l->out != NULL){
	    output_fd = open(l->out, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	    if(output_fd == -1) error_message("error en open");
		
		/* Truncar el archivo a longitud 0 para borrar el contenido existente */
    	if(ftruncate(output_fd, 0) == -1) error_message("error en ftruncate");
	}
	return output_fd;
}

/*
	Fonction pour afficher les jobs
*/
void afficher_jobs(){
	struct Job *current_job = jobs;
	int status;
	
	while(current_job != NULL){
		/* Si el proceso sigue en segundo plano */
		if(waitpid(current_job->pid, &status, WNOHANG) == 0){
			printf("PID: %d\t Command: ", current_job->pid);

			/* Imprimir la lista de comandos */			
			for(int j = 0; current_job->cmd[j] != NULL; j++){
				printf("%s ", current_job->cmd[j]);
			}
			printf("\n");
		}
		current_job = current_job->next;
	}
}

/* 
	Fonction pour ajouter un nouveau processus à la liste des travaux (jobs)
*/
void ajouter_job(pid_t pid, char** cmd){

	/* Créer un nouveau travail */
	struct Job *new_job = malloc(sizeof(struct Job));
    if (new_job == NULL) error_message("error en malloc");
	gettimeofday(&new_job->start_time, NULL);
	new_job->pid = pid;

	/* Ajouter la commande */
	int cmd_len = calculer_cmd_len(cmd);
	new_job->cmd = malloc((cmd_len + 1) * sizeof(char*));
	if(new_job->cmd == NULL) error_message("Error en malloc");
	for(int i = 0; i < cmd_len; i++){
        new_job->cmd[i] = strdup(cmd[i]);
        if(new_job->cmd[i] == NULL) error_message("Error en strdup");
    }
    new_job->cmd[cmd_len] = NULL;
	
	/* Ajouter le travail au début de la liste */
	new_job->next = jobs;
	jobs = new_job;

}

/*
	Fonction pour supprimer un job
*/
void supprimer_job(struct Job *job){
    for(int i = 0; job->cmd[i] != NULL; i++){
        free(job->cmd[i]);
    }
    free(job->cmd);
    free(job);
}

/*
	Fonction pour supprimer les jobs
*/
void supprimer_jobs(){
	while(jobs != NULL){
        struct Job *temp = jobs;
        jobs = jobs->next;
        supprimer_job(temp);
    }
}

/*
	Fonction pour afficher le cmdline
*/
void display_cmdline(struct cmdline *l){
	if(l->err) printf("error: %s\n", l->err);
	if(l->in) printf("in: %s\n", l->in);
	if(l->out) printf("out: %s\n", l->out);
	if(l->bg) printf("background (&)\n");

	/* Display each command of the pipe */
	for(int i = 0; l->seq[i] != 0; i++){
		char **cmd = l->seq[i];
		printf("seq[%d]: ", i);
        for(int j = 0; cmd[j] != 0; j++){
        	printf("'%s' ", cmd[j]);
        }
		printf("\n");
	}
}

/*
	Fonction qui calcule la taille d'une commande
*/
int calculer_cmd_len(char** cmd){
	int cmd_len = 0;
    while (cmd[cmd_len] != 0) {
        cmd_len++;
    }
	return cmd_len;
}

/*
	Fonction qui calcule la taille de la séquence
*/
int calculer_seq_len(struct cmdline *l){
	int seq_len = 0;
    while(l->seq[seq_len] != NULL){
        seq_len++;
    }
	return seq_len;
}

/*
	Fonction pour terminer le programme
*/
void terminate(char *line){
	#if USE_GNU_READLINE == 1
	clear_history();
	#endif
	
	supprimer_jobs();

	if(line) free(line);
	printf("exit\n");
	exit(0);
}

/* 
	Fonction de gestion des erreurs 
*/
void error_message(char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}
