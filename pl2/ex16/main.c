#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void exec_ls(int pipe1[2]);
void exec_sort(int pipe1[2], int pipe2[2]);
void exec_wc(int pipe2[2]);

//#define NUM_ARGS 3
int main(int argc, char **argv)
{
    /*     if (argc < NUM_ARGS)
    {
        printf("Número de argumentos é inválido\n");
        return 0;
    } */

    int pipe1[2];
    int pipe2[2];

    pid_t pid_ls;
    pid_t pid_sort;
    pid_t pid_wc;

    if (pipe(pipe1) == -1)
    {
        perror("Erro ao criar o pipe");
        return EXIT_FAILURE;
    }

    if ((pid_ls = fork()) == -1)
    {
        perror("Erro ao fazer fork");
        exit(1);
    }
    /*
    ==================================================================
       Função (ls -la)
    ==================================================================
    */
    else if (pid_ls == 0)
    {
        exec_ls(pipe1);
    }
    /*
    ==================================================================
        Criar segunda pipe
    ==================================================================
    */
    if (pipe(pipe2) == -1)
    {
        perror("Erro ao criar o pipe2");
        exit(1);
    }
    if ((pid_sort = fork()) == -1)
    {
        perror("Erro ao criar o processo");
        return EXIT_FAILURE;
    }
    /*
    ==================================================================
        Função sort
    ==================================================================
    */
    else if (pid_sort == 0)
    {
        exec_sort(pipe1, pipe2);
    }
    else
    {
        close(pipe1[0]);
        close(pipe1[1]);
        if ((pid_wc = fork()) == -1)
        {
            perror("Erro ao criar o processo");
            return EXIT_FAILURE;
        }
        if (pid_wc == 0)
        {
            exec_wc(pipe2);
        }
        else
        {
            //waitpid(pid_wc, NULL, 0);
            close(pipe2[0]);
            close(pipe2[1]);
        }
    }
    return 0;
}

/*
    ==================================================================
        Processo filho (ls -la)
    ==================================================================
    */
void exec_ls(int pipe1[2])
{
    dup2(pipe1[1], STDOUT_FILENO);
    close(pipe1[0]);
    close(pipe1[1]);
    execlp("ls", "ls", "-la", NULL);
    perror("Erro ao executar ls -la");
    exit(0);
}

void exec_sort(int pipe1[2], int pipe2[2])
{
    dup2(pipe1[0], STDIN_FILENO);
    dup2(pipe2[1], STDOUT_FILENO);
    //Fechar primeiro pipe
    close(pipe1[0]);
    close(pipe1[1]);
    //Fechar segundo pipe
    close(pipe2[0]);
    close(pipe2[1]);
    execlp("sort", "sort", NULL);
    perror("Erro ao executar sort");
    exit(1);
}

void exec_wc(int pipe2[2])
{
    dup2(pipe2[0], STDIN_FILENO);
    //dup2(pipe2[1], STDOUT_FILENO);
    
    close(pipe2[0]);
    close(pipe2[1]);
    execlp("wc", "wc", "-l", NULL);
    perror("Erro ao executar wc");
    exit(1);
}