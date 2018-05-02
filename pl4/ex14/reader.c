#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "sm.h"
#include "shared_data.h"
#include <time.h>
#define MAX_TIME 10
int main()
{
    srand(time(NULL));
    addr = create_shared_memory(SHM_DEFAULT_FLAGS);
    open_semaphores();

    sem_wait(z);
    sem_wait(sem_rsem);
    sem_wait(x);
    addr->readcount++;
    if (addr->readcount == 1)
        sem_wait(sem_wsem);
    sem_post(x);
    sem_post(sem_rsem);
    sem_post(z);

    printf("Readers %d\nTime:%s", addr->readcount, addr->str);
    sleep(rand()% MAX_TIME);
    sem_wait(x);
    int readcount = --(addr->readcount);
    if (readcount == 0)
        sem_post(sem_wsem);
    sem_post(x);

    close_semaphores();
    return 0;
}