#include <semaphore.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include "sm.h"

shared_memory *addr;
sem_t *semaphore;
int main()
{
    check_if_shm_exists();
    const char *MSG = "I'm the Father - with PID ";

    addr = create_shared_memory();

    sem_unlink("semaforo");
    if ((semaphore = sem_open("semaforo", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Can't sem_open()");
        exit(1);
    }

    const int n = 10;
    pid_t pid[n];
    int i;
    for (i = 0; i < n; i++)
    {
        pid[i] = fork();
        if (pid[i] == -1)
        {
            perror("Error forking.");
            exit(1);
        }
        if (pid[i] == 0)
        {
            break;
        }
    }

    if (i == n)
        i = 0;
    /**
     * Child code
     * */
    if (pid[i] == 0)
    {
        while (addr->index < NUM_OF_STRIGS)
        {
            sem_wait(semaphore);
            int idx = addr->index;
            if (idx < NUM_OF_STRIGS)
            {
                sprintf(addr->str[idx], "%s %d\n", MSG, getpid());
                addr->index++;
            }
            sem_post(semaphore);
            int rnd = (rand() % 5) + 1;
            sleep(rnd);
        }
        exit(0);
    }
    /**
     * Fathers Code
     * */
    else
    {
        for (i = 0; i < n; ++i)
        {
            waitpid(pid[i], NULL, 0);
        }
        for (i = 0; i < NUM_OF_STRIGS; ++i)
        {
            printf("[%d] %s", i, addr->str[i]);
        }
    }
    delete_shared_memory(addr);
    return 0;
}
