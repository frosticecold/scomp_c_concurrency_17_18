#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define READ 0
#define WRITE 1


int main()
{
    int pipe1[2];
    int pipe2[2];
    pid_t pid;
    char numero[20];
    if (pipe(pipe1) == -1)
    {
        perror("Erro crar Pipe");
        return EXIT_FAILURE;
    }
    if (pipe(pipe2) == -1)
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
    {
        close(pipe1[WRITE]);
        dup2(pipe1[READ], STDIN_FILENO); // direcionar a leitura do pipe para o input de teclado
        close(pipe1[READ]);

        close(pipe2[READ]);
        dup2(pipe2[WRITE], STDOUT_FILENO); // direcionar a escrita do pipe para o output de ecra
        close(pipe2[WRITE]);

        execlp("./factorial", "./factorial", NULL);
        perror("Erro na execução de factorial");// caso o exec nao seja executado será lançado o erro
        exit(1);
    }
    if (pid > 0)
    {
        close(pipe1[READ]); // fechar pipe leitura para fazer escrita

        scanf("%s", numero);
        if (write(pipe1[WRITE], (void *)&numero, strlen(numero) + 1) == -1) // escrita no pipe1 com verificação 
        {
            perror("Erro escrever numero --- PAI ---\n");
            return EXIT_FAILURE;
        }
        close(pipe1[WRITE]); // fechar pipe apos escrita
        
        close(pipe2[WRITE]); // fechar pipe escrita para fazer leitura
        char fact;
        int n = 0;
        printf("Factorial de %s:", numero);
        while ((n = read(pipe2[READ], (void *)&fact, 1)) > 0)
        {
            printf("%c",fact);
        }
        printf("\n");
        if (n == -1) // veriricação de erros de leitura do pipe
        {
            perror("Erro ler numero --- PAI ---\n");
            return EXIT_FAILURE;
        }
        close(pipe2[READ]); // fechar pipe2 de leitura 
    }
    return 0;
}
