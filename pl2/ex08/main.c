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
    estrutura e_p;
    strncpy(e_p.msg, "Win", 4);
    e_p.round_num = 0;

    int fd[2]; //Inicializar o vetor para o pipe
    if (pipe(fd) == -1)
    { //Criar o pipe, se der erro
        perror("Erro ao criar o pipe.");
        return EXIT_FAILURE;
    }

    int p;
    pid_t pid[NUM_PROC];

    for (p = 0; p < NUM_PROC; ++p) //Vamos criar 10 processos
    {
        pid[p] = fork();
        if (pid[p] == -1) //Se der erro ao criar o processo
        {
            perror("Erro a criar filho");
            return EXIT_FAILURE;
        }
        else
        {
            if (pid[p] == 0) //Se for o filho quebrar o ciclo de criar processos
            {
                break;
            }
        }
    }
    if(p>= NUM_PROC){
        p=0;
    }
    //Se for o filho, ler a estrutura,imprimir e fazer exit com o número da ronda
    if (pid[p] == 0)
    {
        close(fd[1]);//Fechar a extremidade de escrita do pipe
        estrutura nova;
        if(read(fd[0], (void *)&nova, sizeof(nova))==-1){
            perror("Erro ao ler do pipe.");
            return EXIT_FAILURE;
        }
        close(fd[0]);
        //printf("Round number:%d Msg:%s", nova.round_num, nova.msg);
        exit(nova.round_num);
    }
    else
    {
        if (pid[p] > 0)
        {                 //Se for o pai
            close(fd[0]); //Fechar a extremidade de leitura do pipe
            int i;
            
            for (i = 1; i <= NUM_PROC; ++i)
            { //Preencher a estrutura, escrever para o pipe e adormecer 2s
                strncpy(e_p.msg, "Win", MSG_SIZE);
                e_p.round_num = i;
                if (write(fd[1], (void *)&e_p, sizeof(e_p)) == -1)
                { //Escrever para o pipe, verificar se deu erro
                    perror("Erro ao escrever para o pipe.");
                    return EXIT_FAILURE;
                }
                printf("\nSegundos :%d",i*2);
                sleep(2);//Esperar 2 segundos
            }
            for (i = 0; i < NUM_PROC; ++i)//Escrever o pid e número de ronda para cada processo.
            {   
                if(i==0){
                    printf("\n");
                }
                int status;
                wait(&status);//Esperar por um processo
                if (WIFEXITED(status))//Se saiu
                {
                    printf("PID:%d Round number:%d\n", pid[i], WEXITSTATUS(status));
                }else{
                    perror("Erro ao terminar um processo.");
                    return EXIT_FAILURE;
                }
            }
            close(fd[1]);
        }
    }
    return 0;
}