#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "sm.h"
#include "shared_data.h"

void consultdata(int id);
int main()
{
    int r_id; /* Read id */
    int runagain = 0;
    addr = create_shared_memory(SHM_DEFAULT_FLAGS);
    open_semaphores();
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
        printf("Please enter your id number:\nInsert 0 to exit: ");
        scanf("%d", &r_id);
        consultdata(r_id);
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
        printf("Read again? 0/1\n");
        scanf("%d", &runagain);
    } while (runagain);

    return 0;
}

void consultdata(int id)
{
    int i;
    int found = 0;
    for (i = 0; i < addr->index; ++i)
    {
        if (id == addr->db[i].number)
        {
            printf("%d] %s %s\n",
                   addr->db[i].number,
                   addr->db[i].name,
                   addr->db[i].address);
            found = 1;
        }
    }
    if (!found)
    {
        printf("Record not found.\n");
    }
}