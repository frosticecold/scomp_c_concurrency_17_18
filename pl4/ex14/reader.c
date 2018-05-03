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

    int r;
    /*
        Garantir a vez do leitor, prender aqui se existirem mais leitores presos aqui.
    */
    r = sem_wait(z);
    if (r == -1)
    {
        perror("Error sem_wait(z)");
        exit(1);
    }
    /*  
    Garantir a vez do leitor modificar o numero de leitores
    */
    r = sem_wait(sem_rsem);
    if (r == -1)
    {
        perror("Error sem_wait(sem_rsem)");
        exit(1);
    }
    r = sem_wait(x);
    if (r == -1)
    {
        perror("Error sem_wait(x)");
        exit(1);
    }

    /*
    Modificar o número de leitores
    */
    addr->readcount++;
    if (addr->readcount == 1)
    {
        r = sem_wait(sem_wsem);
        if (r == -1)
        {
            perror("Error sem_wait(sem_wsem)");
            exit(1);
        }
    }
    /* Acabou de modificar o número de leitores*/
    r = sem_post(x);
    if (r == -1)
    {
        perror("Error sem_post(x)");
        exit(1);
    }
    /* Levantar rsem */
    r = sem_post(sem_rsem);
    if (r == -1)
    {
        perror("Error sem_post(sem_rsem)");
        exit(1);
    }

    /*
        Se levantou isto, é porque não ficou preso à espera de escritores
    */
    r = sem_post(z);
    if (r == -1)
    {
        perror("Error sem_post(z)");
        exit(1);
    }

    /*
        Zona crítica
    */
    printf("Reading: %s", addr->str);
    printf("Readers %d\nTime:%s", addr->readcount, addr->str);
    sleep(rand() % MAX_TIME);
    /*
     Modificar o valores de número de leitores
     */
    r = sem_wait(x);
    if (r == -1)
    {
        perror("Error sem_wait(x)");
        exit(1);
    }
    int readcount = --(addr->readcount);
    if (readcount == 0)
    {
        r = sem_post(sem_wsem);
        if (r == -1)
        {
            perror("Error sem_wait(z)");
            exit(1);
        }
    }
    /* Acabou de modificar o número de leitores */
    r = sem_post(x);
    if (r == -1)
    {
        perror("Error sem_wait(x)");
        exit(1);
    }

    close_semaphores();
    return 0;
}