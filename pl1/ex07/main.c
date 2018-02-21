#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#define ARRAY_SIZE 1000
int main()
{
    int numbers[ARRAY_SIZE]; /* array to lookup */
    int n;                   /* the number to find */
    time_t t;                /* needed to initialize random number generator (RNG) */
    int i;

    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned)time(&t));

    /* initialize array with random numbers (rand(): stdlib.h) */
    for (i = 0; i < ARRAY_SIZE; i++)
        numbers[i] = rand() % 10000;

    /* initialize n */
    n = rand() % 10000;


    //answer starts here
    int half = n / 2;
    int count = 0;
    int pid_status = 0;
    pid_t pid_f = fork();
    if (pid_f == -1)
    {
        return EXIT_FAILURE;
    }
    else
    {
        if (pid_f == 0) // processo filho vai percorrer metade do array
        {
            for (i = half; i < ARRAY_SIZE; ++i)
            {
                if (numbers[i] == n)
                {
                    count++;
                }
            }
            printf("Contador filho:%d\n", count);
            exit(count);
        } //Senão é o processo vai que vai executar
        else
        {
            for (i = 0; i < half; ++i)
            {
                if (numbers[i] == n)
                {
                    count++;
                }
            }
            waitpid(pid_f, &pid_status, 0);
            printf("Contador pai:%d\n", count);
            if (WIFEXITED(pid_status))
            {
                count += WEXITSTATUS(pid_status);
            }
            printf("Soma dos contadores=%d\n", count);
        }
    }
    return 0;
}