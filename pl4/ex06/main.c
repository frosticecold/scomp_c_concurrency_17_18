#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#define NUM_PROCESSES 7
#define BUFFER 256

int main()
{
    pid_t pid[NUM_PROCESSES];
    sem_t *sem_child[NUM_PROCESSES];
    sem_t *sem_parent[NUM_PROCESSES];

    int i = 0;
    char name_sem_child[NUM_PROCESSES][BUFFER];
    char name_sem_parent[NUM_PROCESSES][BUFFER];
    for (i = 0; i < NUM_PROCESSES; i++)
    {
        sprintf(name_sem_child[i], "sem_child%d", i);
        sprintf(name_sem_parent[i], "sem_parent%d", i);
    }

    /*=========== OPEN THE SEMAPHORE FOR THE CHILD AND PARENT ============*/

    for (i = 0; i < NUM_PROCESSES; i++)
    {
        sem_child[i] = sem_open(name_sem_child[i], O_CREAT | O_EXCL, 0644, 1);
        //value 0 so that the parent can control the flow of the execution
        if (sem_child[i] == SEM_FAILED)
        {
            perror("Can't sem_open() child");
            return 1;
        }

        sem_parent[i] = sem_open(name_sem_parent[i], O_CREAT | O_EXCL, 0644, 0);
        //value 0 so that the parent can lock until the child finishes
        if (sem_parent[i] == SEM_FAILED)
        {
            perror("Can't sem_open() parent");
            return 1;
        }
    }

    /*===================================================================*/

    for (i = 0; i < NUM_PROCESSES; i++)
    {
        pid[i] = fork();  //new process creation
        if (pid[i] == -1) //fork validation
        {
            perror("Error on fork");
            return 1;
        }
        if (pid[i] == 0)
        {
            if (sem_wait(sem_child[i]) == -1) // notifies begining of critical code
            {
                perror("Error sem wait");
                exit(1);
            }
            printf("I'm the child\n");
            if (sem_post(sem_child[i]) == -1) // notifies the process has finished the critical code
            {
                perror("Error sem wait");
                exit(1);
            }
            if (sem_post(sem_parent[i]) == -1) //allows the parent to proceceed with his critical code
            {
                perror("Error sem post 2 child");
                exit(1);
            }

            exit(0);
        }
        else
        {
            if (sem_wait(sem_parent[i]) == -1) //locks until the parent until child finishes
            {
                perror("Error sem wait parent");
                exit(1);
            }
            printf("I'm the parent\n");
            if (sem_post(sem_parent[i]) == -1) //notifies that finished his critical code
            {
                perror("Error sem post parent");
                exit(1);
            }
        }
    }

    for (i = 0; i < NUM_PROCESSES; i++)
    {
        if (sem_unlink(name_sem_child[i]) == -1)
        {
            perror("Error unlink child after");
            return 1;
        }
        if (sem_unlink(name_sem_parent[i]) == -1)
        {
            perror("Error unlink parent");
            return 1;
        }
    }

   
    return 0;
}
