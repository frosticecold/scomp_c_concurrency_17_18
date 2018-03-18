#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
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
    int status[NUM_PROCESSES];
    int vec1[VEC_SIZE];
    int vec2[VEC_SIZE];
    int result[VEC_SIZE];
    int pipes[NUM_PROCESSES][2];
    int i;

    numberGenerator(vec1);
    numberGenerator(vec2);

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
            break;
        }
    }

    if (i >= NUM_PROCESSES) //verificação se indice de acesso à posição do vetor de processos saiu do limite
    {
        i = 0;
    }
    if (pid[i] > 0)
    {
        int p;
        for (p = 0; p < NUM_PROCESSES; p++)//Então vamos esperar por todos os processos
        {
            if(waitpid(pid[p], &status[p], 0) == -1) {
                perror("Erro waitpid");
                return EXIT_FAILURE;
            }
        }
    
        for (p = 0; p < NUM_PROCESSES; p++)
        {
            if (WIFEXITED(status[p]))//Se acabou
            {
                int j;
                close(pipes[p][1]); // pai lê fechar pipe de escrita
                for (j = 0; j < p; j++)
                { //fechar pipes "herdados"
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
                if (read(pipes[p][0], (void *)&((result[p * LIMITE_PROCESSO])), sizeof(int) * LIMITE_PROCESSO) == -1) // leitura no pipe do respetivo processo com teste de sucesso
                {
                    perror("Erro Leitura pelo Pai");
                    return EXIT_FAILURE;
                }
                close(pipes[p][0]); // fechar o pipe de leitura
            }
        }
        for (int k = 0; k < VEC_SIZE; k++)
        { // printing result of sum
            printf("vec1[%d]+vec2[%d] = Result[%d]=%d\n",k,k,k, result[k]);
        }
    }
    if (pid[i] == 0)
    {
        int vectemp[LIMITE_PROCESSO];
        int j;
        close(pipes[i][0]); // filho escreve fechar pipe de leitura
        for (j = 0; j < i; j++)
        { //fechar pipes "herdados"
            close(pipes[j][0]);
            close(pipes[j][1]);
        }
        int limInf = i * LIMITE_PROCESSO;
        int limSup = limInf + LIMITE_PROCESSO;
        int pos = 0;
        for (j = limInf; j < limSup; j++)
        {
            vectemp[pos] = vec1[j] + vec2[j];
            ++pos;
        }
        if (write(pipes[i][1], (void *)vectemp, sizeof(int) * LIMITE_PROCESSO) == -1) // escrita no pipe com teste de sucesso
        {
            perror("Erro escrita pelo Filho");
            return EXIT_FAILURE;
        }
        close(pipes[i][1]); // fechar o pipe de escrita
        exit(vectemp[0]);
    }
    return 0;
}