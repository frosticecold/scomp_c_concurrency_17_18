
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
    /** shared variables */
    addr = create_shared_memory(SHM_DEFAULT_FLAGS);
    open_semaphores();

    /* random time */
    srand(time(NULL));
    /* member variables */
    int number_of_ticket = 0;
    int keeprunning = 1;
    int sleeprnd = 0;
    int idx = 0; // circular index;
    /* loop */
    do
    {
        /** esperar que haja clientes para atender */
        sem_wait(full);
        sem_post(empty);

        //sem_wait(sembilhetes);
        idx = number_of_ticket % MAX_CLIENTS;
        addr->bilhetes[idx] = ++number_of_ticket;
        //sem_post(sembilhetes);
        sleeprnd = rand() % MAX_TIME;
        printf("Serving ticket:%d \n Sleeping for: %d\n",number_of_ticket,sleeprnd);
        sleep(sleeprnd);
        sem_post(sem_vec[idx]);

    } while (keeprunning);
}