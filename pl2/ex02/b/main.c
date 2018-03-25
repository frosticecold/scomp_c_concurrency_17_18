#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 64

typedef struct
{
    char buf[BUF_SIZE];
    int numbuf;
} my_struct;
int main()
{

    my_struct st;
    int fd[2];          //PIPE
    if (pipe(fd) == -1) // Criar um pipe, se der erro escrever erro
    {
        perror("Erro ao criar o pipe.");
        return EXIT_FAILURE;
    }

    pid_t pid = fork(); // Criar um processo filho

    if (pid == -1) //Se não conseguir criar um processo filho, dar erro
    {
        return EXIT_FAILURE;
    }
    else
    {
        if (pid > 0) //Se for o processo pai
        {
            close(fd[0]); //Fechar a leitura do pai

            printf("[PAI] Vamos ler a string:");
            fgets(st.buf, BUF_SIZE, stdin); //Ler a string e guardar na estrutura
            st.buf[strlen(st.buf)] = '\0';  //Remover o caracter de newline

            printf("\n[PAI] Vamos ler o inteiro: ");
            scanf("%d", &st.numbuf); //Ler um inteiro

            if (write(fd[1], (void *)&st, sizeof(st)) == -1)
            {                                            //Escrever para a pipe
                perror("Erro ao escrever a estrutura."); //Se der erro, escrever o erro
                return EXIT_FAILURE;
            }
            close(fd[1]); //Fechar a extremidade de escrita do pipe
            wait(NULL);   // Esperar pelo filho
        }
        else
        {//Se for o filho
            close(fd[1]); //Fechar a escrita do filho
            if (read(fd[0], (void *)&st, sizeof(st)) == -1)
            {
                perror("Erro ao ler a estrutura.");
                exit(1);
            }                                                                     //Ler a estrutura
            printf("\n[FILHO]String:%s\n[FILHO]Inteiro:%d\n", st.buf, st.numbuf); //Escrever para a consola a estrutura
            close(fd[0]);
            exit(0);
        }
    }
    return 0;
}