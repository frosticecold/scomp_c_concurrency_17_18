#ifndef SHARED_DATA_H
#define SHARED_DATA_H
#include <semaphore.h>
#include "sm.h"

#define SEM_MUTEX 1
#define SEM_X "SEM_X"
#define SEM_Y "SEM_Y"
#define SEM_Z "SEM_Z"
#define SEM_RSEM "SEM_RSEM"
#define SEM_WSEM "SEM_WSEM"
shared_memory *addr;
sem_t *z;
sem_t *x;
sem_t *y;
sem_t *sem_rsem, *sem_wsem;
void open_semaphores();
void close_semaphores();
#endif