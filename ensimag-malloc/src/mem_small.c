/******************************************************
 * Copyright Grégory Mounié 2018                      *
 * This code is distributed under the GLPv3+ licence. *
 * Ce code est distribué sous la licence GPLv3+.      *
 ******************************************************/

#include <assert.h>
#include <pthread.h>

#include "mem.h"
#include "mem_internals.h"

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void *
emalloc_small(unsigned long size)
{
    pthread_mutex_lock(&mutex1);
    
    if(arena.chunkpool == 0){

        // Demander au système un bloc pour arena.chunkpool
        unsigned long sizeBloc = mem_realloc_small();
        
        // Construire la liste chaînée
        void* currentChunk = arena.chunkpool;        
        for(int i = 0; i < sizeBloc-CHUNKSIZE; i += CHUNKSIZE){
            void* nextChunk = currentChunk + CHUNKSIZE;
            *((void **) currentChunk) = nextChunk;
            currentChunk = nextChunk;
        }
        *((void **)currentChunk) = NULL;
    }

    // Supprimer le premier élément de la liste
    void *ptr = arena.chunkpool;
    arena.chunkpool = *((void **) ptr);
    
    pthread_mutex_unlock(&mutex1);

    // Marquer l'élément extrait
    return mark_memarea_and_get_user_ptr(ptr, CHUNKSIZE, SMALL_KIND);
}

void efree_small(Alloc a) {
    pthread_mutex_lock(&mutex1);

    // Ajouter le nouvel élément en tête de la liste
    *((void**) a.ptr) = arena.chunkpool; 
    arena.chunkpool = a.ptr;
    
    pthread_mutex_unlock(&mutex1);
}
