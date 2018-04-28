#ifndef SHARED_DATA_H
#define SHARED_DATA_H
#include <semaphore.h>

#define SEM_WRITE   "SEM_WRITE"
#define SEM_MUTEX   "SEM_MUTEX"
#define FLAG_READ 1
#define FLAG_WRITE 1
#define FLAG_MUTEX 1

#define SHM_DEFAULT_FLAGS O_CREAT|O_RDWR
sem_t *mutex;  /* Semaphore for readcount */
sem_t *wrt; /* Semaphor for writing data */
shared_memory *addr;
void open_semaphores();
#endif