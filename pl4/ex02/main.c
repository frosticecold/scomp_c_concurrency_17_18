#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#define NUM_PROCESSES 8
#define BUFFER 256
#define ARRAY_SIZE 200
#define SEM_NAME "semaphore"

int main()
{
    pid_t pid[NUM_PROCESSES];
    int status[NUM_PROCESSES];
    sem_t *semaphore[NUM_PROCESSES];
    int i = 0;
    char name[NUM_PROCESSES][BUFFER];

    for (; i < NUM_PROCESSES; i++)
    {
        sprintf(name[i], "sem%d", i);
    }

    for (i = 0; i < NUM_PROCESSES; i++)
    {
        semaphore[i] = sem_open(name[i], O_CREAT | O_EXCL, 0644, 0);
        if (semaphore[i] == SEM_FAILED)
        {
            perror("Can't sem_open()");
            exit(1);
        }
    }

    for (i = 0; i < NUM_PROCESSES; i++)
    {
        pid[i] = fork();
        if (pid[i] == -1) // check fork creation
        {
            perror("Error fork");
            return 1;
        }
        if (pid[i] == 0)
        {
            if (sem_wait(semaphore[i]) == -1) // aplies a wait to the semaphore to notifiy it his time to run
            {
                perror("Error sem wait");
                exit(1);
            }

            FILE *inputfile;
            FILE *outputfile;

            inputfile = fopen("Numbers.txt", "r");
            outputfile = fopen("Output.txt", "a");

            if (outputfile == NULL || inputfile == NULL)
            {
                perror("Error opening file.");
                exit(1);
            }

            char buff[BUFFER];

            while (fgets(buff, sizeof(buff), inputfile) != NULL)
            {
                fprintf(outputfile, "%s", buff);
            }

            int closeoutputfile = fclose(outputfile);
            int closeinputfile = fclose(inputfile);

            if (closeoutputfile == -1 || closeinputfile == -1)
            {
                perror("Error closing file");
                return -1;
            }

            if (sem_post(semaphore[i])) // notifies the semaphore the it ended
            {
                perror("Error sem post");
                exit(1);
            }

            exit(0);
        }
    }
    for (i = 0; i < NUM_PROCESSES; i++)
    {
        if (sem_post(semaphore[i])) // notifies the process that is its turn
        {
            perror("Error sem post");
            exit(1);
        }
        if (waitpid(pid[i], &status[i], 0) == -1)
        {
            perror("Error waiting for process");
            return 1;
        }
    }

    FILE *printfile;

    printfile = fopen("Output.txt", "r");
    char buff[BUFFER];
    int line = 0;
    while (fgets(buff, sizeof(buff), printfile) != NULL)
    {
        printf("line %d:%s", line, buff);
        ++line;
    }
    printf("\n");

    for (i = 0; i < NUM_PROCESSES; i++)
    {
        if (sem_unlink(name[i]) == -1)
        {
            perror("Error waiting for process");
            return 1;
        }
    }

    return 0;
}