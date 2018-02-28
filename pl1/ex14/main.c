#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int i;
    for (i = 1; i < 4; i++)
        fork();
    execlp("echo", "echo", "SCOMP", NULL);

    /*A) SCOMP vai ser imprimido 8 vezes
    */

    /*B) Vai ser imprimido 8 vezes, 
    pois vai existir 2^3 processos (4-1)
    E cada processo vai executar o comando execlp, como argumento o caminho do ficheiro
    O nome do executavel, echo
    E o argumento ao comando echo, "SCOMP"
    Após terminar com sucesso, o processo que executou o exec termina.
    Caso não tenha terminado com sucesso, retoma a execução do programa.
    */
}