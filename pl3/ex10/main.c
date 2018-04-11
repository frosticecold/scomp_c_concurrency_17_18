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
#define SHM_DELETE "/dev/shm/pl3_ex10"
#define MAX_EXCHANGES 30
#define MAX_NUM_RAND 1000
typedef struct
{
    int buffer[BUFF_SIZE];
    /* File Descriptor */
    int fd;

    /* Turn */
    //int turn;
    /* */
    //int flag[2];

    int idx_prod;

    int idx_cons;

    int counter;
} shared_memory;

shared_memory *create_shared_memory(void);
void delete_shared_memory(shared_memory *addr);

int main()
{
    if (access(SHM_DELETE, F_OK)==0)
    {
        if(unlink(SHM_DELETE) < 0 )
        perror("Erro ao apagar.");
    }
    shared_memory *addr = create_shared_memory();
    if (addr == NULL)
    {
        perror("Erro ao criar a memória partilhada");
        return EXIT_FAILURE;
    }
    int numExec = MAX_EXCHANGES;
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
        addr->counter = 0;
        do
        {
            //addr->flag[id_prod] = 1;
            //addr->turn = id_cons;
            while (addr->idx_prod == BUFF_SIZE - 1 && addr->idx_cons == 0)
                ;
            while (addr->idx_prod == (addr->idx_cons - 1))
                ;
            (num == -1) ? (num = rand() % MAX_NUM_RAND) : (num += STEP);
            addr->buffer[addr->idx_prod] = num;
            (addr->idx_prod == BUFF_SIZE - 1) ? (addr->idx_prod = 0) : ((addr->idx_prod)++);
            (addr->counter)++;
        } while (--numExec);
        int status;
        waitpid(pid, &status, 0);
    }
    /*
    ================
        Filho
            Consumidor
    ================
    */
    else
    {
        int counter = 0;
        do
        {
            if (addr->counter != MAX_EXCHANGES && addr->idx_cons == BUFF_SIZE - 1 && addr->idx_prod == 0)
                while (addr->idx_cons == BUFF_SIZE - 1 && addr->idx_prod == 0)
                    ;
            //            while (addr->flag[id_prod] && addr->turn == id_prod)
            //              ;
            else if (addr->counter != MAX_EXCHANGES)
                while (addr->idx_cons == addr->idx_prod - 1)
                    ;
            printf("Counter[%d] Number:%d\n", counter++, addr->buffer[addr->idx_cons]);
            (addr->idx_cons == BUFF_SIZE - 1) ? (addr->idx_cons = 0) : ((addr->idx_cons)++);
        } while (--numExec);
        exit(0);
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