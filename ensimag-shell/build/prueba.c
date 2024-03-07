#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>

/* 
	Fonction de gestion des erreurs 
*/
void error_message(char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

int main() {
    pid_t pid;
    
    pid = fork();
    if (pid < 0) error_message("error en malloc");
    if (pid == 0) {

        struct rlimit rlim;
        rlim.rlim_cur = 1;
        rlim.rlim_max = 2; 

        if (setrlimit(RLIMIT_CPU, &rlim) != 0) error_message("error en setrlimit");

        char *args[] = {"./prueba2", NULL};
        execvp(args[0], args) == -1;

        printf("Programa terminado\n");

    } else {

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("El proceso hijo ha salido con código de salida: %d\n", WEXITSTATUS(status));
        } else {
            printf("El proceso hijo ha salido de manera anormal.\n");
        }
    }

    printf("Espera");
    while(1){}

    return 0;
}
