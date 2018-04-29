#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "sm.h"
#include "shared_data.h"

void open_semaphores()
{

    full = sem_open(SEM_FULL, O_CREAT, 0644, SEM_FULL_VALUE);
    if (full == SEM_FAILED)
    {
        perror("full sem_open()");
        exit(1);
    }

    empty = sem_open(SEM_EMPTY, O_CREAT, 0644, SEM_EMPTY_VALUE);
    if (empty == SEM_FAILED)
    {
        perror("empty sem_open()");
        exit(1);
    }
    semcliente = sem_open(SEM_INDICE, O_CREAT, 0644, SEM_MUTEX_VALUE);
    if (semcliente == SEM_FAILED)
    {
        perror("mutex sem_open()");
        exit(1);
    }
    sembilhetes = sem_open(SEM_BILHETES, O_CREAT, 0644, SEM_MUTEX_VALUE);
    if (sembilhetes == SEM_FAILED)
    {
        perror("sembilhetes sem_open()");
        exit(1);
    }
    int i;
    for (i = 0; i < MAX_CLIENTS; ++i)
    {
        char str[32];
        sprintf(str, "%s %d", SEM_VEC_NAME, i);
        sem_vec[i] = sem_open(str, O_CREAT, 0644, SEM_VEC_VALUE);
        if (sem_vec[i] == SEM_FAILED)
        {
            perror("empty sem_open()");
            exit(1);
        }
    }
}

void close_semaphores()
{

    int r = sem_close(full);
    if (r == -1)
    {
        perror("Erro ao fechar o semaforo FULL");
    }
    r = sem_close(empty);
    if (r == -1)
    {
        perror("Erro ao fechar o semaforo EMPTY");
    }
    r = sem_close(semcliente);
    if (r == -1)
    {
        perror("Erro ao fechar o semaforo INDICE");
    }
    r = sem_close(sembilhetes);
    if (r == -1)
    {
        perror("Erro ao fechar o semaforo BILHETES");
    }
    int i;
    for (i = 0; i < MAX_CLIENTS; i++)
    {
        r = sem_close(sem_vec[i]);
        if (r == -1)
        {
            perror("Erro ao fechar o semaforo FULL");
        }
    } 
}