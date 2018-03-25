#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define MESSAGE_SIZE 256

void convertString(char *str)
{

    while (*str != '\0')
    {
        if (islower(*str)) // se letra minuscula 
        {
            *str = toupper(*str);// converte para maiuscula
        }
        else
        {// se letra Maiuscula
            *str = tolower(*str);// converte para minuscula
        }
        ++str;
    }
}

int main()
{
    pid_t pid;
    int client[2];
    int server[2];
    char messageClient[MESSAGE_SIZE]; // mensagem que vai ser feita o input de utilizador
    char receivedMessageClient[MESSAGE_SIZE]; // mensagem que server vai receber de client
    char receivedMessageServer[MESSAGE_SIZE]; // mensagem que client vai receber de server
    
    if (pipe(client) == -1) //teste para verificar sucesso pipe
    {
        perror("FAIL PIPE CLIENT");
        return EXIT_FAILURE;
    }
    if (pipe(server) == -1) //teste para verificar sucesso pipe
    {
        perror("FAIL PIPE SERVER");
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
        close(client[1]); // fechar escrita de pipe para fazer leitura
        if(read(client[0], (void *)receivedMessageClient,MESSAGE_SIZE ) == -1) { // verificação de sucesso na leitura do pipe client
            perror("Erro na leitura de client Pai");
            return EXIT_FAILURE;
        }
        close(client[0]);// fechar pipe de leitura de client
        printf("---Server---\nReceived message: %s\n",receivedMessageClient);
        convertString(receivedMessageClient); // metodo que converte letras minusculas em Maiusculas e vice-versa        
        
        close(server[0]);// fechar pipe de leitura para escrita server
        if(write(server[1], (void *)receivedMessageClient, MESSAGE_SIZE) == -1){ // escrita no pipe com teste de sucesso
            perror ("Erro escrita pelo Server");
            return EXIT_FAILURE;
        }
        close(server[1]); // fechar o pipe de escrita
    }
    if(pid == 0) //se processo filho
    {
        close(client[0]); // fechar pipe client de leitura para fazer escrita
        printf("Insira a palavra para enviar para server: ");
        scanf("%s",messageClient); //input de nova string para se escrever no pipe
        if(write(client[1], (void *)messageClient, MESSAGE_SIZE) == -1){ // escrita no pipe com teste de sucesso
            perror ("Erro escrita pelo filho");
            return EXIT_FAILURE;
        }
        close(client[1]); //fechar pipe de escrita apos a escrita no pipe client
        
        close(server[1]); // fechar pipe de escrita para faszer leitura pipe server
        if(read(server[0], (void *)receivedMessageServer,MESSAGE_SIZE ) == -1) { // leitura de pipe com teste de sucesso para receber e imprimir a nova palavra convertida do server
            perror("Erro na leitura de Client");
            return EXIT_FAILURE;
        }
        close(server[0]); // fechar leitura de pipe apos leitura
        printf("---Client---\nReceived and converted message: %s\n",receivedMessageServer);
        exit(2);//terminar processo 
    }
    return 0;
}
