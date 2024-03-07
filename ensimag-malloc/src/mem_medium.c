/******************************************************
 * Copyright Grégory Mounié 2018                      *
 * This code is distributed under the GLPv3+ licence. *
 * Ce code est distribué sous la licence GPLv3+.      *
 ******************************************************/

#include <stdint.h>
#include <assert.h>
#include <pthread.h>

#include "mem.h"
#include "mem_internals.h"

pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

unsigned int puiss2(unsigned long size) {
    unsigned int p=0;
    size = size -1; // allocation start in 0
    while(size) {  // get the largest bit
	p++;
	size >>= 1;
    }
    if (size > (1 << p))
	p++;
    return p;
}


void *
emalloc_medium(unsigned long size)
{
    assert(size < LARGEALLOC);
    assert(size > SMALLALLOC);
    
    // Additionner les 32 octets de marquage
    size += 32;

    // Aligner le bloc avec les 32 octets
    size += 32 - (size % 32);

    // Calculer l'indice approprié dans arena.TZL
    unsigned int indice = puiss2(size);

    // Trouver le bloc disponible plus grand
    unsigned int indiceAux = indice;    
    
    pthread_mutex_lock(&mutex2);
    
    while(arena.TZL[indiceAux] == 0){

        // Si nous atteignons le dernier bloc, demander au système un nouveau bloc aligné
        if(indiceAux == FIRST_ALLOC_MEDIUM_EXPOSANT + arena.medium_next_exponant){
            unsigned long sizeBloc = mem_realloc_medium();   
                
            // Construire la liste chaînée
            void* currentTZL = arena.TZL[indiceAux];
            for(int i = 0; i < sizeBloc-(1UL << indiceAux); i += (1UL << indiceAux)){
                void* nextTZL = currentTZL + (1UL << indiceAux);
                *((void **) currentTZL) = nextTZL;
                currentTZL = nextTZL;
            }
            *((void **) currentTZL) = NULL;
            break;
        }
        indiceAux += 1;
    }

    for(int i = indiceAux; i > indice; i--){
            
        // Supprimer le premier élément de la liste dans TZL[i]
        void* ptr = arena.TZL[i];
        arena.TZL[i] = *((void **) ptr);
        
        // Diviser et ajouter les blocs à la liste de arena.TZL[i-1]
        void* headptr = arena.TZL[i-1];
        *((void **)(ptr + (1UL << (i-1)))) = headptr;
        *((void **)ptr) = (ptr + (1UL << (i-1)));
        arena.TZL[i-1] = ptr;
    }

    // Supprimer le premier élément de la liste arena.TZL[indice]
    void* ptr = arena.TZL[indice];
    arena.TZL[indice] = *((void **) ptr);

    pthread_mutex_unlock(&mutex2);

    // Marquer l'élément extrait
    return mark_memarea_and_get_user_ptr(ptr, size, MEDIUM_KIND);
}


void efree_medium(Alloc a) {
    
    pthread_mutex_lock(&mutex2);

    init_efree:

    // Trouver le compagnon (buddy).
    unsigned int indice = puiss2(a.size);
    void* ptrBuddy = (void *)((uintptr_t) a.ptr ^ (1UL << indice));

    void* ptrPrecedent = NULL;
    void* ptrCourrant = arena.TZL[indice];

    // Vérifier si le compagnon est dans la liste correspondante
    while(ptrCourrant != 0){
        if(ptrCourrant == ptrBuddy){  
            
            // Supprimer l'élément compagnon de la liste arena.TZL[indice]
            if(ptrPrecedent != NULL){
                *((void **)ptrPrecedent) = *((void **) ptrCourrant);
            }else{
                arena.TZL[indice] = *((void **) arena.TZL[indice]);
            }

            // Trouver le début du nouveau bloc fusionné
            void* ptrMin = a.ptr;
            if(ptrBuddy < a.ptr){
                ptrMin = ptrBuddy;
            }

            // Répéter récursivement pour le bloc fusionné
            Alloc newA = {
                .ptr = ptrMin,
                .size = a.size*2,
                .kind = MEDIUM_KIND
            };

            a = newA;

            goto init_efree;
        }

        ptrPrecedent = ptrCourrant;
        ptrCourrant = *((void **) ptrCourrant);
    }

    // Insérer l'élément dans la liste arena.TZL[indice]
    *((void **)(a.ptr)) = arena.TZL[indice];
    arena.TZL[indice] = a.ptr;
    
    pthread_mutex_unlock(&mutex2);

    return;
}


