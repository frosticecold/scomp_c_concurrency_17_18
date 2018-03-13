#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 256
int main()
{
    char buf[BUF_SIZE]; //buffer
    int fd[2];          //pipe
    if (pipe(fd) == -1) //Se falhar a criação do pipe
    {
        return EXIT_FAILURE;
    }

    pid_t pid = fork(); //criar um filho
    if (pid == -1)      //Se falhar a criação do filho
    {
        return EXIT_FAILURE;
    }
    else
    {
        if (pid == 0) //Se for o processo filho
        {
            close(fd[1]); //Não queremos escrever
            int n;
            while ((n = read(fd[0], (void *)buf, BUF_SIZE)) > 0) //Enquanto houver coisas para ler
            {
                printf("%s", buf); //Escrever para a consola
            }
            if (n == -1)
            {
                perror("Erro ao ler.");
                return EXIT_FAILURE;
            }
            close(fd[0]); //Fechar o pipe
            exit(0);      //Exit com sucesso
        }
        else
        {
            close(fd[0]); //Não queremos ler
            FILE *txtfile;
            txtfile = fopen("./text.txt", "r+");          //Abrir um ficheiro
            while (fgets(buf, BUF_SIZE, txtfile) != NULL) //Enquanto houver coisas para ler
            {
                if(write(fd[1], (void *)buf, strlen(buf)) == -1){ //Mandar para o pipe
                    perror("Erro ao escrever no pipe.");
                    return EXIT_FAILURE;
                }
            }
            int status;
            close(fd[1]);  //Fechar a escrita
            wait(&status); //Esperar pelo filho
        }
    }

    return 0;
}