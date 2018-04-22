#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#define ARRAY_SIZE 1000
#define NUM_PROCESSES 10
#define LIMIT_BY_PROCESS ARRAY_SIZE / NUM_PROCESSES
#define READ 0
#define WRITE 1

void populatearray(int *vec)
{
    int i = 0;
    srand(time(NULL));
    for (; i < ARRAY_SIZE; ++i)
    {
        vec[i] = rand() % ARRAY_SIZE;
    }
}

int main()
{
    int vec[ARRAY_SIZE]; // array that gets all the 1000 ELEMENTS
    int biggestTenElements[NUM_PROCESSES]; // array that saves the biggest 10 elements for each processed process
    int pipes[NUM_PROCESSES][2]; //pipes to each process write/read from
    pid_t pid[NUM_PROCESSES]; //pids to identify processes
    int status[NUM_PROCESSES]; // array to verify the completation of each process

    /*POPULATE ARRAY*/

    populatearray(vec);

    /*POPULATE ARRAY*/

    int i = 0;

    for (i = 0; i < NUM_PROCESSES + 1; i++)
    { // abrir pipes para parilhar por todos os processos
        if (pipe(pipes[i]) == -1)
        { //teste na criação do pipe
            perror("Erro no pipe");
            return EXIT_FAILURE;
        }
    }

    for (i = 0; i < NUM_PROCESSES; i++)
    {
        pid[i] = fork(); // criação de processo
        if (pid[i] == -1)
        { // verificar sucesso de criação de processo
            perror("Erro na criação de processos");
            return EXIT_FAILURE;
        }

        if (pid[i] == 0)
        {
            int j;
            close(pipes[i][0]); // filho escreve fechar pipe de leitura
            for (j = 0; j < i; j++)
            { //fechar pipes "herdados"
                close(pipes[j][READ]);
                close(pipes[j][WRITE]);
            }

            int minInterval = i * LIMIT_BY_PROCESS; // begining of the partial vector
            int maxInterval = minInterval + LIMIT_BY_PROCESS; // ending of the partial vector
            int tempMinValue = -1; //temp variable starting as -1 to be passed to the pipe
            for (; minInterval < maxInterval; minInterval++)
            {
                if (vec[minInterval] > tempMinValue) // comparation to see wich element is bigger 
                {// if true update variable
                    tempMinValue = vec[minInterval];
                }
            }
            /*AFTER PROCESS THE BIGGEST ELEMENT OF THE PARTIAL VEC*/
            if (write(pipes[i][WRITE], (void *)&tempMinValue, sizeof(int)) == -1) // escrita no pipe com teste de sucesso
            {
                perror("Erro escrita pelo Filho");
                return EXIT_FAILURE;
            }
            exit(1);
        }
    }

    for (i = 0; i < NUM_PROCESSES; i++) //Então vamos esperar por todos os processos
    {
        if (waitpid(pid[i], &status[i], 0) == -1)
        {
            perror("Erro waitpid");
            return EXIT_FAILURE;
        }
    }
    for (i = 0; i < NUM_PROCESSES; i++)
    {
        if (WIFEXITED(status[i])) //Se acabou
        {
            int j;
            close(pipes[i][WRITE]); // pai lê fechar pipe de escrita
            for (j = 0; j < i; j++)
            { //fechar pipes "herdados"
                close(pipes[j][READ]);
                close(pipes[j][WRITE]);
            }
            if (read(pipes[i][READ], (void *)&((biggestTenElements[i])), sizeof(int)) == -1) // leitura no pipe do respetivo processo com teste de sucesso
            {
                perror("Erro Leitura pelo Pai");
                return EXIT_FAILURE;
            }
            close(pipes[i][READ]); // fechar o pipe de leitura
        }
    }
    int biggest=-1;
    for(i=0;i<NUM_PROCESSES;i++){
        printf("process %d : %d\n",i,biggestTenElements[i]);
        if(biggestTenElements[i]>biggest){ // compare to check the biggest value of the ten integers
            biggest=biggestTenElements[i];
        }
    }
    printf("Biggest integer: %d\n", biggest);

}