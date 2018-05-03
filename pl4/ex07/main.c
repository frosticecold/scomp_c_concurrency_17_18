#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sm.h"
#include <sys/wait.h>

sem_t *sem1;
sem_t *sem2;
sem_t *sem3;
#define SEM_NAME_1 "sem_1"
#define SEM_NAME_2 "sem_2"
#define SEM_NAME_3 "sem_3"
void open_sem();
int main()
{
    sem_unlink(SEM_NAME_1);
    sem_unlink(SEM_NAME_2);
    sem_unlink(SEM_NAME_3);
    /*char *MSG[] = {"Sistemas ",
                   "de ", "Computadores - ",
                   "a ", "melhor ", "disciplina! "};
                   */
    open_sem();

    const int np = 3; /* number of processes */
    int i;
    pid_t pid[3];
    for (i = 0; i < np; ++i)
    {
        if ((pid[i] = fork()) == -1)
        {
            perror("Error forking.\n");
            exit(1);
        }
        else
        {
            if (pid[i] == 0)
            {
                break;
            }
        }
    }
    if (i == np)
        i--;
    /*Process 1*/
    if (pid[0] == 0)
    {
        int r = sem_wait(sem1);
        if (r == -1)
        {
            perror("Error sem_wait(sem1)\n");
            exit(1);
        }

        //Sistemas
        printf("Sistemas\n");
        r = sem_post(sem2);
        if (r == -1)
        {
            perror("Error sem_post(sem2)");
            exit(1);
        }
        r = sem_wait(sem1);
        if (r == -1)
        {
            perror("Error sem_wait(sem1)");
            exit(1);
        }
        printf("a \n");
        r = sem_post(sem2);
        if (r == -1)
        {
            perror("Error sem_post(sem2)");
            exit(1);
        }
        exit(0);
    }
    /* Process 2*/
    if (pid[1] == 0)
    {
        int r = sem_wait(sem2);
        if (r == -1)
        {
            perror("Error sem_wait(sem2)\n");
            exit(1);
        }
        //de
        printf("de \n");

        r = sem_post(sem3);
        if (r == -1)
        {
            perror("Error sem_post(sem3)");
            exit(1);
        }
        r = sem_wait(sem2);
        if (r == -1)
        {
            perror("Error sem_wait(sem2)");
            exit(1);
        }
        printf("melhor \n");
        r = sem_post(sem3);
        if (r == -1)
        {
            perror("Error sem_post(sem3)");
            exit(1);
        }
        exit(0);
    }
    /* Process 3*/
    if (pid[2] == 0)
    {
        int r = sem_wait(sem3);
        if (r == -1)
        {
            perror("Error sem_wait(sem3)\n");
            exit(1);
        }

        //Computadores
        printf("Computadores - \n");
        r = sem_post(sem1);
        if (r == -1)
        {
            perror("Error sem_post(sem1)");
            exit(1);
        }
        r = sem_wait(sem3);
        if (r == -1)
        {
            perror("Error sem_wait(sem3)");
            exit(1);
        }
        printf("disciplina!\n");
        exit(0);
    }
    /* Father code */

    for (i = 0; i < np; ++i)
    {
        waitpid(pid[i], NULL, 0);
    }

    sem_unlink(SEM_NAME_1);
    sem_unlink(SEM_NAME_2);
    sem_unlink(SEM_NAME_3);
    return 0;
}

void open_sem()
{
    if ((sem1 = sem_open(SEM_NAME_1, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("No sem_open(sem1)");
        exit(1);
    }
    if ((sem2 = sem_open(SEM_NAME_2, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("No sem_open(sem2)");
        exit(1);
    }
    if ((sem3 = sem_open(SEM_NAME_3, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("No sem_open(sem3)");
        exit(1);
    }
}