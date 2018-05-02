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

#define MAX_CARS 30
#define SLEEP_TIME 3
int main()
{

    addr = create_shared_memory(SHM_DEFAULT_FLAGS);
    open_semaphores();
    pid_t pid;
    pid = fork();

    if (pid == -1)
    {
        perror("Erro ao criar fork()");
        return EXIT_FAILURE;
    }

    /** Father - Cards from East to the WEST**/
    if (pid > 0)
    {
        srand(time(NULL));
        addr->num_cars_east = rand() % MAX_CARS;
        printf("East Cars: %d\n", addr->num_cars_east);
        int indx = 0;
        do
        {
            sem_wait(directionWEST);
            sem_wait(bridge);
            /** cross bridge **/
            addr->num_cars_east--;
            indx++;
            printf("Car %d going from EAST\n", indx);
            sleep(rand() % SLEEP_TIME);
            sem_post(bridge);
            if (addr->num_cars_east > 0)
                sem_post(directionEAST);
            else
            {
                int i;
                for (i = 0; i < addr->num_cars_west; i++)
                {
                    sem_post(directionEAST);
                }
                break;
            }
        } while (1);
    }
    /** Child - Cars from the West to the EAST **/
    else
    {
        srand(time(NULL) * getpid());
        addr->num_cars_west = rand() % MAX_CARS;
        int indx = 0;
        printf("West Cards: %d\n", addr->num_cars_west);
        do
        {
            sem_wait(directionEAST);
            sem_wait(bridge);
            /** cross bridge **/
            indx++;
            addr->num_cars_west--;
            printf("Car %d going from WEST\n", indx);
            sleep(rand() % SLEEP_TIME);
            sem_post(bridge);
            if (addr->num_cars_west > 0)
                sem_post(directionWEST);
            else
            {
                int i;
                for (i = 0; i < addr->num_cars_east; ++i)
                    sem_post(directionEAST);
                break;
            }
        } while (1);
        exit(0);
    }
    waitpid(pid, NULL, 0);
    printf("Run finished!\n");
    delete_shared_memory(addr);
    close_semaphores();
    return 0;
}