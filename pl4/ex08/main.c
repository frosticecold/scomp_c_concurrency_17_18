#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sm.h"
#include <sys/wait.h>
#include <time.h>

sem_t *sem_empty, *sem_full;
#define SEM_EMPTY "sem_empty"
#define SEM_FULL "sem_full"
void open_sem();
int main()
{
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    open_sem();
    pid_t pid;

    if ((pid = fork()) == -1)
    {
        perror("Error forking.");
        exit(0);
    }

    /*Child code print S*/
    if (pid == 0)
    {
        int r;
        while (1)
        {
            r = sem_wait(sem_empty);
            if (r == -1)
            {
                perror("Error sem_wait(sem_empty)");
                exit(1);
            }
            fflush(stdout);
            printf("S");
            r = sem_post(sem_full);
            if (r == -1)
            {
                perror("Error sem_post(sem_full)");
                exit(1);
            }
        }
        exit(0);
    }
    /* Father code */
    else
    {
        int r;
        while (1)
        {
            r = sem_wait(sem_full);
            if (r == -1)
            {
                perror("Error sem_wait(sem_full)");
                exit(1);
            }
            fflush(stdout);
            printf("C");
            r = sem_post(sem_empty);
            if (r == -1)
            {
                perror("Error sem_post(sem_empty)");
                exit(1);
            }
        }
    }
    sem_unlink(SEM_EMPTY);
    return 0;
}

void open_sem()
{
    if ((sem_empty = sem_open(SEM_EMPTY, O_CREAT | O_EXCL, 0644, 2)) == SEM_FAILED)
    {
        perror("No sem_open()");
        exit(1);
    }
    if ((sem_full = sem_open(SEM_FULL, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("No sem_open()");
        exit(1);
    }
}