#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
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
    }
    printf("This is the end. \n");
}