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

    open_semaphores();
    addr = create_shared_memory(SHM_DEFAULT_FLAGS);

    /** vai tirar vez**/
    int keeprunning = 1;
    int idx = 0;
    int nrbilhete = 0;
    do
    {

        /** esperar por uma vez*/
        sem_post(full);
        sem_wait(empty);
        /* conseguiu esperar por uma vez, incrementar num clientes*/
        sem_wait(semcliente);
        idx = addr->numClientes++;
        if (idx > MAX_CLIENTS)
            idx = idx % MAX_CLIENTS;
        sem_post(semcliente);
        /** vai esperar que seja atendido **/
        sem_wait(sem_vec[idx]);
        nrbilhete = addr->bilhetes[idx];
        printf("O bilhete do cliente %d é:%d", idx, nrbilhete);
        keeprunning = 0;
    } while (keeprunning);

    close_semaphores();
    return 0;
}