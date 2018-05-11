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
#include <limits.h>
#define ARRAY_SIZE 1000
#define THREAD_SIZE 3

int array[ARRAY_SIZE];
int lowest_value = INT_MAX;
int highest_value = INT_MIN;
float average = 0;

void *thread_function(void *args)
{
    int *thread_id = (int *)args;
    int i;
    switch (*thread_id)
    {
    case 0: //first thread will find the lowest value in the array
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            if (array[i] < lowest_value)
            {
                lowest_value = array[i];
            }
        }
        break;

    case 1: //second thread will find the highest value in the array
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            if (array[i] > highest_value)
            {
                highest_value = array[i];
            }
        }
        break;

    case 2: //will make the average of the array
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            average += array[i];
        }
        average /= ARRAY_SIZE;
        break;
    default:
        pthread_exit(NULL);
        break;
    }
    pthread_exit(NULL);
}

void generate_numbers()
{
    srand(time(NULL));
    int i = 0;
    for (; i < ARRAY_SIZE; i++)
    {
        array[i] = (rand() % 50) + 1;
    }
}

int main()
{
    pthread_t threads[THREAD_SIZE];
    generate_numbers();
    int index[THREAD_SIZE];
    int i = 0;
    for (; i < THREAD_SIZE; i++)
    {
        /*====Creating threads==================*/
        index[i] = i; //give the index the value to an array so that the pointer of i will not be losed in the thread function
        if (pthread_create(&threads[i], NULL, thread_function, (void *)&index[i]) == -1)
        { //confirming error for creatin thread
            perror("error creating thread\n");
        }
    }

    /*====Waiting for the threads==============*/
    for (i = 0; i < THREAD_SIZE; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        { //confirming the thread join NULL value because is not expected any particular return for the thread
            perror("Error pthread join\n");
        }
    }
    /*====Threads terminated===================*/

    printf("Lowest value is: %d\n", lowest_value);
    printf("Highest value is: %d\n", highest_value);
    printf("Average is: %.2f\n", average);
    return 0;
}
