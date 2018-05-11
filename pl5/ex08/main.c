#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 1000
#define NUM_THREADS 5
#define STEP ARRAY_SIZE / NUM_THREADS
#define MAX_RANDOM 100
int data[ARRAY_SIZE];
int result[ARRAY_SIZE];
pthread_mutex_t mutex[NUM_THREADS];
void *docalc(void *args);
int main()
{

    int i;
    int th_index[NUM_THREADS];
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        data[i] = rand() % MAX_RANDOM;
    }

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_mutex_init(&mutex[i], NULL);
        if (i > 0)
        {
            pthread_mutex_lock(&mutex[i]);
        }
    }
    pthread_t threads[NUM_THREADS];
    for (i = 0; i < NUM_THREADS; i++)
    {
        th_index[i] = i;
        pthread_create(&threads[i], NULL, docalc, &th_index[i]);
    }
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    return 0;
}

void *docalc(void *args)
{

    int thread_id = *(int *)args;
    int i, limit;
    const int mult = 2;
    const int sum = 10;
    i = thread_id * STEP;
    limit = i + STEP;
    for (; i < limit; i++)
    {
        result[i] = data[i] * mult + sum;
    }
    pthread_mutex_lock(&mutex[thread_id]);
    for (i = thread_id * STEP; i < limit; i++)
        printf("Result[%d]=%d\n", i, result[i]);
    pthread_mutex_unlock(&mutex[thread_id]);
    if (thread_id < NUM_THREADS - 1)
    {
        pthread_mutex_unlock(&mutex[thread_id + 1]);
    }
    pthread_exit(0);
}