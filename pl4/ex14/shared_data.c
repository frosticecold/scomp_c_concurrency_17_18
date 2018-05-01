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


    z = sem_open(SEM_Z, O_CREAT, 0644, SEM_MUTEX);
    if (z == SEM_FAILED)
    {
        perror("sem_z  sem_open()");
        exit(1);
    }

    x = sem_open(SEM_X, O_CREAT, 0644, SEM_MUTEX);
    if (x == SEM_FAILED)
    {
        perror("sem_x sem_open()");
        exit(1);
    }
    y = sem_open(SEM_Y, O_CREAT, 0644, SEM_MUTEX);
    if (y == SEM_FAILED)
    {
        perror("sem_y sem_open()");
        exit(1);
    }
    sem_rsem = sem_open(SEM_RSEM, O_CREAT, 0644, SEM_MUTEX);
    if (sem_rsem == SEM_FAILED)
    {
        perror("sem_rsem sem_open()");
        exit(1);
    }

    sem_wsem = sem_open(SEM_WSEM, O_CREAT, 0644, SEM_MUTEX);
    if (sem_wsem == SEM_FAILED)
    {
        perror(" sem_wsem sem_open()");
        exit(1);
    }

}

void close_semaphores()
{

    int r = sem_close(z);
    if (r == -1)
    {
        perror("Erro ao fechar o semaforo z");
    }
    r = sem_close(x);
    if (r == -1)
    {
        perror("Erro ao fechar o semaforo x");
    }
    r = sem_close(y);
    if (r == -1)
    {
        perror("Erro ao fechar o semaforo y");
    }
    r = sem_close(sem_rsem);
    if (r == -1)
    {
        perror("Erro ao fechar o semaforo sem_rsem");
    }
        r = sem_close(sem_wsem);
    if (r == -1)
    {
        perror("Erro ao fechar o semaforo sem_wsem");
    }
}