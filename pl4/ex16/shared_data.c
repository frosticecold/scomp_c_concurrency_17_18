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

    bridge = sem_open(SEM_BRIDGE, O_CREAT, 0644, SEM_MUTEX);
    if (bridge == SEM_FAILED)
    {
        perror("sem_z  sem_open()");
        exit(1);
    }
    directionEAST = sem_open(SEM_DIRECTIONEAST, O_CREAT, 0644, 1);
    if (directionEAST == SEM_FAILED)
    {
        perror("direction  sem_open()");
        exit(1);
    }
    directionWEST = sem_open(SEM_DIRECTIONWEST, O_CREAT, 0644, 0);
    if (directionWEST == SEM_FAILED)
    {
        perror("direction sem_open()");
        exit(1);
    }
}

void close_semaphores()
{

    int r = sem_close(bridge);
    if (r == -1)
    {
        perror("Erro ao fechar o semaforo bridge");
    }
    r = sem_close(directionEAST);
    if (r == -1)
    {
        perror("Erro ao fechar o semaforo directionEAST");
    }
    r = sem_close(directionWEST);
    if (r == -1)
    {
        perror("Erro ao fechar o semaforo directionWEST");
    }
    /*
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
    */
}