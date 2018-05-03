#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

#define ARRAY_SIZE 1000
#define LIMIT_BY_PROCESS 100
#define LIMIT_INTEGER 1000
#define NUM_PROCESSES 10
#define MAX_NUM_SIZE 10

int main()
{
    pid_t pid[NUM_PROCESSES];
    int status[NUM_PROCESSES];
    int *biggestTenElements;
    int vec_values[ARRAY_SIZE]; //declaracao de vetor
    int i = 0;
    int globalMax = -1;

    int fd, data_size = sizeof(biggestTenElements); // definição de variavel  para tamhanho de memoria e para a criação de memoria partilhada

    fd = shm_open("/ex05", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); //criação de ficheiro de memoria partilhada para escrita
    if (fd == -1)                                                         // verificação de criação de ficheiro de memoria partilhada para escrita
    {
        perror("Erro partilha de memoria Writer");
        return EXIT_FAILURE;
    }
    int memoryAmount = ftruncate(fd, data_size); // definição de memoria a ser partilhada
    if (memoryAmount == -1)                      // verificação de definição de memoria partilhada
    {
        perror("Erro a definir tamanho de memoria");
        return EXIT_FAILURE;
    }

    biggestTenElements = (int *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //endereçamento para a estrutura "s" a memoria que foi partilhada

    if (biggestTenElements == MAP_FAILED) //verificação  se falhou o endereçamento de memoria partilhada
    {
        perror("Erro no endereçamento de memoria partilhada");
        return EXIT_FAILURE;
    }
    srand(time(NULL));
    for (; i < ARRAY_SIZE; ++i)
    {
        vec_values[i] = rand() % LIMIT_INTEGER;
    }

    for (i = 0; i < NUM_PROCESSES; ++i)
    {
        pid[i] = fork();
        if (pid[i] == -1)
        {
            printf("Erro na criacao de processo");
            exit(-1);
        }
        if (pid[i] == 0)
        {
            int minInterval = i * LIMIT_BY_PROCESS;
            int maxInterval = minInterval + LIMIT_BY_PROCESS;
            int tempMinValue = -1;
            for (; minInterval < maxInterval; minInterval++)
            {
                if (vec_values[minInterval] > tempMinValue)
                {
                    tempMinValue = vec_values[minInterval];
                }
            }
            biggestTenElements[i] = tempMinValue;
            exit(1);
        }
    }
    for (i = 0; i < NUM_PROCESSES; i++)
    {
        waitpid(pid[i], &status[i], 0);
    }
    for (i = 0; i < NUM_PROCESSES; i++)
    {
        if (WIFEXITED(status[i]))
        {
            printf("Number[%d] : %d\n", i, biggestTenElements[i]);
            if (biggestTenElements[i] > globalMax)
            {
                globalMax = biggestTenElements[i];
            }
        }
    }
    printf("The biggest global number is %d\n", globalMax);

    if (munmap(biggestTenElements, data_size) == -1)
    { //unmap de memoria com verificação de erro
        printf("Erro unmap\n");
    }
    if (close(fd) == -1)
    { //close de memoria com verificação de erro
        printf("Erro close\n");
    }
    if (shm_unlink("/ex05") == -1)
    { //shm_unlink de memoria para remoção de ficheiro de memoria parilhada com verificação de erro
        printf("Erro unlink\n");
    }

    return 0;

    /**
     *  Se o número de processos for igual ao número de cores disponíveis,
     * é a melhor performance existente
    **/
}