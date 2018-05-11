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
        if (pthread_mutex_init(&mutex[i], NULL) != 0)
        {
            perror("Erro ao criar mutex.");
            exit(1);
        }
        if (i > 0)
        {
            if (pthread_mutex_lock(&mutex[i]) != 0)
            {
                perror("Erro ao fazer mutex lock");
                exit(1);
            }
        }
    }
    pthread_t threads[NUM_THREADS];
    for (i = 0; i < NUM_THREADS; i++)
    {
        th_index[i] = i;
        if (pthread_create(&threads[i], NULL, docalc, &th_index[i]) != 0)
        {
            perror("Erro ao criar thread.");
            exit(1);
        }
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
    if (pthread_mutex_lock(&mutex[thread_id]) != 0)
    {
        pthread_exit((void *)1);
    }
    for (i = thread_id * STEP; i < limit; i++)
        printf("Result[%d]=%d\n", i, result[i]);
    if (pthread_mutex_unlock(&mutex[thread_id]) != 0)
    {
        pthread_exit((void *)1);
    }
    if (thread_id < NUM_THREADS - 1)
    {
        if (pthread_mutex_unlock(&mutex[thread_id + 1]) != 0)
        {
            pthread_exit((void *)1);
        }
    }
    pthread_exit(0);
}