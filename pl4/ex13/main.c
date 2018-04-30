#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define SEM_NAME_EMPTY "semaphore_empty_ex13"
#define SEM_NAME_FULL "semaphore_full_ex13"
#define SHAREDMEMORY_NAME "addr_ex13"
#define MUTEX_NAME "semaphore_mutex_ex13"
#define BALANCE_NAME "semaphore_balance_ex13"
#define BUFFER 30
#define SIZE 10

typedef struct
{
    int buffer[SIZE];
    int producer_index;
    int consumer_index;
    int rnd_num;
} shared_memory;


int main()
{
    /*==================== INITIALIZE VARIABLES ====================*/
    /*====================      AND MEMORY      ====================*/

    pid_t pid;
    sem_t *sem_full;
    sem_t *sem_empty;
    sem_t *mutex;
    sem_t *balance;
    shared_memory *addr;

    int fd, data_size = sizeof(shared_memory);

    fd = shm_open(SHAREDMEMORY_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); //criação de ficheiro de memoria partilhada para escrita
    if (fd == -1)
    {
        perror("Erro partilha de memoria Writer");
    }
    int memoryAmount = ftruncate(fd, data_size); // definição de memoria a ser partilhada
    if (memoryAmount == -1)                      // verificação de definição de memoria partilhada
    {
        perror("Erro a definir tamanho de memoria");
    }

    addr = (shared_memory *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //endereçamento para a estrutura "addr" a memoria que foi partilhada

    if (addr == MAP_FAILED) //verificação  se falhou o endereçamento de memoria partilhada
    {
        perror("Erro no endereçamento de memoria partilhada");
    }

    srand(time(NULL));
    addr->producer_index = 0;
    addr->consumer_index = 0;
    addr->rnd_num = rand()%BUFFER;

    /*==============================================================*/

    balance = sem_open(BALANCE_NAME, O_CREAT | O_EXCL, 0644, 0);
    //value_empty 0 to coordidate with the sem_empty semaphore
    if (balance == SEM_FAILED)
    {
        perror("Can't sem_open() sem_full\n");
    }

    sem_full = sem_open(SEM_NAME_FULL, O_CREAT | O_EXCL, 0644, 0);
    //value_empty 0 to coordidate with the sem_empty semaphore
    if (sem_full == SEM_FAILED)
    {
        perror("Can't sem_open() sem_full\n");
    }

    sem_empty = sem_open(SEM_NAME_EMPTY, O_CREAT | O_EXCL, 0644, 30);
    //value_empty 30 for mutual exclusion so it will block after the process
    if (sem_empty == SEM_FAILED)
    {
        perror("Can't sem_open() sem_empty\n");
    }

    mutex = sem_open(MUTEX_NAME, O_CREAT | O_EXCL, 0644, 1);
    //value_empty 1 for mutual exclusion, so it entering there is no people exiting
    if (mutex == SEM_FAILED)
    {
        perror("Can't sem_open() mutex\n");
    }
    int iteration = BUFFER;
    /*==============================================================*/

    /*================= PRODUCER CONSUMER PROBLEM ==================*/

    pid = fork();
    if (pid == -1)
    {
        perror("Error in fork()");
    }

    if (pid > 0)
    { /*FATHER ------ CONSUMER*/

        int index = 0;
        do
        {
            if (addr->consumer_index == SIZE - 1) //check if it consumed all the buffer
            {
                sem_post(balance);        // if yes then gives time to the producer
                addr->consumer_index = 0; //restart the index for the producer
            }
            sem_wait(sem_full); //notifies its going to consume
            sem_wait(mutex);    //create exclusive access to the critical code
            printf("Consumer --- Counter[%d] Number:%d\n", index, addr->buffer[addr->consumer_index]);
            addr->consumer_index++;
            index++;
            sem_post(mutex);     //notifies it ended the critical code
            sem_post(sem_empty); //signal it consumed the buffer
        } while (iteration--);
        exit(0);
    }
    else
    { /*CHILD ------ PRODUCER*/

        int index = 0;
        do
        {
            if (addr->producer_index == SIZE - 1) //check if it produced all the buffer
            {
                sem_wait(balance); //if so gives time to the producer to catch up blocking the semaphore
                addr->producer_index = 0;
            }
            sem_wait(sem_empty);                                //tells the process is going to produce
            sem_wait(mutex);                                    //creates exclusive access to the shared memory
            addr->buffer[addr->producer_index] = addr->rnd_num; // assigns the buffer the random number
            addr->rnd_num++;                                    //increments the random number
            printf("Producer --- Counter[%d] Number:%d\n", index, addr->buffer[addr->producer_index]);
            addr->producer_index++;
            index++;
            sem_post(mutex);    //noticies other processes it finished the critical code
            sem_post(sem_full); //noticies the end of the production
        } while (iteration--);
    }

    /*==============================================================*/

    /*==================== UNLINKING SEMAPHORES ====================*/

    if (sem_unlink(BALANCE_NAME) == -1)
    {
        perror("Error unlink sem_empty");
    }

    if (sem_unlink(SEM_NAME_EMPTY) == -1)
    {
        perror("Error unlink sem_empty");
    }

    if (sem_unlink(SEM_NAME_FULL) == -1)
    {
        perror("Error unlink sem_full");
    }

    if (sem_unlink(MUTEX_NAME) == -1)
    {
        perror("Error unlink Mutex");
    }

    /*=================== CLEAR SHARED MEMORY ======================*/

    if (munmap(addr, data_size) == -1)
    { //unmap de memoria com verificação de erro
        perror("Erro unmap\n");
    }
    if (close(fd) == -1)
    { //close de memoria com erificação de erro
        perror("Erro close\n");
    }
    if (shm_unlink(SHAREDMEMORY_NAME) == -1)
    { //shm_unlink de memoria para remoção de ficheiro de memoria parilhada com verificação de erro
        perror("Erro unlink\n");
    }
    /*==============================================================*/

    return 0;
}