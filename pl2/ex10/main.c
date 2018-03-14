#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define STARTING_MONEY 20

#define MIN 1
#define MAX 5
int main()
{
    int buffer;

    int pipe_father_to_child[2];
    int pipe_child_to_father[2];
    /*
    ==================================================================
        Criar pipes e verificar se são válidos
    ==================================================================
            */
    if (pipe(pipe_father_to_child) == -1)
    {
        perror("Erro ao criar o pipe");
        return EXIT_FAILURE;
    }
    if (pipe(pipe_child_to_father) == -1)
    {
        perror("Erro ao criar o pipe");
        return EXIT_FAILURE;
    }
    //Criar filho
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Erro ao criar o filho.");
        return EXIT_FAILURE;
    }
    if (pid > 0) //Se for o processo pai
    {
        srand(time(NULL) * getpid());
        close(pipe_father_to_child[0]); // Fechar a extremidade de leitura de pai -> filho
        close(pipe_child_to_father[1]); // Fechar a extremidade de escrita de filho -> pai
        int runagain = 1;
        int credito = STARTING_MONEY;
        do
        {
            int aposta_gerada = (rand() % MAX) + 1;
            int number;
            //Vamos ler o credito do filho

            /*
            ==================================================================
            Verificar Credito
            ==================================================================
            */
            if (credito > 0)
            {
                number = 1;
                if (write(pipe_father_to_child[1], (void *)&number, sizeof(int)) == -1)
                {
                    perror("Erro ao escrever no processo pai");
                    return EXIT_FAILURE;
                }
            }
            else
            { //Senão informar que não pode apostar
                number = 0;
                runagain = 0;
                if (write(pipe_father_to_child[1], (void *)&number, sizeof(int)) == -1)
                {
                    perror("Erro ao escrever no processo pai");
                    return EXIT_FAILURE;
                }
                break;
            }
            /*
            ==================================================================
            OK, verificou o crédito e mandou a resposta ao filho,
            Se for válida
            Ler a aposta do filho e incrementar/decrementar credito
            ==================================================================
            */
            //Ler aposta do filho
            if (read(pipe_child_to_father[0], (void *)&buffer, sizeof(int)) == -1)
            {
                perror("Erro ao ler no processo pai.");
                return EXIT_FAILURE;
            }
            int apostafilho = buffer;
            if (apostafilho == aposta_gerada)
            {
                credito += 10;
            }
            else
            {
                credito -= 5;
            }
            /*
            ==================================================================
            Enviar para o filho o novo crédito
            ==================================================================
            */
            if (write(pipe_father_to_child[1], (void *)&credito, sizeof(int)) == -1)
            {
                perror("Erro ao escrever no processo pai.");
                return EXIT_FAILURE;
            }
        } while (runagain);
        /*
            ==================================================================
            Fechar extremidades do pipe
            ==================================================================
            */
        close(pipe_father_to_child[1]); // Fechar a extremidade de escrita de pai -> filho
        close(pipe_child_to_father[0]); // Fechar a extremidade de leitura de filho -> pai
    }
    else
    {
        int podeApostar;
        //Apostar enquanto puder
        srand(time(NULL) * getpid());
        close(pipe_father_to_child[1]); // Fechar a extremidade de escrita de pai -> filho
        close(pipe_child_to_father[0]); // Fechar a extremidade de leitura de filho -> pai
        do
        {
            /*
            ==================================================================
            Ler do pai se podemos apostar
            ==================================================================
            */
            if (read(pipe_father_to_child[0], (void *)&buffer, sizeof(int)) == -1)
            {
                perror("Erro ao ler no processo filho.");
                return EXIT_FAILURE;
            }
            podeApostar = buffer;
            /*
            ==================================================================
            Decidir se pode apostar
            ==================================================================
            */
            if (podeApostar == 1)
            {
                /*
                ==================================================================
                Gerar um número random e escrever para o pai
                ==================================================================
            */
                int bet = (rand() % MAX) + 1;
                if (write(pipe_child_to_father[1], (void *)&bet, sizeof(int)) == -1)
                {
                    perror("Erro ao escrever no processo filho.");
                    return EXIT_FAILURE;
                }
                /*
                ==================================================================
                Ler o novo crédito e imprimir.
                ==================================================================
            */
                if (read(pipe_father_to_child[0], (void *)&buffer, sizeof(int)) == -1)
                {
                    perror("Erro ao ler no processo filho.");
                    return EXIT_FAILURE;
                }
                int credito = buffer;
                printf("\n======\nA aposta do filho é:%d\nO crédito do filho é:%d\n======", bet, credito);
            }
            else
            {   
                //Fechar as extremidades e terminar o processo
                close(pipe_father_to_child[0]);
                close(pipe_child_to_father[1]);
                exit(0);
            }
            //Se for o processo filho
        } while (podeApostar);
    }

    return 0;
}