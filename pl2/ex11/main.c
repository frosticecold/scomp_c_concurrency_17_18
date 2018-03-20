#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define LIMITE 500
#define NUM_PROCESSOS 5
#define READ 0
#define WRITE 1

int randomNumber()
{
    time_t t; /* needed to initialize random number generator (RNG) */
    srand((unsigned)time(&t) * getpid());
    int number = (rand() % LIMITE) + 1;
    return number;
}

int father(int  pipes[NUM_PROCESSOS + 1][2])
{
    int greaterNumber;
    // Processo pai
    int r = randomNumber();
    printf("---Processo pai PID:   %d--- Numero Gerado: %d\n", getpid(), r);
    close(pipes[0][READ]);
    if (write(pipes[0][WRITE], (void *)&r, sizeof(int)) == -1)
    { // escrita do pai para o seu filho
        perror("Erro escrita pai");
        return EXIT_FAILURE;
    }
    close(pipes[0][WRITE]);

    close(pipes[NUM_PROCESSOS][WRITE]);
    if (read(pipes[NUM_PROCESSOS][READ], (void *)&greaterNumber, sizeof(int)) == -1)
    {
        perror("Erro Leitura pai");
        return EXIT_FAILURE;
    }
    printf("---Processo pai PID:   %d--- Numero Maior gerado: %d\n", getpid(), greaterNumber);
    close(pipes[NUM_PROCESSOS][READ]);
    return 0;
}

int main()
{
    int pipes[NUM_PROCESSOS + 1][2];
    pid_t pid[NUM_PROCESSOS];
    //int status[NUM_PROCESSOS];
    
    int j, i = 0;

    for (i = 0; i < NUM_PROCESSOS + 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        { //teste na criação do pipe
            perror("Erro no pipe");
            return EXIT_FAILURE;
        }
    }

    for (i = 0; i < NUM_PROCESSOS; i++)
    {
        pid[i] = fork(); // criação de processo
        if (pid[i] == -1)
        { // verificar sucesso de criação de processo
            perror("Erro na criação de processos");
            return EXIT_FAILURE;
        }

        if (pid[i] == 0)
        { // se filho
            close(pipes[i][WRITE]);
            for (j = 0; j < i; j++)
            { //fechar pipes "herdados"
                close(pipes[j][READ]);
                close(pipes[j][WRITE]);
            }
            int newRandomNumber = randomNumber(); // criar numero aleatorio
            int previousRandomNumber = 0;
            printf("---Processo Filho PID: %d--- Numero Gerado: %d\n", getpid(), newRandomNumber);
            if (read(pipes[i][READ], (void *)&previousRandomNumber, sizeof(int)) == -1) // leitura do pipe com verificação
            {
                printf("%d\n", i);
                perror("Erro leitura Filho");
                return EXIT_FAILURE;
            }
            close(pipes[i][READ]); // fechar o pipe de leitura apos a leitura pipe

            int compare = newRandomNumber > previousRandomNumber ? newRandomNumber : previousRandomNumber; // verificar se numero lido é maior que numero gerado pelo filho
            close(pipes[i + 1][READ]); //fechar pipe de escrita do proximo filho 
            if (write(pipes[i + 1][WRITE], (void *)&compare, sizeof(int)) == -1) // escrita no pipe para o proximo filho
            {
                printf("%d\n", i);
                perror("Erro escrita filho");
                return EXIT_FAILURE;
            }
            close(pipes[i + 1][WRITE]);// fechar o pipe de escrita proximo filho apos escrita bem sucedida
            exit(2);
        }
    }
    if(father(pipes) != 0){
        return EXIT_FAILURE;
    }

    return 0;
}