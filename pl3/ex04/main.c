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

/*  
    Function Prototypes
*/
int *create_shared_memory(void);
void delete_shared_memory(int *addr);
void exec_repeated_operation(char *str, int *addr);

/*
    Defines
*/
#define NUM_OP 1000000

/*
    Constants
*/
int fd_shared;
const char *SHM_FILENAME = "/pl3_ex04";

int main()
{
    int *addr = create_shared_memory();
    if (addr == NULL)
    {
        perror("Erro ao criar a memória partilhada.");
        return EXIT_FAILURE;
    }
    *addr = 100;
    pid_t pid;
    if ((pid = fork()) == -1)
    {
        perror("Erro ao fazer fork.");
        return EXIT_FAILURE;
    }
    if (pid > 0)
    {
        exec_repeated_operation("Pai", addr);
        waitpid(pid, NULL, 0);
    }
    else
    {
        exec_repeated_operation("Filho", addr);
        exit(0);
    }
    delete_shared_memory(addr);
    return 0;
}
/*
    Criar a memória partilhada
*/
int *create_shared_memory(void)
{
    int fd;
    const char *workingdir = SHM_FILENAME;
    int flags = O_CREAT | O_EXCL | O_RDWR;
    mode_t mode = S_IRUSR | S_IWUSR;
    fd = shm_open(workingdir, flags, mode);
    if (fd == -1)
    {
        perror("Erro ao criar memoria partilhada.");
        return NULL;
    }
    if (ftruncate(fd, sizeof(int)) == -1)
    {
        perror("Erro ao truncar a memória.");
        return NULL;
    }
    int prot_flags = PROT_READ | PROT_WRITE;
    int *addr = mmap(NULL, sizeof(int), prot_flags, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("Erro ao fazer map à memória.");
        return NULL;
    }
    return addr;
}
void delete_shared_memory(int *addr)
{
    if (munmap(addr, sizeof(int)) == MAP_FAILED)
    {
        perror("Erro ao fazer map unmapping");
        return;
    };
    if (close(fd_shared) == -1)
    {
        perror("Erro ao fazer close.");
        return;
    }
    if (shm_unlink(SHM_FILENAME) == -1)
    {
        perror("Erro ao fazer unlink");
        return;
    }
}
void exec_repeated_operation(char *str, int *addr)
{

    int i;
    for (i = 0; i < NUM_OP; ++i)
    {
        (*addr)++;
        (*addr)--;
    }
    printf("%s FinalResult:%d\n", str, *addr);
}

/*
Não os valores não estão sempre corretos,
Existem duas execuções ao mesmo tempo a
Incrementar/Decrementar a mesma variável.
Não é possível desta maneira, garantir que
cada operação seja atómica.
*/
