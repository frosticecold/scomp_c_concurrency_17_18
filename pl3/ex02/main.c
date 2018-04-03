#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

#define ARRAY_SIZE 1000000
#define MAX_RAND 100

typedef struct
{
    int array[ARRAY_SIZE];
    unsigned char state;
    /*
    0 - A fazer nada
    1 - Vez do pai escrever alguma coisa
    2 - Vez do filho escrever alguma coisa
    3 - Filho terminou de escrever
    */
} shared_data_type;

void init_array(int *array, int n);
void test_speed_shared_memory(int *array, int n);
void test_speed_pipe(int *array, int n);
shared_data_type *create_shared_memory(void);

int main()
{

    int array[ARRAY_SIZE];
    init_array(array, ARRAY_SIZE);
    test_speed_shared_memory(array, ARRAY_SIZE);
    return 0;
}
/*
    Initialize the array with random numbers
*/
void init_array(int *array, int n)
{
    int i;
    srand(time(NULL) * getpid());
    for (i = 0; i < n; ++i)
        array[i] = (rand() % MAX_RAND);
}

/*
    Benchmark in CPU ticks of copying between shared memory the array
*/
void test_speed_shared_memory(int *array, int n)
{
    shared_data_type *addr = create_shared_memory();
    if (addr == NULL)
    {
        perror("Erro no speed test shared memory.");
        return;
    }
    pid_t pid;
    int ticks;
    if ((pid = fork()) == -1)
    {
        perror("Erro ao fazer fork no speed test shared memory.");
        return;
    }
    /*
    ======
    Codigo Pai
    ======
    */
    if (pid > 0)
    {
        addr->state = 1;
        int read_array[ARRAY_SIZE];
        clock_t start = clock();
        /*
            Escrever 1º Para Mem partilhada
        */
        int i;
        for (i = 0; i < n; ++i)
            addr->array[i] = array[i];
        /*
            Sinalizar que acabou de escrever e é a vez do filho

        */
        addr->state = 2;
        /*
            Esperar pela vez do pai ler
        */
        while (addr->state != 3)
            ;
        /*
            Ler da memória partilhada para o array
        */
        for (i = 0; i < n; ++i)
            read_array[i] = addr->array[i];
        /*
            Calc dif in ms
        */
        clock_t dif = clock() - start;
        //ticks = (dif * 1000);// / CLOCKS_PER_SEC;
        ticks = dif >> 1;
    }
    /*
    ======
    Codigo Filho
    ======
    */
    else
    {
        int read_array[ARRAY_SIZE];
        int i;
        /*
            Esperar pela  vez de ler
        */
        while (addr->state != 2)
            ;
        /*
            Ler
        */
        for (i = 0; i < n; ++i)
            read_array[i] = addr->array[i];
        /*
            Ler Após a Escrita
        */
        for (i = 0; i < n; ++i)
            addr->array[i] = read_array[i];
        /*
            Terminou de escrever
        */
        addr->state = 3;
        exit(0);
    }
    printf("SM Ticks:%d", ticks);
}

/*
    Criar a memória partilhada
*/
shared_data_type *create_shared_memory(void)
{
    int fd;
    char *workingdir = "/pl3_ex02";
    int flags = O_CREAT | O_EXCL | O_RDWR;
    mode_t mode = S_IRUSR | S_IWUSR;
    fd = shm_open(workingdir, flags, mode);
    if (fd == -1)
    {
        perror("Erro ao criar memoria partilhada.");
        return NULL;
    }
    if (ftruncate(fd, sizeof(shared_data_type)) == -1)
    {
        perror("Erro ao truncar a memória.");
        return NULL;
    }
    int prot_flags = PROT_READ | PROT_WRITE;
    shared_data_type *addr = mmap(NULL, sizeof(shared_data_type), prot_flags, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("Erro ao fazer map à memória.");
        return NULL;
    }
    return addr;
}
void test_speed_pipe(int *array, int n){
    int fd_father_to_child[2],fd_child_to_father[2];
}