#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
void main()
{
    int i;
    int status;

    for (i = 0; i < 4; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            sleep(1); /*sleep(): unistd.h*/
            break;
        }
        if (pid > 0)
        {
            //printf("Reached here: %d",pid);
            int status;
            if (pid % 2 == 0)
            {
                waitpid(pid, &status, 0);
            }
        }
    }
    printf("This is the end. \n");
}