#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_THREADS 5
#define DATA_STR_SIZE 64
typedef struct
{
    int number;
    char name[64];
    char address[64];
} Data;

void creatStruct(Data *arraypos, int num, char *name, char *addr);
void *print_struct(void *d);

int main()
{
    Data array[5];
    const int ARRAY_SIZE = 5;
    creatStruct(&array[0], 1, "Raúl", "ISEP");
    creatStruct(&array[1], 2, "Mário", "ISEP");
    creatStruct(&array[2], 3, "André", "ISEP");
    creatStruct(&array[3], 4, "Rui", "ISEP");
    creatStruct(&array[4], 5, "Miguel", "ISEP");

    pthread_t threads[5];
    int i;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        if (pthread_create(&threads[i], NULL, print_struct, &array[i]) != 0)
        {
            perror("Erro ao criar a thread...");
            exit(1);
        }
    }

    printf("====Vamos esperar por todas as threads====\n");
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        if(pthread_join(threads[i], NULL)!=0){
            perror("Erro pthread_join.");
            exit(1);
        }
    }
    printf("====Todas as threads terminarem====");
    return 0;
}

void creatStruct(Data *arraypos, int num, char *name, char *addr)
{

    arraypos->number = num;
    strncpy(arraypos->name, name, DATA_STR_SIZE);
    strncpy(arraypos->address, addr, DATA_STR_SIZE);
}

void *print_struct(void *d)
{
    Data *ptr = (Data *)d;
    printf("Name:%s Number:%d\nAddress:%s\n", ptr->name, ptr->number, ptr->address);
    pthread_exit(NULL);
}