#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "sm.h"
#include "shared_data.h"

void consultall();
int main()
{
    addr = create_shared_memory(SHM_DEFAULT_FLAGS);
    open_semaphores();
    int runagain = 0;
    do
    {
        /** Isolate readcount **/
        if (sem_wait(mutex) == -1)
        {
            perror("Erro sem_post(mutex)");
            exit(1);
        };
        (addr->readcount)++;
        if (addr->readcount == 1)
        {
            if (sem_wait(wrt) == -1)
            {
                perror("Erro sem_wait(wrt)");
                exit(1);
            }
        }
        if (sem_post(mutex) == -1)
        {
            perror("Erro sem_post(mutex)");
            exit(1);
        }
        /** End readcount **/
        /** Crtical part **/
        printf("Consulting all records...\n");
        consultall();
        /** End critical part **/
        /** Isolate read **/
        if (sem_wait(mutex) == -1)
        {
            perror("Erro sem_wait(mutex)");
            exit(1);
        }
        (addr->readcount)--;
        if (addr->readcount == 0)
        {
            if (sem_post(wrt) == -1)
            {
                perror("Erro sem_post(wrt)");
                exit(1);
            }
        }
        if (sem_post(mutex) == -1)
        {
            perror("Erro sem_post(mutext)");
            exit(1);
        }
        /** end isolate read **/
        printf("Run again? 0/1 ");
        scanf("%d", &runagain);
    } while (runagain);
    return 0;
}

void consultall()
{
    int i;
    for (i = 0; i < addr->index; ++i)
    {
        printf("%d] %s %s\n",
               addr->db[i].number,
               addr->db[i].name,
               addr->db[i].address);
    }
}