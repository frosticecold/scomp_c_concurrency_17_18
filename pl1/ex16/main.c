#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int my_exec(char *command);

int main()
{
    //const int BUFSIZE = 64;
    //char buf[BUFSIZE];
    //my_exec(buf);
    //strncpy(buf,"ls",BUFSIZE);
    int ret = 0;
    ret = my_exec("ls");
    if (ret == 1)
    {
        printf("Error executing!");
    }
    ret=my_exec("who");
    if (ret == 1)
    {
        printf("Error executing!");
    }
    ret=my_exec("ps");
    if (ret == 1)
    {
        printf("Error executing!");
    }
    return 0;
}

int my_exec(char *command)
{
    int ret;
    int status = 0;
    pid_t pid = fork();
    if (pid == -1)
    {
        return EXIT_FAILURE;
    }
    else
    {
        if (pid == 0)
        {
            ret = execlp(command, command, NULL);
            if(ret == -1) ret = 1;
            exit(ret);
        }
        else
        {
            wait(&status);
            if (WIFEXITED(status))
            {
                status = WEXITSTATUS(status);
            }
            else
            {
                status = EXIT_FAILURE;
            }
        }
    }
    return status;
}