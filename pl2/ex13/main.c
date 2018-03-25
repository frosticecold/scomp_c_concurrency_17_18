#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main()
{
    pid_t pid;
    int fd[2];
    char output;
    if (pipe(fd) == -1)
    { //teste na criação do fd
        perror("Erro no pipe");
        return EXIT_FAILURE;
    }
    pid = fork();
    if (pid == -1) // verificação de criação de processo
    {
        perror("Erro na criação do processo");
    }
    if (pid == 0)
    {                                   //processo filho
        close(fd[READ]);                //fechar extremidade de leitura
        dup2(fd[WRITE], STDOUT_FILENO); // direcionar output de WRITE para STDOUT
        close(fd[WRITE]);               // fechar apos direcionar output

        execlp("sort", "sort", "fx.txt", NULL); // executar comando sort no ficheiro "fx.txt"
        perror("Erro ao executar comando sort");// erro no execlp esta linha sera executada
        exit(0);
    }
    close(fd[WRITE]); //fechar pipe de escrita para ler
    int n = 0;
    while ((n = read(fd[READ], (void *)&output, 1)) > 0) // enquanto houver alguma coisa para ler do pipe 
    {
        if (n == -1)
        {
            perror("erro na leitura Pai");
            return EXIT_FAILURE;
        }
        printf("%c", output); //impressao do conteudo do pipe
    }
    close(fd[READ]); // fechar pipe apos a leitura completa do pipe

    return 0;
}