#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
void main()
{
    int i;
    int status;

    for (i = 0; i < 4; i++)
    {
        pid_t pid = fork();
        int exitcde;
        if (pid == 0)
        {
            sleep(1); /*sleep(): unistd.h*/
            exit(i + 1);
        }
        if (pid > 0)
        {
            int status = 0;
            if (pid % 2 == 0)
            {
                waitpid(pid, &status, 0);
                //Supõe-se que apenas se pode imprimir os processos porque se esperaram.
                if (WIFEXITED(status))
                {
                    printf("Son pid:%d returned the value:%d\n", pid, WEXITSTATUS(status));
                }
            }
        }
    }
    printf("This is the end. \n");
}