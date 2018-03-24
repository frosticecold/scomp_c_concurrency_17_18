#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "user.h"

#define READ 0
#define WRITE 1

#define CODE_SUCCESS 0
#define CODE_ERROR 1
#define CODE_NOT_EXISTS 2
#define BUFFER 15

int main()
{
    int fd[2];
    pid_t pid;
    User user;
    int status;

    if (pipe(fd) == -1)
    {
        perror("Erro crar Pipe");
        return EXIT_FAILURE;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("Erro criar processo");
        return EXIT_FAILURE;
    }
    if (pid == 0)
    { //processo filho

        close(fd[WRITE]);
        dup2(fd[READ], STDIN_FILENO); // direcionar a leitura do pipe para o input de teclado
        close(fd[READ]);

        execlp("./validate", "./validate", NULL);
        perror("Exec falhou");
        exit(3);
    }
    if (pid > 0)
    {                    //processo pai
        close(fd[READ]); // fechar pipe leitura para fazer escrita
        printf("Insert username: ");
        scanf("%s", user.username);
        printf("Insert password: ");
        scanf("%s", user.password);

        if (write(fd[WRITE], (void *)&user, sizeof(user)) == -1) // escrita no pipe1 com verificação
        {
            perror("Erro escrever numero --- PAI ---\n");
            return EXIT_FAILURE;
        }
        close(fd[WRITE]); // fechar pipe apos escrita

        wait(&status);
        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) == CODE_SUCCESS)
            {
                printf("Success\n");
            }
            if (WEXITSTATUS(status) == CODE_ERROR)
            {
                printf("Password invalid\n");
            }
            if (WEXITSTATUS(status) == CODE_NOT_EXISTS)
            {
                printf("Does not exist\n");
            }
        }
    }

    return 0;
}