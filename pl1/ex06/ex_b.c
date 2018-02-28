#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
    int i;
    //int status;

    for (i = 0; i < 4; i++)//Criar 4 processos
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            sleep(1); /*sleep(): unistd.h*/
            break;//Após criar um processo, saltar o ciclo for
        }
    }
    printf("This is the end. \n");
    return 0;
}