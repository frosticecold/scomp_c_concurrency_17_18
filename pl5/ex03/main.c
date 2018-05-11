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
int* thread_id;

void generate_number(int* args){
    int i =0;
    for(;i<ARRAY_SIZE;i++)
    {
        array[i] = i+1;
    }
}

void* find_number(void* args){
    thread_id= (int*) args;
    
    int begin = *((int*) args) * NUM_PER_THREAD;
    int end = begin + NUM_PER_THREAD;
    for( ;begin<end; begin++){
        if(array[begin] == number){
            pthread_exit((void*)thread_id);
        }
    }
    pthread_exit(NULL);
}

int main()
{
    int index [THREAD_SIZE];
    srand(time(NULL));

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
        void* ret;
        if(pthread_join(threads[i],&ret) != 0 ){
            perror("Error pthread join\n");
        }
        if(ret != NULL){
            int* n = (int*)ret;
            printf("Thread %d found the number\n",*n);
        }
    }
    printf("====Todas as threads terminarem====\n");
    return 0;
}
