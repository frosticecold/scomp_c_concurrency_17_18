#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

void main()
{
    int status_p1, status_p2;
    pid_t pid1 = fork();
    pid_t pid2;
    if (pid1 == -1)
    {
        printf("ERRO");
        exit(-1);
    }
    if (pid1 == 0)
    { //Filho
        sleep(1);
        exit(1);
    }
    if (pid1 > 0)
    {
        pid2 = fork();
        if (pid2 == -1)
        {
            printf("ERRO");
            exit(-1);
        }
        if (pid2 == 0)
        { //Filho
            sleep(2);
            exit(2);
        }
    }
    if (pid1 > 0 && pid2 > 0)
    {
        waitpid(pid1, &status_p1, 0);
        if (WIFEXITED(status_p1))
        {
            printf("Pai: %d retournou o valor %d\n", pid1, WEXITSTATUS(status_p1));
        }

        waitpid(pid2, &status_p2, 0);
        if (WIFEXITED(status_p2))
        {
            printf("Pai: %d retournou o valor %d\n", pid2, WEXITSTATUS(status_p2));
        }
    }
}