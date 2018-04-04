#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#define TOTAL 10

int main()
{
    int* vec;

    int fd, data_size = TOTAL*4 ; // definição de variavel  para tamhanho de memoria e para a criação de memoria partilhada
    
    fd = shm_open("/vec10", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); //criação de ficheiro de memoria partilhada para leitura
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

    vec = (int*)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //endereçamento para a estrutura "s" a memoria que foi partilhada

    if (vec == MAP_FAILED) //verificação  se falhou o endereçamento de memoria partilhada
    {
        perror("Erro no endereçamento de memoria partilhada");
        return EXIT_FAILURE;
    }     
    /*
        IMPRESSAO NO MONITOR DE RESULTADOS PROVENIENTES DA MEMORIA PARTILHADA ALOCADA NA ESTRUTURA DE "Student" 
    */

    int i =0;
    for(;i<TOTAL;i++){
        vec[i]=i+1;
    }
    return 0;
}