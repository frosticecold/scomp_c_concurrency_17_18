#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MESSAGE_SIZE 256

void convertString(char *str, int flag)
{

    while (*str != '\0')
    {

        if (islower(*str))
        {
            *str = toupper(*str);
        }
        else
        {
            if (isupper(*str))
            {
                *str = tolower(*str);
            }
        }
        ++str;
    }
}

int main()
{
    pid_t pid;
    int client[2];
    int server[2];
    int status;
    char message[MESSAGE_SIZE];
    
    if (pipe(client) == -1) //teste para verificar sucesso pipe
    {
        perror("FAIL PIPE");
        return EXIT_FAILURE;
    }
    pid = fork();  //criação de um processo
    if (pid == -1) //verificar erro na criação do processo
    {
        perror("Erro criação de processo");
        return EXIT_FAILURE;
    }
    if (pid > 0)
    {
        wait(&status);
        if (WIFEXITED(status))
        {
            close(client[1]);

        }
    }
    if(pid == 0) //se processo filho
    {
        close(client[0]);
        scanf("%s",message);
        if(write(client[1], (void *)message, MESSAGE_SIZE) == -1){
            perror ("Erro escrita pelo filho");
            return EXIT_FAILURE;
        }
        close(client[1]);
        exit(2);
    }
    return 0;
}
