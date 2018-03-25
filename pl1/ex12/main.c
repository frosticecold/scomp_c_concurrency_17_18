#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int spawn_childs(int n);

int main()
{
    const int NUM_PROC = 6;           //NUM CHILDS
    int num = spawn_childs(NUM_PROC); // criar NUM_CHILDS
    if (num == 0)                     // Se estamos no processo pai
    {
        int i;
        for (i = 0; i < NUM_PROC; ++i) //VAMOS ESPERAR NUM_PROC VEZES
        {
            wait(NULL);
        }
    }
    printf("Num processo:%d -- %d*2 = %d\n", num, num, num * 2); //Escrever output
    if (num > 0)
    {
        exit(0);
    }
    return 0;
}

/*
Função que cria n processos filhos
Retorna o indice do processo
*/
int spawn_childs(int n)
{
    if (n < 0)
    {
        return -1;
    }
    int i;
    for (i = 1; i <= n; ++i)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            return EXIT_FAILURE;
        }
        else
        {
            if (pid == 0)
            {
                break;
            }
        }
    }
    if (i > n)
    {
        i = 0;
    }
    return i;
}
