#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#define VEC_SIZE 1000
#define NUM_PROCESSES 5
#define LIMITE_PROCESSO 200

void numberGenerator(int *vec)
{
    time_t t; /* needed to initialize random number generator (RNG) */
    int i;
    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned)time(&t));

    /* initialize array with random numbers (rand(): stdlib.h) */
    for (i = 0; i < VEC_SIZE; i++)
    {
        vec[i] = rand() % 10;
    }
}

int main()
{
    pid_t pid[NUM_PROCESSES];
    int vec1[VEC_SIZE];
    int vec2[VEC_SIZE];
    int result[VEC_SIZE];
    int pipes[NUM_PROCESSES][2];
    int i;

    numberGenerator(vec1);
    numberGenerator(vec2);

    for (i = 0; i < 1000; i++)
    {
        printf("realResult[i]=%d\n", vec1[i] + vec2[i]);
    }
    for (i = 0; i < NUM_PROCESSES; i++)
    {
        if (pipe(pipes[i]) == -1)
        { //teste na criação do pipe
            perror("Erro no pipe");
            return EXIT_FAILURE;
        }
        pid[i] = fork(); // criação de processos
        if (pid[i] == -1)
        { // verificar sucesso de criação de processo
            perror("Erro na criação de processos");
            return EXIT_FAILURE;
        }
        if (pid[i] == 0)
        {
            close(pipes[i][0]); // filho escreve fechar pipe de leitura
            int j;
            for (j = 0; j < i; j++)
            { //fechar pipes "herdados"
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            break;
        }
    }

    if (i == NUM_PROCESSES) //verificação se indice de acesso à posição do vetor de processos saiu do limite
    {
        i = 0;
    }
    if (pid[i] > 0)
    {
        close(pipes[i][1]); // filho escreve fechar pipe de leitura
        int j;
        int vectemp[LIMITE_PROCESSO];
        int pos = i * LIMITE_PROCESSO;
        int fim = pos + LIMITE_PROCESSO;
        
        for (j = 0; j < i; j++)
        { //fechar pipes "herdados"
            close(pipes[j][0]);
            close(pipes[j][1]);
        }
        int newvec[LIMITE_PROCESSO];
        if (read(pipes[i][0],(void *)newvec, sizeof(int)) == -1) // escrita no pipe com teste de sucesso
        {
            perror("Erro Leitura pelo Pai");
            return EXIT_FAILURE;
        }
        for (j = pos; j < fim; j++)
        {
            result[j] = newvec[j];
            printf("Result[%d] = %d \n",j, result[j]);
        }

        close(pipes[i][0]); // fechar o pipe de escrita
    }
    if (pid[i] == 0)
    {
        int vectemp[LIMITE_PROCESSO];
        int pos = i * LIMITE_PROCESSO;
        int fim = pos + LIMITE_PROCESSO;
        int j;
        for (j = pos; j < fim; j++)
        {
            vectemp[j] = vec1[j] + vec2[j];
        }
        if (write(pipes[i][1],(void *)vectemp, sizeof(int)) == -1) // escrita no pipe com teste de sucesso
        {
            perror("Erro escrita pelo Filho");
            return EXIT_FAILURE;
        }
        close(pipes[i][1]); // fechar o pipe de escrita
        exit(2);
    }
    return 0;
}