#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *calcprimes(void *);
int main()
{

    unsigned int limit = 0;
    printf("Please insert a number to check previous primes:");
    scanf("%u", &limit);
    pthread_t thread;
    if (pthread_create(&thread, NULL, calcprimes, &limit) != 0)
    {
        perror("Error pthread_create.");
        exit(1);
    }
    if (pthread_join(thread, NULL) != 0)
    {
        perror("Error pthread_join");
        exit(1);
    }
    return 0;
}

void *calcprimes(void *arg)
{
    unsigned int limit = *(unsigned int *)arg;
    if (limit > 1)
    {
        printf("2\n");
    }
    int running = 1;
    int c;
    int i = 3;
    if (limit >= i)
    {
        while (running)
        {
            for (c = 2; c < i; c++)
            {
                if (i % c == 0)
                    break;
            }
            if (c == i)
            {
                printf("%d\n", i);
            }
            i++;
            if (i > limit)
                running = 0;
        }
    }
    pthread_exit(0);
}