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
    int time_to_cross = 5;
    srand(time(NULL));
    if (pid == -1)
    {
        perror("Erro ao criar fork()");
        return EXIT_FAILURE;
    }
    addr->num_cars_east = (rand() % MAX_CARS) + 1;
    addr->num_cars_west = (rand() % MAX_CARS) + 1;
    printf("East Cars: %d\n", addr->num_cars_east);
    printf("West Cards: %d\n", addr->num_cars_west);

    /** Father - Cars from East to the WEST**/
    if (pid > 0)
    {
        int current_time_limit = 0;

        int indx = 0;
        while (1)
        {
            if (addr->num_cars_east >= time_to_cross)
            {

                current_time_limit = time_to_cross;
                printf("%d INSIDE IF EAST\n", current_time_limit);
            }
            else if (addr->num_cars_east < time_to_cross && addr->num_cars_east > 0)
            {
                current_time_limit = addr->num_cars_east;
                printf("%d INSIDE ELSE EAST\n", current_time_limit);
            }
            else
            {
                break;
            }
            sem_wait(bridge);
            while (current_time_limit != 0)
            {
                /** cross bridge **/
                addr->num_cars_east--;
                indx++;
                current_time_limit--;
                printf("Time Limit %d\n", current_time_limit);
                printf("Car %d going from EAST\n", indx);
            }

            printf("%d Cars had crossed the bridge\n", indx);
            indx = 0;
            printf("GIVE TURN TO WEST\n");

            sem_post(bridge);
            
            sleep(1);
            sem_post(directionWEST);
            sem_wait(directionEAST);
            if (addr->num_cars_east == 0)
            {
                
                printf("The cars from East side had all crossed\n");
                sem_post(directionWEST);
                break;
            }
        }
        sem_post(directionWEST);
    }
    /** Child - Cars from the West to the EAST **/
    else
    {
        int indx = 0;
        int current_time_limit = 0;
        while (1)
        {

            if (addr->num_cars_west >= time_to_cross)
            {
                current_time_limit = time_to_cross;
            }
            else if (addr->num_cars_west < time_to_cross && addr->num_cars_west > 0)
            {
                current_time_limit = addr->num_cars_west;
            }
            else
            {
                break;
            }
            sem_wait(bridge);
            while (current_time_limit != 0)
            {

                /** cross bridge **/
                addr->num_cars_west--;
                indx++;
                printf("Time Limit %d\n", current_time_limit);
                printf("Car %d going from WEST\n", indx);
                current_time_limit--;
            }

            printf("%d Cars had crossed the bridge\n", indx);
            indx = 0;
            printf("GIVE TURN TO EAST\n");

            sem_post(bridge);
            
            sleep(1);
            sem_post(directionEAST);
            sem_wait(directionWEST);
            if (addr->num_cars_west == 0)
            {
                sem_post(directionEAST);
                printf("The cars from West side had all crossed\n");
                break;
            }
        }
        sem_post(directionEAST);
        exit(0);
    }
    waitpid(pid, NULL, 0);
    printf("Run finished!\n");
    delete_shared_memory(addr);
    close_semaphores();
    return 0;
}