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
    sem_close(full);
    sem_close(empty);
    sem_close(semcliente);
    sem_close(sembilhetes);
    int i;
    for (i = 0; i < MAX_CLIENTS; i++)
    {
        sem_close(sem_vec[i]);
    }
    unlink(SHM_DELETE);

    addr = create_shared_memory(O_CREAT | O_RDWR);
    memset(addr,0,sizeof(shared_memory));
    open_semaphores();

    return 0;
}