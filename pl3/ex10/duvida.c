#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#define BUFF_SIZE 10
#define SHM_FILENAME "/pl3_ex10"
#define MAX_EXCHANGES 30
#define MAX_NUM_RAND 1000
typedef struct
{
    int buffer[BUFF_SIZE];
    /* File Descriptor */
    int fd;
    /* Counter */
    int counter;
    /* Turn */
    int turn;
    /* */
    int flag[2];
} shared_memory;

shared_memory *create_shared_memory(void);
void delete_shared_memory(shared_memory *addr);

int main()
{
    shared_memory *addr = create_shared_memory();
    if (addr == NULL)
    {
        perror("Erro ao criar a memória partilhada");
        return EXIT_FAILURE;
    }
    int id_prod = 0;
    int id_cons = 1;
    addr->turn = id_prod;
    pid_t pid;
    if ((pid = fork()) == -1)
    {
        perror("Erro ao criar fork.");
        delete_shared_memory(addr);
        return EXIT_FAILURE;
    }
    /*
    ================
        Pai
            Produtor
    ================
    */
    if (pid > 0)
    {
        srand(time(NULL));
        const int STEP = 10;
        int num = -1;
        do
        {
            addr->flag[id_prod] = 1;
            addr->turn = id_cons;
            while (addr->flag[id_cons] && addr->turn == id_cons)
                ;
            (num == -1) ? (num = rand() % MAX_NUM_RAND) : (num += STEP);
            int idx = (addr->counter + 1) % BUFF_SIZE;
            addr->buffer[idx] = num;
            (addr->counter)++;

            addr->flag[id_prod] = 0;
            if (addr->counter == MAX_EXCHANGES)
                break;
        } while (1);
    }
    /*
    ================
        Filho
            Consumidor
    ================
    */
    else
    {
        do
        {
            addr->flag[id_cons] = 1;
            addr->turn = id_prod;
            while (addr->flag[id_prod] && addr->turn == id_prod)
                ;
            int idx = (addr->counter + 1) % BUFF_SIZE;
            printf("Counter[%d] Number:%d",addr->counter,addr->buffer[idx]);
            addr->flag[id_cons] = 0;
            if(addr->counter == MAX_EXCHANGES){
                exit(0);
            }
        } while (1);
    }
    delete_shared_memory(addr);
    return 0;
}
shared_memory *create_shared_memory(void)
{
    int fd;
    const char *const workingdir = SHM_FILENAME;
    int flags = O_CREAT | O_EXCL | O_RDWR;
    mode_t mode = S_IRUSR | S_IWUSR;
    fd = shm_open(workingdir, flags, mode);
    if (fd == -1)
    {
        perror("Erro ao criar memoria partilhada.");
        return NULL;
    }
    if (ftruncate(fd, sizeof(shared_memory)) == -1)
    {
        perror("Erro ao truncar a memória.");
        return NULL;
    }
    int prot_flags = PROT_READ | PROT_WRITE;
    shared_memory *addr = mmap(NULL, sizeof(shared_memory), prot_flags, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("Erro ao fazer map à memória.");
        return NULL;
    }
    memset(&addr[0], 0, sizeof(shared_memory));
    addr->fd = fd;
    return addr;
}
void delete_shared_memory(shared_memory *addr)
{
    int fd = addr->fd;
    munmap(addr, sizeof(shared_memory));
    close(fd);
    if (shm_unlink(SHM_FILENAME) == -1)
        perror("Erro ao fazer shm unlink");
}