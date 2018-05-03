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

int main(){

    check_if_shm_exists();
    sem_unlink(SEM_X);
    sem_unlink(SEM_Y);
    sem_unlink(SEM_Z);
    sem_unlink(SEM_RSEM);
    sem_unlink(SEM_WSEM);
    shm_unlink(SHM_DELETE);

    return 0;
}