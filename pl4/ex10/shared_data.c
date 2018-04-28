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

    mutex = sem_open(SEM_MUTEX, O_CREAT, 0644, FLAG_READ);
    if (mutex == SEM_FAILED)
    {
        perror("mutex sem_open()");
        exit(1);
    }

    wrt = sem_open(SEM_WRITE, O_CREAT, 0644, FLAG_WRITE);
    if (wrt == SEM_FAILED)
    {
        perror("wrt sem_open()");
        exit(1);
    }
}