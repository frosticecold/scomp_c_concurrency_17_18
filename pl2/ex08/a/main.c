#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define NUM_PROC 10
#define MSG_SIZE 20
#define ROUND_MAX 10
typedef struct
{
    char msg[MSG_SIZE];
    int round_num;
} estrutura;
int main()
{
    //Inicializar a estrutura
    estrutura e1;
    strncpy(e1.msg, "Win", 4);
    e1.round_num = 0;

    int fd[2];//Inicializar o vetor para o pipe
    if(pipe(fd)==-1){//Se 
        perror("Erro ao criar o pipe.");
        return EXIT_FAILURE;
    }
    int p;
    pid_t pid[NUM_PROC];

    for (p = 0; p < NUM_PROC; ++p)//Vamos criar 10 processos
    {
        pid[p] = fork();
        if (pid[p] == -1)
        {
            perror("Erro a criar filho");
            return EXIT_FAILURE;
        }
        else
        {
            if (pid[p] == 0)//Se for o filho quebrar o ciclopid
            {
                break;
            }
        }
    }
    if(pid[p] == 0){
        close(fd[1]);
            estrutura nova;
            read(fd[0],(void*)&nova,sizeof(nova));
            printf("Round number:%d Msg:%s",nova.round_num,nova.msg);
            exit(nova.round_num);
    }else{
        if(pid[p] > 0){
            int i;
            for(i=0;i< NUM_PROC;++i){
                int status;
                waitpid(pid[i],&status,0);
                if(WIFEXITED(status)){
                    printf("PID:%d Round number:%d",pid[i],WEXITSTATUS(status));
                }
            }
        }
    }
    return 0;
}