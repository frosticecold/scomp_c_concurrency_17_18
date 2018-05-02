#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define SEM_NAME_EMPTY "semaphore_empty_ex17"
#define SEM_NAME_FULL "semaphore_full_ex17"

#define SEM_NAME_VIP "semaphore_vip_ex17"
#define SEM_NAME_SPECIAL "semaphore_special_ex17"
#define SEM_NAME_NORMAL "semaphore_normal_ex17"

#define SEM_NAME_THEATER_STATUS "semaphore_theater_ex17"
#define SHAREDMEMORY_NAME "t_ex17"
#define MUTEX_NAME "semaphore_mutex_ex17"

#define SIZE 300

#define VIP 1
#define SPECIAL 2
#define NORMAL 3

typedef struct
{
    int vip_clients;
    int special_clients;
    int normal_clients;
} Theater;

int main()
{
    /*==================== INITIALIZE VARIABLES ====================*/
    /*====================      AND MEMORY      ====================*/

    pid_t pid;
    sem_t *sem_full;
    sem_t *sem_empty;
    sem_t *sem_vip;
    sem_t *sem_special;
    sem_t *sem_normal;
    Theater *t;

    /*ESTE EXERCICIO FUNCIONA EM LOOP INFINITO SE PRECISAR DE UNLINK 
    LIMPAR A MEMORIA OU SEMAFORO DESOMENTAR O CODIGO SEGUINTE...*/
    /*
    if (shm_unlink(SHAREDMEMORY_NAME) == -1)
    { //shm_unlink de memoria para remoção de ficheiro de memoria parilhada com verificação de erro
        perror("Erro unlink\n");
    }

    if (sem_unlink(SEM_NAME_EMPTY) == -1)
    {
        perror("Error unlink sem_empty\n");
    }

    if (sem_unlink(SEM_NAME_FULL) == -1)
    {
        perror("Error unlink sem_full\n");
    }

    if (sem_unlink(SEM_NAME_NORMAL) == -1)
    {
        perror("Error unlink sem_empty\n");
    }

    if (sem_unlink(SEM_NAME_SPECIAL) == -1)
    {
        perror("Error unlink sem_full\n");
    }

    if (sem_unlink(SEM_NAME_VIP) == -1)
    {
        perror("Error unlink sem_full\n");
    }
    */
    int fd, data_size = sizeof(Theater);

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

    t = (Theater *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //endereçamento para a estrutura "t" a memoria que foi partilhada

    if (t == MAP_FAILED) //verificação  se falhou o endereçamento de memoria partilhada
    {
        perror("Erro no endereçamento de memoria partilhada");
    }

    /*======================================================================*/

    /*==========================OPEN SEMAPHORES=============================*/

    sem_full = sem_open(SEM_NAME_FULL, O_CREAT | O_EXCL, 0644, 0);
    //value_empty 0 to coordidate with the sem_empty semaphore
    if (sem_full == SEM_FAILED)
    {
        perror("Can't sem_open() sem_full\n");
    }

    sem_empty = sem_open(SEM_NAME_EMPTY, O_CREAT | O_EXCL, 0644, SIZE);
    //value_empty 30 for mutual exclusion so it will block after the process
    if (sem_empty == SEM_FAILED)
    {
        perror("Can't sem_open() sem_empty\n");
    }

    sem_vip = sem_open(SEM_NAME_VIP, O_CREAT | O_EXCL, 0644, 0);
    //vip value 0 until it receives a signal it can get out
    if (sem_vip == SEM_FAILED)
    {
        perror("Can't sem_open() mutex\n");
    }

    sem_special = sem_open(SEM_NAME_SPECIAL, O_CREAT | O_EXCL, 0644, 0);
    //value_empty 1 for mutual exclusion, so it entering there is no people exiting
    if (sem_special == SEM_FAILED)
    {
        perror("Can't sem_open() mutex\n");
    }

    sem_normal = sem_open(SEM_NAME_NORMAL, O_CREAT | O_EXCL, 0644, 0);
    //value_empty 1 for mutual exclusion, so it entering there is no people exiting
    if (sem_normal == SEM_FAILED)
    {
        perror("Can't sem_open() mutex\n");
    }

    /*======================================================================*/

    pid = fork();
    if (pid == -1)
    {
        perror("fork error");
    }
    if (pid > 0)
    { /*FATHER
        -EXITS THE THEATER*/
        /*=====     VIP SECTION     =====*/
        int total_people;
        while (1)
        {
            sem_getvalue(sem_empty, &total_people); // check if its full
            if (total_people == SIZE)
            {
                printf("Theater is FULL, Someone exits...\n");
                sleep(2);
                /*SOMEONE ES LEAVING THE ROOM*/
                sem_wait(sem_full);
                sem_post(sem_empty);
                /*IF SOMEONE IS IN LINE THE SIGNALS IT CAN ENTER*/
                if (t->vip_clients > 0)
                {
                    printf("VIP client can enter\n");
                    sem_post(sem_vip); //signal the vip client can enter, someone has left the room
                }
                if (t->special_clients > 0)
                {
                    printf("SPECIAL client can enter\n");
                    sem_post(sem_special); //signal the special client can enter, someone has left the room
                }
                if(t->normal_clients>0)
                {
                    printf("NORMAL client can enter\n");
                    sem_post(sem_normal); //signal the normal client can enter, someone has left the room
                }
                /*==============================================*/
            }
            else
            {
                sem_wait(sem_full);
                sem_post(sem_empty);
            }
        }
        /*===============================*/
    }
    else
    { /*CHILD
        -POPULATES THE THEATER*/
        int total_people;
        while (1)
        {
            sem_getvalue(sem_full, &total_people); // check if its full
            if (total_people == SIZE)
            {
                int client_status = (rand() % 3) + 1; // generate people with status VIP 1 | SPECIAL 2 | NORMAL 3
                if (client_status == VIP)
                {
                    printf("Theater full , VIP client Waiting\n");
                    t->vip_clients++;
                    sleep(1);
                    sem_wait(sem_vip); // vip client has to wait for its turn
                    t->vip_clients--;
                    printf("Theater is now free , VIP client has entered!\n");
                    sleep(1);
                }
                if (client_status == SPECIAL)
                {
                    printf("Theater full , SPECIAL client Waiting\n");
                    t->special_clients++;
                    sleep(1);
                    sem_wait(sem_special); // special client has to wait for its turn
                    t->special_clients--;
                    printf("Theater is now free , SPECIAL client has entered!\n");
                    sleep(1);
                }
                if (client_status == NORMAL)
                {
                    printf("Theater full , NORMAL client Waiting\n");
                    t->normal_clients++;
                    sleep(1);
                    sem_wait(sem_normal); // normal client has to wait for its turn
                    t->normal_clients--;
                    printf("Theater is now free , NORMAL client has entered!\n");
                    sleep(1);
                }
                sem_wait(sem_empty);
                sem_post(sem_full);
            }
            else
            {
                sem_wait(sem_empty);
                sem_post(sem_full);
            }
        }
        exit(0);
    }

    /*==================== UNLINKING SEMAPHORES ====================*/

    if (sem_unlink(SEM_NAME_EMPTY) == -1)
    {
        perror("Error unlink sem_empty\n");
    }

    if (sem_unlink(SEM_NAME_FULL) == -1)
    {
        perror("Error unlink sem_full\n");
    }

    if (sem_unlink(SEM_NAME_NORMAL) == -1)
    {
        perror("Error unlink sem_empty\n");
    }

    if (sem_unlink(SEM_NAME_SPECIAL) == -1)
    {
        perror("Error unlink sem_full\n");
    }

    if (sem_unlink(SEM_NAME_VIP) == -1)
    {
        perror("Error unlink sem_full\n");
    }

    /*=================== CLEAR SHARED MEMORY ======================*/

    if (munmap(t, data_size) == -1)
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