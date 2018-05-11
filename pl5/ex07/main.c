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
#define ARRAY_SIZE 10000
#define KEY_SIZE 5
#define THREAD_SIZE 10
#define TOTAL_NUM 49
#define NUM_PER_THREAD ARRAY_SIZE / THREAD_SIZE

int keys[ARRAY_SIZE][KEY_SIZE];
int statistics[TOTAL_NUM][1];
int total = 0;

pthread_mutex_t mutex[THREAD_SIZE];

void generate_number()
{
    int i = 0, j = 0;
    srand(time(NULL));
    int number;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        for (j = 0; j < KEY_SIZE; j++)
        {
            do
            {
                number = (rand() % TOTAL_NUM + 1);
                if (number == 0)
                    number = 1;
            } while (keys[i][0] == number || keys[i][1] == number || keys[i][2] == number || keys[i][3] == number || keys[i][4] == number);
            keys[i][j] = number;
        }
    }
}

void *thread_func(void *args)
{
    int *thread_id = (int *)args;
    int begin = (*thread_id) * NUM_PER_THREAD;
    int end = begin + NUM_PER_THREAD;
    int j = 0;
    pthread_mutex_lock(&mutex[*thread_id]);
    for (; begin < end; begin++)
    {
        for (j = 0; j < KEY_SIZE; j++)
        {
            int statistics_pos = keys[begin][j] - 1;
            statistics[statistics_pos][0]++;
            total++;
        }
    }
    pthread_mutex_unlock(&mutex[*thread_id]);
    if (*thread_id < THREAD_SIZE - 1) // confirm it doesnt unlock invalid mutex
    {                                 // if mutex 9 is not unlocking unexisting mutex 10
        pthread_mutex_unlock(&mutex[(*thread_id) + 1]);
    }
    pthread_exit(NULL);
}

void function_initialize()
{
    generate_number();
    int i = 0;
    for (i = 0; i < THREAD_SIZE; i++)
    {
        pthread_mutex_init(&mutex[i], NULL);
        /*if ( != 0)
        {
            perror("Error mutex creation\n");
        }
        if (i > 0)
        {
            pthread_mutex_lock(&mutex[i]);
            /*if ( != 0)
            {
                perror("error locking mutex\n");
            }*/
        // }
    }
    for (i = 0; i < TOTAL_NUM; i++)
    { //intialize the statistics counter
        statistics[i][0] = 0;
    }
}

int main()
{
    int index[THREAD_SIZE];
    srand(time(NULL));

    pthread_t threads[THREAD_SIZE];

    function_initialize(); // initialize mutexes, array and statistics
    int i = 0;
    for (; i < THREAD_SIZE; i++)
    {
        /*====Creating threads==================*/
        index[i] = i;
        if (pthread_create(&threads[i], NULL, thread_func, (void *)&index[i]) != 0)
        { //confirming error for creating thread
            perror("error creating thread\n");
        }
    }
    /*====Waiting for the threads==============*/

    for (i = 0; i < THREAD_SIZE; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Error pthread join\n");
        }
    }
    /*====Threads terminated===================*/
    int temp =0;
    for (i = 0; i < TOTAL_NUM; i++)
    {
        temp+=statistics[i][0];
        printf("Number: %d --- Times: %d\n", i + 1, statistics[i][0]);
    }
    printf("Total %d\n", total);

    printf("Temp %d\n", temp);
    return 0;
}
