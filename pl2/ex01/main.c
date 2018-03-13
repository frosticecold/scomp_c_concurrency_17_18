#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    pid_t pid;
    int fd[2];
    if (pipe(fd) == -1)         //teste para verificar sucesso pipe
    {
        perror("FAIL PIPE");
        return EXIT_FAILURE;
    }
    pid = fork();               //criação de um processo
    if (pid == -1)              // verificar erro ca criação do processo
    {
        perror("Erro criação de processo");
        return EXIT_FAILURE;
    }
    if (pid > 0)                // verificar se é processo pai
    {
        close(fd[0]);           // como vai ser escrito fechar pipe para leitura
        printf("---Parent printing---\nPID: %d\n", pid);    // imprimir  pid de pai
        if(write(fd[1], (void *)&pid, sizeof(pid)) == -1){
            perror("Erro escrita"); //fazer a escrita para o pipe
            return EXIT_FAILURE;
        }
        close(fd[1]);           //fechar o pipe para escrita
    }
    if (pid == 0)               // verificar se é porcesso filho
    {
        close(fd[1]);           //fechar pipe para escrita
        if (read(fd[0], (void *)&pid, sizeof(pid)) == -1){
            perror("Erro Leitura"); //fazer a leitura do pipe
            return EXIT_FAILURE;
        }
        printf("---Child printing---\nPID: %d\n", pid);     
        close(fd[0]);           //  fechar pipe de leitura
    }
    return 0;
}