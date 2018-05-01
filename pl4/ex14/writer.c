#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include "sm.h"
#include "shared_data.h"

#define MAX_TIME 10

int main()
{
    srand(time(NULL));
    addr = create_shared_memory(SHM_DEFAULT_FLAGS);
    open_semaphores();

    char buffer[STR_BUFFER];
    time_t now = time(0);
    sprintf(buffer, "PID:%d TIME:%s", getpid(), asctime(localtime(&now)));

    /** increment the number of writers **/
    sem_wait(y);
    addr->writercount++;
    if (addr->writercount == 1)
    {
        sem_wait(sem_rsem);
    }
    sem_post(y);

    printf("The number of writers:%d\nThe number of Readers:%d", addr->writercount, addr->readcount);

    sleep(rand()%MAX_TIME);
    /** critical section **/
    sem_wait(sem_wsem);
    strncpy(addr->str, buffer, STR_BUFFER);
    sem_post(sem_wsem);
    /** critical section **/

    /** decrement the number of writers **/
    sem_wait(y);
    addr->writercount--;
    if (addr->writercount == 0)
        sem_post(sem_rsem);
    sem_post(y);

    close_semaphores();
    return 0;
}