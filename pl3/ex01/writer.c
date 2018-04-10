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
    int fd, data_size = sizeof(Student);// definição de variavel  para tamhanho de memoria e para a criação de memoria partilhada
    Student *s;

    fd = shm_open("/writer", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); //criação de ficheiro de memoria partilhada para escrita
    if (fd == -1)// verificação de criação de ficheiro de memoria partilhada para escrita
    {
        perror("Erro partilha de memoria Writer");
        return EXIT_FAILURE;
    }
    int memoryAmount = ftruncate(fd, data_size);// definição de memoria a ser partilhada
    if (memoryAmount == -1) // verificação de definição de memoria partilhada
    {
        perror("Erro a definir tamanho de memoria");
        return EXIT_FAILURE;
    }

    s = (Student *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);//endereçamento para a estrutura "s" a memoria que foi partilhada

    if (s == MAP_FAILED)//verificação  se falhou o endereçamento de memoria partilhada
    {
        perror("Erro no endereçamento de memoria partilhada");
        return EXIT_FAILURE;
    }
    /*
        INSERÇÃO DE DADOS NA ESTRUTURA PARA PARTILHA DE MEMORIA
    */
    printf("Insira o nome aluno: ");
    scanf("%s", s->name);
    printf("Insira o numero aluno: ");
    scanf("%d", &(s->number));
    if(munmap(s,data_size) == -1){
        printf("Erro unmap");
    }
    if(close(fd) == -1){
        printf("Erro close");
    }
    return 0;
}