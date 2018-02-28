#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define NUM_PROC 4
int main()
{
    int i;
    pid_t pid;
    for (i = 0; i < NUM_PROC; i++)//Criar 4 processos
    {
        pid = fork();
        if (pid == 0)//Se for filho então dormir
        {
            sleep(1); /*sleep(): unistd.h*/
            break;
        }
        if (pid > 0)
        {
            if(pid %2 == 0){//Se for um processo par
                printf("We will wait for: %d\n",pid);
                waitpid(pid, NULL, 0);
            }
        }
    }
    printf("This is the end. \n");
    return 0;
}