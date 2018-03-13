#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 64
int main()
{
    int numbuf = 0;     //Buffer para leitura de um numero
    char buf[BUF_SIZE]; //Buffer para leitura de uma string
    int fd[2];          //PIPE
    if (pipe(fd) == -1)
    { //Vamos criar o pipe
        //Se falhar então erro
        perror("Erro ao criar o pipe.");
        return EXIT_FAILURE;
    }
    pid_t pid = fork(); //Criar um processo filho

    if (pid == -1) //Se falhou ao criar o processo filho
    {
        return EXIT_FAILURE;
    }
    else
    {
        if (pid > 0) // Se for o processo pai
        {
            close(fd[0]); //Fechar a leitura do pai

            printf("[PAI] Vamos ler a string:");
            fgets(buf, BUF_SIZE, stdin);

            buf[strlen(buf)] = '\0';

            printf("\n[PAI] Vamos ler o inteiro: ");
            scanf("%d", &numbuf);

            if (write(fd[1], (void *)&buf, BUF_SIZE) == -1) //Escrever a string
            {
                perror("Erro ao escrever a string.");
                return EXIT_FAILURE;
            }

            if (write(fd[1], (void *)&numbuf, sizeof(int))) //Escrever o inteiro
            {
                perror("Erro ao escrever o inteiro.");
                return EXIT_FAILURE;
            }

            close(fd[1]); // Fechar a extremidade de escrita do pipe.
            wait(NULL);   //Esperar pelo filho
        }
        else
        {//Se for o filho
            close(fd[1]);                                  //Fechar a escrita do filho
            if (read(fd[0], (void *)&buf, BUF_SIZE) == -1) //Ler a string
            {
                perror("Erro ao ler a string.");
                return EXIT_FAILURE;
            }
            if (read(fd[0], (void *)&numbuf, sizeof(int)) == -1)
            { //Ler o inteiro
                perror("Erro ao ler o inteiro.");
                return EXIT_FAILURE;
            }
            printf("\n[FILHO]String:%s\n[FILHO]Inteiro:%d\n", buf, numbuf);
            close(fd[0]);
            exit(0);
        }
    }
    return 0;
}