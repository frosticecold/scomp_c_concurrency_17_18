#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
/*
==================================================================
    Constants
==================================================================
*/
#define MAX_LINES 100
#define BUF_SIZE 16
int main()
{
    pid_t pid;
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Erro ao criar o pipe");
        return EXIT_FAILURE;
    }
    if ((pid = fork()) == -1)
    {
        perror("Erro ao criar processo filho");
        return EXIT_FAILURE;
    }
    /*
    ==================================================================
        Processo filho
    ==================================================================
    */
    if (pid == 0)
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        execlp("more", "more", NULL);
        perror("Error executing");
        exit(1);
    }
    /*
    ==================================================================
        Processo Pai
    ==================================================================
    */
    else
    {
        //Fechar a extremidade de leitura do PAI
        close(fd[0]);
        //Send message
        int i;
        for (i = 1; i <= MAX_LINES; i++)
        {
            char str[BUF_SIZE];
            sprintf(str, "Line %d\n", i);
            if (write(fd[1], (void *)str, BUF_SIZE) == -1)
            {
                perror("Erro ao escrever no pipe");
                return EXIT_FAILURE;
            }
        }
        close(fd[1]);
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}