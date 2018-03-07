#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 64
int main()
{
    int numbuf=0;
    char buf[BUF_SIZE];
    int fd[2]; //PIPE
    if(pipe(fd) == -1){
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
            close(fd[0]);//Fechar a leitura do pai
            printf("[PAI] Vamos ler a string:");
            fgets(buf,BUF_SIZE,stdin);
            buf[strlen(buf)] = '\0';
            printf("\n[PAI] Vamos ler o inteiro: ");
            scanf("%d",&numbuf);
            //printf("\n[PAI]String:%s\n[PAI]Inteiro:%d\n",buf,numbuf);
            write(fd[1],(void*)&buf,BUF_SIZE);
            write(fd[1],(void*)&numbuf,sizeof(int));
            close(fd[1]);
            wait(NULL);
        }
        else
        {
            close(fd[1]);//Fechar a escrita do filho
            read(fd[0],(void*)&buf,BUF_SIZE);//Ler a string
            read(fd[0],(void*)&numbuf,sizeof(int));
            printf("\n[FILHO]String:%s\n[FILHO]Inteiro:%d\n",buf,numbuf);
            close(fd[0]);

        }
    }
    return 0;
}