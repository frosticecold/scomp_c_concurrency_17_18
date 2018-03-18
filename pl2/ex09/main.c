#include "product.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define VEC_SIZE 50000
#define NUM_PROCESSOS 10
#define LIMITE_PROCESSO 5000
#define CRITERIA 20

void populateProducts(Product *product)
{
    int i = 0;
    int dummyValue = 21;
    for (; i < VEC_SIZE; i++)
    {
        product->customer_code = i;
        product->product_code = dummyValue;
        product->quantity = dummyValue;
        dummyValue++;
        ++product;
    }
}

int main()
{
    Product sales[VEC_SIZE];
    populateProducts(sales);
    int products[VEC_SIZE];

    pid_t pid[NUM_PROCESSOS];
    int status[NUM_PROCESSOS];
    int pipes[NUM_PROCESSOS][2];
    int i, j;
    int totalSales = 0;
    for (i = 0; i < NUM_PROCESSOS; i++)
    {
        if (pipe(pipes[i]) == -1)
        { //teste na criação do pipe
            perror("Erro no pipe");
            return EXIT_FAILURE;
        }
        pid[i] = fork(); // criação de processo
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
    if (i >= NUM_PROCESSOS) //verificação se indice de acesso à posição do vetor de processos saiu do limite
    {
        i = 0;
    }
    if (pid[i] == 0)
    {                       // processo filho
        close(pipes[i][0]); // filho escreve fechar pipe de leitura
        for (j = 0; j < i; j++)
        { //fechar pipes "herdados"
            close(pipes[j][0]);
            close(pipes[j][1]);
        }
        int limiteInferior = i * LIMITE_PROCESSO;                   //inicio do seu ciclo
        int limiteSuperior = limiteInferior + LIMITE_PROCESSO; //fim do seu ciclo
        int index;
        for (index = limiteInferior; index < limiteSuperior; index++)
        {
            if (sales[index].quantity > CRITERIA)
            {
                if (write(pipes[i][1], (void *)&(sales[index].product_code), sizeof(int)) == -1)
                { //escrita no pipe com verificação de processo
                    perror("Erro na escrita do filho");
                    return EXIT_FAILURE;
                }
            }
        }
        close(pipes[i][1]); // fechar o pipe de escrita
        exit(0);
    }
    if (pid[i] > 0)
    { // processo pai
        int p;
        for(j=0;j<NUM_PROCESSOS;j++){
            if (waitpid(pid[j], &status[j], 0) == -1)
            { //esperar por todos os processos
                perror("Erro waitpid");
                return EXIT_FAILURE;
            }
        }
        for (p = 0; p < NUM_PROCESSOS; p++)
        {
            if (WIFEXITED(status[p])) //Se processo p acabou
            {
                close(pipes[p][1]); // pai lê fechar pipe de escrita
                for (j = 0; j < p; j++)
                { //fechar pipes "herdados"
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
                /*
                Saber quanto ler para nao haver irregularidades no vetor de produtos que ultrapassam o numero maximo do Criterio
                */
                int venda = 0;
                int n = 1;
                while (1)
                {
                    n = read(pipes[p][0], (void *)&venda, sizeof(int));
                    if (n == 0)
                    { // se nao houve leitura termina o ciclo
                        break;
                    }
                    products[totalSales] = venda;
                    totalSales++;
                    if (n == -1)
                    {
                        perror("Erro leitura Pai");
                        return EXIT_FAILURE;
                    }
                }
                close(pipes[p][0]);
            }
        }
        for (j = 0; j < totalSales; j++)
        {
            printf("Sales above 20 product number: %d\n", products[j]);
        }
        printf("Total sales above 20: %d\n", totalSales);
    }

    return 0;
}