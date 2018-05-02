#include <semaphore.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include "sm.h"
#include <time.h>

#define SEM_EMPTY "/EMPTY"
#define SEM_FULL "/FULL"
#define SEM_INDEX "/INDEX_ACCESS"
#define CIRCULAR_BUFF_SIZE 300
shared_memory *addr;
sem_t *empty, *full, *index_access;

void rmv_str_shm(shared_memory *addr);
int main()
{
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_INDEX);
    check_if_shm_exists();
    const char *MSG = "I'm the Father - with PID ";
    const int n = 5; /* Number of processes  */
    pid_t pid[n];    /* Vector of processes  */
    int i;           /* loop variable        */

    addr = create_shared_memory();

    sem_unlink(SEM_EMPTY);
    /* criar semaforo com 50 posicoes */
    if ((empty = sem_open(SEM_EMPTY, O_CREAT | O_EXCL, 0644, NUM_OF_STRIGS)) == SEM_FAILED)
    {
        perror("Can't sem_open() SEM_EMPTY");
        exit(1);
    }

    if ((full = sem_open(SEM_FULL, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Can't sem_open() SEM_FULL");
        exit(1);
    }

    if ((index_access = sem_open(SEM_INDEX, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Can't sem_open() SEM_INDEX");
        exit(1);
    }
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
        struct timespec twait;
        twait.tv_sec = 12;
        twait.tv_nsec = 0;
        srand(time(NULL));
        int keeprunning = 1;
        do
        {
            int ret = sem_timedwait(empty, &twait);
            if (ret != -1)
            {
                sem_wait(index_access);
                int idx = addr->index;
                addr->index++;
                sem_post(index_access);
                if (idx < CIRCULAR_BUFF_SIZE)
                {
                    sprintf(addr->str[(idx)%NUM_OF_STRIGS], "%s %d\n", MSG, getpid());
                    idx=(idx+1);
                }
                if (idx >= CIRCULAR_BUFF_SIZE)
                    keeprunning = 0;
            }
            else
            {
                printf("Warning: %d timedout.\n", getpid());
            }

            sem_post(full);
            int rnd = (rand() % 3) + 1;
            sleep(1);
        } while (keeprunning);
        exit(0);
    }
    /**
     * Fathers Code
     * */
    else
    {
        int read_index = 0;

        for (i = 0; i < CIRCULAR_BUFF_SIZE; ++i)
        {
            sem_wait(full);
            printf("[%d] %s\n", i, addr->str[read_index]);
            sem_post(empty);
            read_index++;
            if (read_index == NUM_OF_STRIGS)
                read_index = 0;

        }
    }
    delete_shared_memory(addr);
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_INDEX);
    return 0;
}
void rmv_str_shm(shared_memory *addr)
{
    int idx;
    if (addr->index == NUM_OF_STRIGS)
        idx = addr->index - 1;
    printf("Removing %d line\n %s\n", idx, addr->str[idx]);
    strcpy(addr->str[idx], "");
    addr->index--;
}