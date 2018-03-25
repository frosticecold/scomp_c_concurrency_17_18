#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "info.h"

/*
==================================================================
    Constants
==================================================================
*/
#define NUM_BARCODES 5
#define NUM_OF_PRODUCTS 5

int main()
{
    //Vector de processos
    pid_t pid[NUM_BARCODES];
    //Vector de pipes
    int pipe_father_to_child[NUM_BARCODES][2];
    int pipe_childs_to_father[2];
    int p;

    /*
    ==================================================================
        Criar pipes e verificar se são válidos
    ==================================================================
    */
    if (pipe(pipe_childs_to_father) == -1)
    {
        perror("Erro ao criar o pipe");
        return EXIT_FAILURE;
    }

    //Criar os pipes necessários
    for (p = 0; p < NUM_BARCODES; ++p)
    {
        if (pipe(pipe_father_to_child[p]) == -1)
        {
            perror("Erro ao criar o pipe.");
            return EXIT_FAILURE;
        }
    }

    /*
    ==================================================================
        Criar todos os processos
    ==================================================================
            */
    for (p = 0; p < NUM_BARCODES; ++p)
    {
        pid[p] = fork();  //Criar um processo novo
        if (pid[p] == -1) //Se não conseguir criar
        {
            perror("Erro ao criar o filho");
            return EXIT_FAILURE;
        }
        else
        {
            /*
            ==================================================================
            Processos filho
            ==================================================================
            */
            if (pid[p] == 0) //Se for o processo filho
            {
                int j;
                /*
                ==================================================================
                Fechar os pipes herdados
                ==================================================================
                */
                for (j = 0; j < p; ++j)
                {
                    close(pipe_father_to_child[j][0]);
                    close(pipe_father_to_child[j][1]);
                }
                break;
            } //Código do filho acaba aqui, salta o ciclo fora

            /*
            ==================================================================
            Processos Pai
            ==================================================================
            */
            else
            {
                //Fechar a leitura de todos os pipes no processo pai.
                close(pipe_father_to_child[p][0]);
            }
        }
    }

    /*
    ==================================================================
        Código processo pai
    ==================================================================
    */
    if (p >= NUM_BARCODES)
    {
        close(pipe_childs_to_father[1]);
        /*
        ==================================================================
            Criar a base de dados
        ==================================================================
        */
        Product db[NUM_OF_PRODUCTS];
        db[0] = createProduct("Product A", 10, 1);
        db[1] = createProduct("Product B", 20, 2);
        db[2] = createProduct("Product C", 30, 3);
        db[3] = createProduct("Product D", 40, 4);
        db[4] = createProduct("Product E", 50, 5);
        //Reinicializar a variável p
        p = 0;
        //Ciclo de leitura dos Requests dos filhos
        //Mandar resposta com informação dos produtos
        while (1)
        {
            int readtype;
            Request req;
            if ((readtype = read(pipe_childs_to_father[0], (void *)&req, sizeof(req))) == -1)
            {
                perror("Erro ao ler o Request no pai dos filhos");
                return EXIT_FAILURE;
            }
            //Verificar se não existe mais nada para ler
            if (readtype == 0)
            {
                break;
            }
            Product prod = db[req.cod_prod];
            if (write(pipe_father_to_child[req.num_processo][1], (void *)&prod, sizeof(prod)) == -1)
            {
                perror("Erro ao escrever Produto no pai para filho");
                return EXIT_FAILURE;
            }
        }

        //É porque o ciclo while acabou, e pode-se fechar o pipe partilhado
        close(pipe_childs_to_father[0]);

        //Fechar a escrita de todo pai -> filho
        //A leitura já foi fechada de pai-> filho
        int i;
        for (i = 0; i < NUM_BARCODES; ++i)
        {
            close(pipe_father_to_child[i][1]);
        }
    }
    /*
    ==================================================================
        Código processo filho
    ==================================================================
    */
    else
    {
        close(pipe_childs_to_father[0]);
        close(pipe_father_to_child[p][1]);
        srand(time(NULL) * getpid());
        /*
            ==================================================================
                Gerar um pedido e escrever
            ==================================================================
            */
        Request req;
        req.cod_prod = rand() % NUM_OF_PRODUCTS;
        req.num_processo = p;
        if (write(pipe_childs_to_father[1], (void *)&req, sizeof(req)) == -1)
        {
            perror("Erro ao escrever do filho para o pai");
            exit(1);
        }
        /*
            ==================================================================
                Obter um produto do pai
            ==================================================================
            */
        Product prod;
        if (read(pipe_father_to_child[p][0], (void *)&prod, sizeof(prod)) == -1)
        {
            perror("Erro ao ler do filho para o pai");
            exit(1);
        }
        printf("Produto Nome:%s Cod:%d\n", prod.p_name, prod.p_price);
        close(pipe_father_to_child[p][0]);
        exit(0);
    }

    return 0;
}