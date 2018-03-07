#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int i;
    for(i=0;i<10;i++){
        execlp("echo","echo", "SCOMP", NULL);
    }
    /*so e imprimido uma vez o text SCOMP, pois o sucesso do comando 
    execlp() so executa uma vez, pois em caso de alguma falha o comando seria repetido 
    na proxima iteração e assim sucessivamente.  
    */
    return 0;
}
