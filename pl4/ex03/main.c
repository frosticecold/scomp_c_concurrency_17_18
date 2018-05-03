#include <semaphore.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include "sm.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SEM_NAME "semaforo"
#define INDEX_NAME "SEM_INDEX"
#define MAX_TIME 5
shared_memory *addr;
sem_t *semaphore;
sem_t *index_access;

int main()
{
    srand(time(NULL));
    //check_if_shm_exists();
    const char *MSG = "I'm the Father - with PID ";

    addr = create_shared_memory();
    if (addr == NULL)
        return 1;

    //sem_unlink(SEM_NAME);
    if ((semaphore = sem_open(SEM_NAME, O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Can't sem_open()");
        exit(1);
    }

    if ((index_access = sem_open(INDEX_NAME, O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Can't sem_open()");
        exit(1);
    }

    char buffer[50];
    int r = sem_wait(index_access);
    if (r == -1)
    {
        perror("Error sem_wait(index_access)");
        exit(1);
    }
    else
    {
        sprintf(buffer, "%s %d [%d]", MSG, getpid(), addr->index);
        strcpy(addr->str[addr->index], buffer);
        addr->index++;
        sleep(rand() % MAX_TIME);
        r = sem_post(index_access);
        if (r == -1)
        {
            perror("Error sem_post(index_access)");
            exit(1);
        }
        printf("%s\n", buffer);
    }

    //delete_shared_memory(addr);
    //sem_unlink(SEM_NAME);
    return 0;
}
