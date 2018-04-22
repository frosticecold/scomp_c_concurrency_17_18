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
#define SEM_NAME "sem"
void open_sem();
int main()
{
    check_if_shm_exists();
    const int MAX_INDEX = 6;
    char *MSG[] = {"Sistemas ", "de ", "Computadores -", "a ", "melhor ", "disciplina! "};
    open_sem();
    addr = create_shared_memory();

    const int np = 3; /* number of processes */
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
        int idx = 0;
        do
        {
            sem_wait(sem);
            if (addr->index < MAX_INDEX)
            {
                printf("%s", MSG[addr->index]);
                addr->index++;
            }
            idx = addr->index;
            sem_post(sem);
        } while (idx < MAX_INDEX);
        exit(0);
    }
    /* Father code */
    else
    {
        for (i = 0; i < np; ++i)
        {
            waitpid(pid[i], NULL, 0);
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