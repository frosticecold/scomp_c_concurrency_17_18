#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#define SEM_NAME_CHILD "semaphore_child_ex5"
#define SEM_NAME_PARENT "semaphore_parent_ex5"

int main()
{
    pid_t pid;
    sem_t *sem_child;
    sem_t *sem_parent;

    /*=========== OPEN THE SEMAPHORE FOR THE CHILD ============*/

    sem_child = sem_open(SEM_NAME_CHILD, O_CREAT | O_EXCL, 0644, 0);
    //value 0 so that the parent can control the flow of the execution
    if (sem_child == SEM_FAILED)
    {
        perror("Can't sem_open() child");
        return 1;
    }

    /*=========================================================*/

    /*=========== OPEN THE SEMAPHORE FOR THE PARENT ===========*/

    sem_parent = sem_open(SEM_NAME_PARENT, O_CREAT | O_EXCL, 0644, 0);
    //value 0 so that the parent can lock until the child finishes
    if (sem_parent == SEM_FAILED)
    {
        perror("Can't sem_open() parent");
        return 1;
    }

    /*========================================================*/

    pid = fork(); //new process creation

    if (pid == -1) //fork validation
    {
        perror("Error on fork");
        return 1;
    }

    if (pid == 0)
    {
        if (sem_wait(sem_child) == -1) // aplies a wait to the sem_child, and locks until it reachs the parent
        {
            perror("Error sem wait");
            exit(1);
        }
        printf("I'm the child\n");
        if (sem_post(sem_child) == -1) // notifies the process has finished the critical code
        {
            perror("Error sem wait");
            exit(1);
        }
        if (sem_post(sem_parent) == -1) //allows the parent to proceceed with his critical code
        {
            perror("Error sem post 2 child");
            exit(1);
        }
    }
    else
    {
        if (sem_post(sem_child) == -1) //gives permission to the child process to run
        {
            perror("Error sem child wait PARENT ");
            exit(1);
        }
        if (sem_wait(sem_parent) == -1) //locks until the child says it finished his code
        {
            perror("Error sem wait parent");
            exit(1);
        }
        printf("I'm the parent\n");
        if (sem_post(sem_parent) == -1) //notifies that finished his critical code
        {
            perror("Error sem post parent");
            exit(1);
        }

        /*=========== UNLINKING SEMAPHORES CHILD AND PARENT ===========*/

        if (sem_unlink(SEM_NAME_CHILD) == -1)
        {
            perror("Error unlink child");
            return 1;
        }
        if (sem_unlink(SEM_NAME_PARENT) == -1)
        {
            perror("Error unlink parent");
            return 1;
        }

        /*============================================================*/
    }

    return 0;
}
