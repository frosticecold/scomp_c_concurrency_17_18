#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#define ARRAY_SIZE 1000
#define THREAD_SIZE 5
#define NUM_PER_THREAD ARRAY_SIZE/THREAD_SIZE

int array [ARRAY_SIZE];

int number;

void generate_number(int* args){
    int i =0;
    for(;i<ARRAY_SIZE;i++)
    {
        array[i] = i+1;
    }
}

void* find_number(void* args){
    int exit_value= *((int*) args);
    int begin = *((int*) args) * NUM_PER_THREAD;
    int end = begin + NUM_PER_THREAD;
    int flag = 0;
    for( ;begin<end; begin++){
        if(array[begin] == number){
            printf("Number in the array\n");
            flag = 1;
            break;
        }
    }
    if(flag == 1){
        pthread_exit((void*)&exit_value);
    }
    pthread_exit(NULL);
}

int main()
{
    int index [THREAD_SIZE];
    srand(time(NULL));
    void* ret [THREAD_SIZE] ;

    number = rand() % ARRAY_SIZE;

    generate_number(array);

    pthread_t threads[THREAD_SIZE];
    int i=0;
    printf("The number to search is %d\n", number);
    for (; i < THREAD_SIZE; i++)
    {
        index[i] = i;
        if(pthread_create(&threads[i], NULL, find_number, (void*)&index[i]) == -1) {
            perror("error creating thread\n");
        }
    }

    printf("====Vamos esperar por todas as threads====\n");
    for (i = 0; i < THREAD_SIZE; i++)
    {
        pthread_join(threads[i], &ret[i]);
        if(ret[i] != NULL){
            printf("Thread %s found the number\n",*((int*)ret[i]));
        }
    }
    printf("====Todas as threads terminarem====\n");
    return 0;
}
