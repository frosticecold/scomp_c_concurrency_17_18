#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFFER 64

int main()
{
    pid_t pid;
    char buf[BUFFER];    
    int status;
    while(scanf("%s",buf) != EOF){ //enqunato a condição e verdadeira fazer scanf para executar comand execlp
        pid = fork();
        if (pid == -1){
            perror("Erro criar filho");
            return -1;
        }
        if(pid == 0){   // se for filho
            execlp("ls","ls",buf,NULL);//executar comando execlp para executar um ls com o valor de scanf f
            exit(-1); // signed number -1 = 255
        }
        if(pid > 0){       
            wait(&status); // espera que se execute sempre o filho
            if(WIFEXITED(status)){ // se filho acabou fazer verificações adequadas
                if(WEXITSTATUS(status) == 255){ // se houve erro na execução de execlp
                    printf("Falhou erro\n"); 
                    return EXIT_FAILURE;
                }
                if(WEXITSTATUS(status) > 0 ){ //se nao existe ficheiro 
                    printf("Nao existe ficheiro\n");
                    return EXIT_FAILURE;
                }
                if (WEXITSTATUS(status) == 0){ // se encontrou corretamente o ficheiro introduzido pelo utilizador
                    printf("Encontrou ficheiro!\n");
                }
            }
        }
    }
    return 0;
}