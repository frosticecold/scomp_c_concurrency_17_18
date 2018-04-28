#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "sm.h"
#include "shared_data.h"
#include "personal_data.h"

int insertdata();

int main()
{
    /* Create shared memory and semaphores 
    addr shared memory
    wrt - Write semaphore
    mutex - Mutual exclusion semaphore to readcount
    */

    /* Disclaimer
    Problem with this solution is that writer may starve,
    waiting for its turn to write
   */
    addr = create_shared_memory(SHM_DEFAULT_FLAGS);
    open_semaphores();
    int runagain = 0;
    do
    {
        /** We are writing */
        if (sem_wait(wrt) == -1)
        {
            perror("Error : sem_wait(wrt)");
            exit(1);
        }
        int r = insertdata();
        if (sem_post(wrt) == -1)
        {
            perror("Erro: sem_post(wrt");
            exit(1);
        }
        //We finished writing
        if (r < 0)
        {
            printf("Error inserting.");
            break;
        }
        printf("Read again? 0/1");
        scanf("%d", &runagain);
    } while (runagain);

    return 0;
}

int insertdata()
{
    printf("Please enter your data:\n");
    PersonalData pd;
    printf("Name:\n");
    scanf("%s", pd.name);
    printf("Address:\n");
    scanf("%s", pd.address);
    printf("Number:\n");
    scanf("%d", &pd.number);
    int idx = addr->index;
    if (idx >= DB_SIZE)
    {
        return -1;
    }
    memcpy(&(addr->db[idx]), &pd, sizeof(pd));
    (addr->index)++;
    return 0;
}