#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
    int i, status, error, fd[2];
    pid_t pid;
    char message[100], bufread[100];

    pipe(fd);       //Criar um pipe
    dup2(fd[0], 0); //Redirecionar leitura do pipe para stdin
    dup2(fd[1], 1); //Redirecionar a escrita para stdout
    close(fd[0]);   //Fechar as extremidades
    close(fd[1]);

    //Para 4 Processos
    for (i = 1; i < 5; i++)
    {
        pipe(fd);                   //Criar um pipe
        pid = fork();               //Criar um processo
        if (pid > 0)                //Se for o pai
            error = dup2(fd[1], 1); //Redirecionar a extremidade de escrita para stdout
        else
            error = dup2(fd[0], 0); //Senão redirecionar a extremidade leitura stdin

        close(fd[0]); //Fechar o pipe
        close(fd[1]); //Fechar o pipe

        if (pid) //Se for o pai parar o ciclo
            break;
        //Se for o filho, continuar a criar processos
    }

    sprintf(message, "This is process %d with PID %d and its %d\n", i, (int)getpid(),
            (int)getppid());
    write(1, message, strlen(message) + 1);
    read(0, bufread, strlen(message));
    wait(&status);
    fprintf(stderr, "\nCurrent process = %d, data =%s", (int)getpid(), bufread);
    exit(0);
}