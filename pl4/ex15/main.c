#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define SEM_NAME_EMPTY "semaphore_empty_ex15"
#define SEM_NAME_FULL "semaphore_full_ex15"

#define SHOWROOMS 5
#define MAXTIME 5

int main()
{
    /*==================== INITIALIZE VARIABLES ====================*/

    pid_t pid[SHOWROOMS];
    int status[SHOWROOMS];
    sem_t *sem_full;
    sem_t *sem_empty;

    sem_full = sem_open(SEM_NAME_FULL, O_CREAT | O_EXCL, 0644, 0);
    //value_empty 0 to coordidate with the sem_empty semaphore
    if (sem_full == SEM_FAILED)
    {
        perror("Can't sem_open() sem_full\n");
    }

    sem_empty = sem_open(SEM_NAME_EMPTY, O_CREAT | O_EXCL, 0644, 5);
    //value_empty 30 for mutual exclusion so it will block after the process
    if (sem_empty == SEM_FAILED)
    {
        perror("Can't sem_open() sem_empty\n");
    }

    /*==============================================================*/

    /*===================  SHOWROOM EXPOSITION  ====================*/

    int i = 0;
    for (; i < SHOWROOMS; i++)
    {
        pid[i] = fork();
        if (pid[i] == -1)
        {
            perror("error fork()\n");
        }
        srand(time(NULL));
        int show_length = rand() % MAXTIME + 1;
        if (pid[i] > 0) /*FATHER ------ EXITING PEOPLE*/
        {
            do
            {
                sem_wait(sem_full);
                printf("Finished the visit\n");
                sleep(1);
                printf("Exited the exposition\n");
                sem_post(sem_empty);
            } while (--show_length);
            if (waitpid(pid[i], &status[i], 0)) // if the show is over
            {
                if (WIFEXITED(status[i])) //empty the current showroom
                {

                    printf("Emptying the room\n");
                    int people = WEXITSTATUS(status[i]); //get the ammount of people left in the current showroom
                    if (people > 0)                      // if there is people
                    {
                        int index = 0;
                        for (; index < people - 1; index++)
                        {
                            sem_wait(sem_full);  //notifies people exiting
                            sem_post(sem_empty); //notifies people still in the room
                        }
                    }
                    printf("The room %d is empty\n", i + 1);
                }
            }
        }
        else
        { /*CHILD ------ ENTERING PEOPLE*/
            do
            {
                sem_wait(sem_empty); // notifies its entering people
                printf("Entering the exposition\n");
                sleep(1);
                printf("Enjoying the exposition\n");
                sem_post(sem_full); // notifies someone entered the room
            } while (--show_length);
            int peopleRemaining;
            sem_getvalue(sem_full, &peopleRemaining); // gets the number of people remaining in the showroom
            exit(peopleRemaining);                    // notifies the father how much people are left
        }
    }

    /*==============================================================*/

    /*==================== UNLINKING SEMAPHORES ====================*/

    if (sem_unlink(SEM_NAME_EMPTY) == -1)
    {
        perror("Error unlink sem_empty");
    }

    if (sem_unlink(SEM_NAME_FULL) == -1)
    {
        perror("Error unlink sem_full");
    }

    /*============================================================*/

    return 0;
}
