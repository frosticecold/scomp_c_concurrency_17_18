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
    while(scanf("%s",buf) != EOF){ 
        pid = fork();
        if (pid == -1){
            perror("Erro criar filho");
            return -1;
        }
        if(pid == 0){   // se for filho
            execlp("ls","ls",buf,NULL);
            exit(-1);
        }
        if(pid > 0){       
            wait(&status);
            if(WIFEXITED(status)){
                if(WEXITSTATUS(status) == 255){
                    printf("Falhou erro");
                }
                if(WEXITSTATUS(status) > 0 ){
                    printf("Nao existe ficheiro");
                }
                if (WEXITSTATUS(status) == 0){
                    printf("Encontrou ficheiro!");
                }
                return WEXITSTATUS(status);
            }
        }
    }
    return 0;
}