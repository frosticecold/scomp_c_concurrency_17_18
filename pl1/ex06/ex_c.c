#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
    int i;

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
            if(pid %2 == 0){
                printf("We will wait for: %d\n",pid);
            }
            //printf("Reached here: %d",pid);
            int status;
            if (pid % 2 == 0)
            {
                waitpid(pid, &status, 0);
            }
        }
    }
    printf("This is the end. \n");
    return 0;
}