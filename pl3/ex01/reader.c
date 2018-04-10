#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef struct
{
    char name[50];
    int number;
} Student;

int main()
{
    int fd, data_size = sizeof(Student); // definição de variavel  para tamhanho de memoria e para a criação de memoria partilhada
    Student *s; // declaração estrutura aluno 

    fd = shm_open("/writer", O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); //criação de ficheiro de memoria partilhada para leitura
    if (fd == -1) // verificação de criação de ficheiro de memoria partilhada para leitura
    {
        perror("Erro partilha de memoria Reader");
        return EXIT_FAILURE;
    }
    int memoryAmount = ftruncate(fd, data_size); // definição de memoria a ser partilhada
    if (memoryAmount == -1)// verificação de definição de memoria partilhada
    {
        perror("Erro a definir tamanho de memoria");
        return EXIT_FAILURE;
    }

    s = (Student *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //endereçamento para a estrutura "s" a memoria que foi partilhada

    if (s == MAP_FAILED) //verificação  se falhou o endereçamento de memoria partilhada
    {
        perror("Erro no endereçamento de memoria partilhada");
        return EXIT_FAILURE;
    }     
    /*
        IMPRESSAO NO MONITOR DE RESULTADOS PROVENIENTES DA MEMORIA PARTILHADA ALOCADA NA ESTRUTURA DE "Student" 
    */
    printf("---Print Result---\nNome do aluno é : %s\nNumero: %d\n", s->name, s->number);

    if(munmap(s,data_size) == -1){//unmap de memoria com verificação de erro 
        printf("Erro unmap");
    }
    if(close(fd) == -1){//close de memoria com verificação de erro 
        printf("Erro close");
    }
    if(shm_unlink("/writer") == -1){//shm_unlink de memoria para remoção de ficheiro de memoria parilhada com verificação de erro 
        printf("Erro unlink");
    }
    return 0;
}