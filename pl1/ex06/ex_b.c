#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
void main()
{
    int i;
    int status;

    for (i = 0; i < 4; i++)
    {
        if (fork() == 0)
        {
            sleep(1); /*sleep(): unistd.h*/
        }
    }
    printf("This is the end. \n");
}