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
    int fd[2]; //PIPE
    if (pipe(fd) == -1)
    {
        perror("Erro ao criar o pipe.");
        return EXIT_FAILURE;
    }
    pid_t pid = fork();

    if (pid == -1)
    {
        return EXIT_FAILURE;
    }
    else
    {
        if (pid > 0)
        {
            close(fd[0]); //Fechar a leitura do pai
            printf("[PAI] Vamos ler a string:");
            fgets(st.buf, BUF_SIZE, stdin);
            st.buf[strlen(st.buf)] = '\0';
            printf("\n[PAI] Vamos ler o inteiro: ");
            scanf("%d", &st.numbuf);
            //printf("\n[PAI]String:%s\n[PAI]Inteiro:%d\n",buf,numbuf);
            write(fd[1], (void *)&st, sizeof(st));
            close(fd[1]);
            wait(NULL);
        }
        else
        {
            close(fd[1]);                        //Fechar a escrita do filho
            read(fd[0], (void *)&st, sizeof(st)); //Ler a string
            printf("\n[FILHO]String:%s\n[FILHO]Inteiro:%d\n", st.buf, st.numbuf);
            close(fd[0]);
        }
    }
    return 0;
}