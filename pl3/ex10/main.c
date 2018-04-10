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
        int i, in = 0;
        //const int STEP = 10;
        //int num = -1;
        for (i = 0; i < MAX_EXCHANGES; ++i)
        {
            while (addr->counter == BUFF_SIZE)
                ;
            srand(time(NULL) + i);
            addr->buffer[in] = rand() % MAX_NUM_RAND;
            in = (in + 1) % BUFF_SIZE;
            addr->counter++;
        }
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
        do
        {
            int i, out = 0;
            for (i = 0; i < MAX_EXCHANGES; ++i)
            {
                while (addr->counter == 0)
                    ;
                printf("Array[%d] = %d\n", i, addr->buffer[out]);
                out = (out + 1) % BUFF_SIZE;
                addr->counter--;
            }
            exit(0);
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
    if (munmap(addr, sizeof(shared_memory)) == -1)
    {
        perror("Erro ao fazer unmap");
        return;
    }
    if (close(fd) == -1)
    {
        perror("Erro ao fechar file descriptor.");
        return;
    }
    if (shm_unlink(SHM_FILENAME) == -1)
        perror("Erro ao fazer shm unlink");
    return;
}