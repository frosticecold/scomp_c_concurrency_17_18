#ifndef SHARED_DATA_H
#define SHARED_DATA_H
#include <semaphore.h>
#include "sm.h"
/* STRINGS SEM*/
#define SEM_FULL        "SEM_FULL"
#define SEM_EMPTY       "SEM_EMPTY"
#define SEM_VEC_NAME    "SEM_VEC"
#define SEM_INDICE      "SEM_MUTEX"
#define SEM_BILHETES    "SEM_BILHETE"
/* VALUES OF SEM*/
#define SEM_FULL_VALUE        0
#define SEM_EMPTY_VALUE       MAX_CLIENTS
#define SEM_VEC_VALUE         0
#define SEM_MUTEX_VALUE       1

sem_t *full;                 /* Semaphore for full */
sem_t *empty;                /* Semaphor for empty */
sem_t *sem_vec[MAX_CLIENTS]; // 30 semaphores
sem_t *semcliente;
sem_t *sembilhetes;
shared_memory *addr;
void open_semaphores();
void close_semaphores();
#endif