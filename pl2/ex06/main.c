#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUM_PROC 5
#define ARR_SIZE 1000
#define RANGE 100
#define STEP 200

int main()
{
    int arr_one[ARR_SIZE], arr_two[ARR_SIZE]; //Dois arrays
    srand(time(NULL));
    int sum = 0;
    int p;
    int fd[2]; //Pipe

    if (pipe(fd) == -1) //Criar o pipe,
    {                   //Se der erro
        perror("Erro ao criar o pipe.");
        return EXIT_FAILURE;
    }

    for (p = 0; p < ARR_SIZE; ++p)
    { //preencher arrays
        /*         arr_one[p]=rand() % RANGE;
        arr_two[p]=rand() % RANGE; */
        arr_one[p] = 1;
        arr_two[p] = 1;
    }

    pid_t vec_pid[NUM_PROC];       //Array com número de processos
    for (p = 0; p < NUM_PROC; ++p) //Criar cada processo
    {
        if ((vec_pid[p] = fork()) == -1) //Se falhar
        {
            return EXIT_FAILURE;
        }
        else
        {
            if (vec_pid[p] == 0) //Se for filho
            {
                close(fd[0]);                       //Fechar a leitura
                int initialIndex = 0, endIndex = 0; //Indice inicial e final do bocado a somar
                initialIndex = p * STEP;            //Inicio
                endIndex = initialIndex + STEP;     //Fim

                for (; initialIndex < endIndex; ++initialIndex) //De bocados entre STEP(200)
                {
                    sum += arr_one[initialIndex] + arr_two[initialIndex]; //Somar cada elemento vo vetor
                }
                if (write(fd[1], (void *)&sum, sizeof(sum)) == -1)
                { //Escrever apenas uma vez no pipe
                    perror("Erro de escrita no pipe.");
                    exit(1);
                }
                close(fd[1]); //Fechar a escrita
                exit(0);      //Terminar o filho
            }
        }
    }
    for (p = 0; p < NUM_PROC; ++p) //para todos os processos
    {
        int status;
        waitpid(vec_pid[p], &status, 0); //vamos esperar todos os processos
        if (WIFEXITED(status))           //Se acabou com sucesso
        {
            int readnum = 0;
            if (read(fd[0], (void *)&readnum, sizeof(status)) == -1)
            {   //Vamos ler o número que foi somado pelo filho
                //Se der erro, escrever erro
                perror("Erro de leitura no pipe.");
                return EXIT_FAILURE;
            }
            sum += readnum; //Adicionar à soma
        }
        else
        {
            perror("Child terminated abnormally");
            return EXIT_FAILURE;
        }
    }
    close(fd[0]);
    printf("The sum is:%d", sum);
    return 0;
    /*
    O read bloqueia canal enquanto não ler n bytes
    Enquanto que o write não bloqueia o canal
    */
}