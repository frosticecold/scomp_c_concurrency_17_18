#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sm.h"
#include <sys/wait.h>

sem_t *sem;
shared_memory *addr;
#define SEM_NAME "sem_ex08"
void open_sem();
int main()
{
    sem_unlink(SEM_NAME);
    check_if_shm_exists();
    open_sem();
    addr = create_shared_memory();

    const int np = 1; /* number of processes */
    int i;
    pid_t pid[np];
    for (i = 0; i < np; ++i)
    {
        if ((pid[i] = fork()) == -1)
        {
            perror("Error forking.");
            exit(0);
        }
    }
    if (i == np)
        i = 0;
    /*Child code*/
    if (pid[i] == 0)
    {
        srand(time(NULL) * getpid());
        while (1)
        {
            sem_wait(sem);
            if (addr->numS < 2)
            {
                printf("S\n");
                addr->numS++;
            }
            if (addr->numC >= 2)
            {
                addr->numC = 0;
            }
            sem_post(sem);
            //sleep(rand() % 5);
        }
        exit(0);
    }
    /* Father code */
    else
    {
        srand(time(NULL) * getpid());
        while (1)
        {
            sem_wait(sem);
            if (addr->numC < 2)
            {
                printf("C\n");
                addr->numC++;
            }
            if (addr->numS >= 2)
            {
                addr->numS = 0;
            }
            sem_post(sem);
            //sleep(rand() % 5);
        }
    }
    delete_shared_memory(addr);
    sem_unlink(SEM_NAME);
    return 0;
}

void open_sem()
{
    if ((sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("No sem_open()");
        exit(1);
    }
}