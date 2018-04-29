#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#define SEM_NAME_BEER "semaphore_beer_ex9"
#define SEM_NAME_CHIPS "semaphore_chips_ex9"

void buyingBeer()
{
    sleep(1);
    //buying beer
}

void buyingChips()
{
    sleep(1);
    //buying chips
}

int main()
{
    pid_t pid;
    sem_t *sem_beer;
    sem_t *sem_chips;

    /*=========== OPEN THE SEMAPHORE FOR BEER AND CHIPS ============*/

    sem_beer = sem_open(SEM_NAME_BEER, O_CREAT | O_EXCL, 0644, 0);
    //value 0 so that the parent can control the flow of the execution
    if (sem_beer == SEM_FAILED)
    {
        perror("Can't sem_open() child\n");
        return 1;
    }

    sem_chips = sem_open(SEM_NAME_CHIPS, O_CREAT | O_EXCL, 0644, 0);
    //value 0 so that the parent can control the flow of the execution
    if (sem_chips == SEM_FAILED)
    {
        perror("Can't sem_open() child\n");
        return 1;
    }

    /*==============================================================*/

    /*=============== PROCESS OF BUYING BEER AND CHIPS ===============*/

    pid = fork();
    if (pid == -1)
    {
        perror("Error in fork\n");
        return 1;
    }
    if (pid == 0)
    { //buy chips

        if (sem_post(sem_chips) == -1)
        {
            perror("Error sem_post chips\n");
            exit(1);
        }
        printf("Buying chips\n");
        buyingChips();
        if (sem_wait(sem_beer) == -1)
        {
            perror("Error sem_post beer\n");
            exit(1);
        }
        printf("Eating Child\n");
        exit(0);
    }
    else
    {
        if (sem_post(sem_beer) == -1)
        {
            perror("Error sem_post beer\n");
            exit(1);
        }
        printf("Buying beer\n");
        buyingBeer();
        if (sem_wait(sem_chips) == -1)
        {
            perror("Error sem_post beer\n");
            exit(1);
        }

        printf("Eating Parent\n");
    }

    /*================================================================*/

    /*=========== UNLINK THE SEMAPHORE FOR BEER AND CHIPS ============*/
    if (sem_unlink(SEM_NAME_BEER) == -1)
    {
        perror("Error unlink beer");
        return 1;
    }

    if (sem_unlink(SEM_NAME_CHIPS) == -1)
    {
        perror("Error unlink chips");
        return 1;
    }
    /*================================================================*/
    return 0;
}