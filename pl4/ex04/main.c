#include <semaphore.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include "sm.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define INDEX_NAME "SEM_INDEX"
#define MAX_TIME 5
shared_memory *addr;
sem_t *index_access;

int main()
{
    srand(time(NULL));

    addr = create_shared_memory();
    if (addr == NULL)
        return 1;

    if ((index_access = sem_open(INDEX_NAME, O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Can't sem_open()");
        exit(1);
    }

    struct timespec st;
    st.tv_sec = 12;
    st.tv_nsec = 0;
    int r = sem_timedwait(index_access,&st);
    if (r == -1)
    {
        printf("Error sem_wait\n");
        exit(1);
    }
    if (r == ETIMEDOUT)
    {
        printf("PID:%d timedout", getpid());
        exit(1);
    }
    if (addr->index > 0)
    {
        addr->index--;
        printf("Deleting: %s\nCurrent PID:%d\n", addr->str[addr->index],getpid());
        strcpy(addr->str[addr->index], "REMOVED THIS LINE");
        sleep(rand() % MAX_TIME);
        sem_post(index_access);
    }
    else
    {
        printf("Can't delete more...\n");
        sem_post(index_access);
    }

    //delete_shared_memory(addr);
    //sem_unlink(SEM_NAME);
    return 0;
}
