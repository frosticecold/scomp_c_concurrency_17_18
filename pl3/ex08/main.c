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
/*
    Aluno
*/
typedef struct
{
    int numero;
    char nome[STR_SIZE];
    int disciplinas[NR_DISC];
} aluno;

int main(){


    return 0;
}
