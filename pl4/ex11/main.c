#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define SEM_PORTA1 "sem_porta1"
#define SEM_PORTA2 "sem_porta2"
#define SEM_PORTA3 "sem_porta3"
#define SEM_FULL "sem_full"
#define SEM_EMPTY "sem_empty"
#define NUMBER_OF_TRIPS 5
#define NUM_PROCESSES 3
#define MAX_PEOPLE 200

typedef struct
{
    int is_empty;
    int seats_remaining;
    int seats_occupied;
} Train;

int peopleGenerator(int ammount)
{
    int people;
    srand(time(NULL) * getpid());
    people = rand() % ammount;
    return people;
}

void peopleEntering(sem_t *full, sem_t *empty, Train *t)
{
    int numberOfPeople;
    int i = 0;
    if (t->is_empty == 0) //is empty
    {
        numberOfPeople = peopleGenerator(MAX_PEOPLE); //ammount of people exiting the train
        t->seats_remaining -= numberOfPeople;
        t->seats_occupied += numberOfPeople;
        t->is_empty = 1;
    }
    else
    {
        numberOfPeople = peopleGenerator(t->seats_remaining); //ammount of people exiting the train
        t->seats_remaining -= numberOfPeople;
        t->seats_occupied += numberOfPeople;
    }
    printf("Entering the train: %d people\n", numberOfPeople);
    sleep(1);
    for (; i < numberOfPeople; i++)
    {
        sem_post(full);
        sem_wait(empty);
    }
}

void peopleExiting(sem_t *full, sem_t *empty, Train *t)
{
    int numberOfPeople;
    int i = 0;
    if (t->is_empty == 1) //not empty
    {
        numberOfPeople = peopleGenerator(t->seats_occupied); //ammount of people exiting the train
        t->seats_remaining += numberOfPeople;
        t->seats_occupied -= numberOfPeople;
        if (t->seats_remaining == 0)
        {
            t->is_empty = 0;
        }
    }
    else
    {
        return;
    }
    printf("Exiting the train: %d people\n", numberOfPeople);
    sleep(1);
    for (; i < numberOfPeople; i++)
    {
        sem_wait(full);
        sem_post(empty);
    }
}

int main()
{
    pid_t pid[NUM_PROCESSES];
    sem_t *sem_porta1;
    sem_t *sem_porta2;
    sem_t *sem_porta3;
    sem_t *sem_empty;
    sem_t *sem_full;

    int fd, data_size = sizeof(Train);
    Train *t;
    int travel = 0;

    fd = shm_open("/semaphore11", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); //criação de ficheiro de memoria partilhada para escrita
    if (fd == -1)                                                                // verificação de criação de ficheiro de memoria partilhada para escrita
    {
        perror("Erro partilha de memoria Writer");
    }
    int memoryAmount = ftruncate(fd, data_size); // definição de memoria a ser partilhada
    if (memoryAmount == -1)                      // verificação de definição de memoria partilhada
    {
        perror("Erro a definir tamanho de memoria");
    }

    t = (Train *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //endereçamento para a estrutura "s" a memoria que foi partilhada

    if (t == MAP_FAILED) //verificação  se falhou o endereçamento de memoria partilhada
    {
        perror("Erro no endereçamento de memoria partilhada");
    }

    t->is_empty = 0;
    t->seats_remaining = MAX_PEOPLE;
    t->seats_occupied = 0;

    /*=========== OPEN THE SEMAPHORE FOR BEER AND CHIPS ============*/

    sem_porta1 = sem_open(SEM_PORTA1, O_CREAT | O_EXCL, 0644, 1);
    //value 1 for mutual exclusion, so it entering there is no people exiting
    if (sem_porta1 == SEM_FAILED)
    {
        perror("Can't sem_open() child\n");
    }

    sem_porta2 = sem_open(SEM_PORTA2, O_CREAT | O_EXCL, 0644, 1);
    //value 1 for mutual exclusion, so it entering there is no people exiting
    if (sem_porta2 == SEM_FAILED)
    {
        perror("Can't sem_open() child\n");
    }

    sem_porta3 = sem_open(SEM_PORTA3, O_CREAT | O_EXCL, 0644, 1);
    //value 1 for mutual exclusion, so it entering there is no people exiting
    if (sem_porta3 == SEM_FAILED)
    {
        perror("Can't sem_open() child\n");
    }

    sem_empty = sem_open(SEM_EMPTY, O_CREAT | O_EXCL, 0644, 200);
    //value 200 so that
    if (sem_empty == SEM_FAILED)
    {
        perror("Can't sem_open() child\n");
    }

    sem_full = sem_open(SEM_FULL, O_CREAT | O_EXCL, 0644, 0);
    //value 0 so that the parent can control the flow of the execution
    if (sem_full == SEM_FAILED)
    {
        perror("Can't sem_open() child\n");
    }

    /*==============================================================*/

    /*======================= TRAVEL PROCESS =======================*/

    for (; travel < NUM_PROCESSES; travel++)
    {
        pid[travel] = fork();
        if (pid[travel] == -1)
        {
            perror("Error pid travel\n");
            exit(1);
        }
        if (pid[travel] == 0)
        { //exiting the train
            sem_wait(sem_porta1);
            printf("Exiting door 1\n");
            peopleExiting(sem_full, sem_empty, t);
            sem_post(sem_porta1);

            sem_wait(sem_porta2);
            printf("Exiting door 2\n");
            peopleExiting(sem_full, sem_empty, t);
            sem_post(sem_porta2);

            sem_wait(sem_porta3);
            printf("Exiting door 3\n");
            peopleExiting(sem_full, sem_empty, t);

            sem_post(sem_porta3);

            exit(0);
        }
        else
        { //entering the train

            sem_wait(sem_porta1);
            printf("Entering door 1\n");
            peopleEntering(sem_full, sem_empty, t);
            sem_post(sem_porta1);

            sem_wait(sem_porta2);
            printf("Entering door 2\n");
            peopleEntering(sem_full, sem_empty, t);
            sem_post(sem_porta2);

            sem_wait(sem_porta3);
            printf("Entering door 3\n");
            peopleEntering(sem_full, sem_empty, t);
            sem_post(sem_porta3);
        }
    }

    /*==============================================================*/

    /*==================== UNLINKING SEMAPHORES ====================*/
    if (sem_unlink(SEM_EMPTY) == -1)
    {
        perror("Error unlink empty");
    }

    if (sem_unlink(SEM_FULL) == -1)
    {
        perror("Error unlink full");
    }

    if (sem_unlink(SEM_PORTA1) == -1)
    {
        perror("Error unlink porta 1");
    }

    if (sem_unlink(SEM_PORTA2) == -1)
    {
        perror("Error unlink porta 2");
    }

    if (sem_unlink(SEM_PORTA3) == -1)
    {
        perror("Error unlink porta 3");
    }
    /*==============================================================*/

    if (munmap(t, data_size) == -1)
    { //unmap de memoria com verificação de erro
        perror("Erro unmap\n");
    }
    if (close(fd) == -1)
    { //close de memoria com erificação de erro
        perror("Erro close\n");
    }
    if (shm_unlink("/semaphore11") == -1)
    { //shm_unlink de memoria para remoção de ficheiro de memoria parilhada com verificação de erro
        perror("Erro unlink\n");
    }
    /*==============================================================*/
    return 0;
}