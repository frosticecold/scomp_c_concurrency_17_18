#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "sm.h"
#include "shared_data.h"

int main()
{
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_VEC_NAME);
    sem_unlink(SEM_INDICE);

    int i;
    for (i = 0; i < MAX_CLIENTS; i++)
    {
        char str[32];
        sprintf(str, "%s %d", SEM_VEC_NAME, i);
        sem_unlink(str);
    }
    unlink(SHM_DELETE);

    addr = create_shared_memory(O_CREAT | O_RDWR);
    memset(addr, 0, sizeof(shared_memory));
    open_semaphores();

    return 0;
}