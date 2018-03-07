#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 256
int main()
{
    char buf[BUF_SIZE];
    int fd[2];
    if (pipe(fd) == -1)
    {
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        return EXIT_FAILURE;
    }
    else
    {
        if (pid == 0)
        {
            close(fd[1]);
            while (read(fd[0], (void *)buf, BUF_SIZE))
            {
                printf("%s", buf);
            }
            close(fd[0]);
        }
        else
        {
            close(fd[0]);
            FILE *txtfile;
            txtfile = fopen("./text.txt", "r+");
            while (fgets(buf, BUF_SIZE, txtfile) != NULL)
            {
                write(fd[1], (void *)buf, strlen(buf));
            }
            int status;
            close(fd[1]);
            wait(&status);
        }
    }

    return 0;
}