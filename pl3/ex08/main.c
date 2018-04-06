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
    defines
*/
#define STR_SIZE 50
#define NR_DISC 10

enum State
{
    Nothing = 0,
    FatherWriting = 1,
    FatherFinishedWriting = 2,
    ChildReading = 3,
    ChildFinishedWriting = 4,
    EndCycle = 5
};
/*
    structs
*/
typedef struct
{
    int numero;
    char nome[STR_SIZE];
    int disciplinas[NR_DISC];
} Aluno;

typedef struct
{
    Aluno aluno;
    int fd;
    enum State state;
    /*
        0 - Doing nothing
        1 - Pai a escrever
        2 - Pai terminou de escrever
        3 - Filho a ler
        4 - Filho terminou de escrever
        5 - Terminar ciclo
    */
} shared_memory;
/*
    Functions
*/
shared_memory *create_shared_memory(void);
void delete_shared_memory(shared_memory *addr);
void my_code(shared_memory *addr);
void cpyaluno(Aluno *dest, Aluno *src);

/*
    Constants
*/
const char *SHM_FILENAME = "/pl3_ex08";

int main()
{
    shared_memory *addr = create_shared_memory();
    if (addr == NULL)
        return EXIT_FAILURE;
    my_code(addr);
    delete_shared_memory(addr);
    return 0;
}
shared_memory *create_shared_memory(void)
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
    shared_memory *addr = mmap(NULL, sizeof(shared_memory), prot_flags, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("Erro ao fazer map à memória.");
        return NULL;
    }
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
void cpyaluno(Aluno *dest, Aluno *src)
{
    dest->numero = src->numero;
    strncpy(dest->nome, src->nome, STR_SIZE);
    memcpy(dest->disciplinas, src->disciplinas, sizeof(int) * NR_DISC);
}
/*
    Actual exercise code
*/
void my_code(shared_memory *addr)
{
    pid_t pid;
    if ((pid = fork()) == -1)
    {
        perror("Erro ao criar filho.");
        return;
    }

    /*
    ==========
    Codigo Pai
    ==========
    */
    if (pid > 0)
    {
        addr->state = FatherWriting;
        while (addr->state != EndCycle)
        {
            Aluno input;
            printf("Por favor introduza o número do Aluno:");
            scanf("%d", &input.numero);
            printf("\nPor favor introduza o nome do Aluno:");
            scanf("%s", input.nome);
            printf("\nPor favor introduza as notas da disciplina");
            int i;
            for (i = 0; i < NR_DISC; ++i)
            {
                printf("\nNota[%d]", i);
                scanf("%d", &input.disciplinas[i]);
            }
            cpyaluno(&addr->aluno, &input);
            addr->state = FatherFinishedWriting;
            while (addr->state == FatherFinishedWriting || addr->state == ChildReading || addr->state == ChildFinishedWriting)
                ;
        }
        waitpid(pid, NULL, 0);
    }
    /*
    ==========
    Codigo Filho
    ==========
    */
    else
    {
        while (addr->state != FatherFinishedWriting)
            ;
        addr->state = ChildReading;
        int highest, lowest, avg;
        int i;
        for (i = 0; i < NR_DISC; ++i)
        {
            int num = addr->aluno.disciplinas[i];
            if (i == 0)
            {
                highest = num;
                lowest = num;
                avg = num;
            }
            else
            {
                if (highest < num)
                    highest = num;
                if (lowest > num)
                    lowest = num;
                avg += num;
            }
        }
        avg /= NR_DISC;
        addr->state = ChildFinishedWriting;
        printf("Aluno:%d Highest:%d Lowest:%d Avg:%d",
               addr->aluno.numero, highest, lowest, avg);
        addr->state = EndCycle;
        exit(0);
    }
}
