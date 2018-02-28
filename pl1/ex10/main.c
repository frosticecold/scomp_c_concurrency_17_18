#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define ARRAY_SIZE 2000
#define NUM_PROC 10
#define LIM_SUP 200
#define MAX_NUMBER 100

int main()
{
    int vec[ARRAY_SIZE];    //Vector
    int i;                  //i
    pid_t vecpid[NUM_PROC]; //Vector of processes
    int n;                  //number to find;

    srand((unsigned)time(NULL));

    for (i = 0; i < ARRAY_SIZE; ++i) //Inicializar vetor
    {
        vec[i] = rand() % MAX_NUMBER;
    }

    n = rand() % MAX_NUMBER; //Numero a procurar entre 0 e MAX_NUMBER

    for (i = 0; i < NUM_PROC; ++i) //Criar 10 processos
    {
        vecpid[i] = fork();
        if (vecpid[i] == -1)
        {
            return EXIT_FAILURE;
        }
        if (vecpid[i] == 0)
        {
            break;
        }
    }

    if (i < 10)
    {
        if (vecpid[i] == 0)
        {
            //Procurar primeira ocorrencia de um numero n
            int indice;
            int fim;
            (i > 0) ? ((indice = i * LIM_SUP, fim = (i * LIM_SUP) + LIM_SUP)) : (indice = 0, fim = LIM_SUP);
            for (; indice < fim; ++indice)
            {
                if (vec[indice] == n)
                {
                    exit(indice - (LIM_SUP * i));
                }
            }
            exit(255);
        }
    }
    for (i = 0; i < NUM_PROC; ++i)
    {
        if (vecpid[i] > 0)
        { // Se for o pai
            int p;
            int status[NUM_PROC];
            for (p = 0; p < NUM_PROC; ++p)
            {
                waitpid(vecpid[p], &status[p], 0);
            }
            for (i = 0; i < NUM_PROC; ++i)
            {
                if (WIFEXITED(status[i]))
                {
                    int indice = WEXITSTATUS(status[i]);
                    if (indice != 255)
                    {
                        int validIndex = indice + (LIM_SUP * i);
                        printf("Valid index:%d\n", validIndex);
                    }
                    else
                    {
                        printf("Invalid index pid: %d\n", vecpid[i]);
                    }
                }
            }
        }
    }
    return 0;
}