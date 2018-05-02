#ifndef SHARED_DATA_H
#define SHARED_DATA_H
#include <semaphore.h>
#include "sm.h"

#define SEM_MUTEX 1
#define SEM_BRIDGE "SEM_BRIDGE"
#define SEM_DIRECTIONEAST "SEM_DIRECTIONEAST"
#define SEM_DIRECTIONWEST "SEM_DIRECTIONWEST"
shared_memory *addr;
sem_t *bridge;
sem_t *directionEAST;
sem_t *directionWEST;
void open_semaphores();
void close_semaphores();
#endif