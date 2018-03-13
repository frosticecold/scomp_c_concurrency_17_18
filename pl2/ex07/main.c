#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#define VEC_SIZE 1000
#define NUM_PROCESSES 5 

void numberGenerator(int* vec){
    time_t t;                /* needed to initialize random number generator (RNG) */
    int i;
    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned)time(&t));

    /* initialize array with random numbers (rand(): stdlib.h) */
    for (i = 0; i < VEC_SIZE; i++){
        vec[i] = rand() % 255;
    }
        
}

int main(){
    pid_t pid [NUM_PROCESSES];
    int vec [VEC_SIZE];
    int pipe[2];
    int i=0;
    numberGenerator(vec);
    
    for(;i<NUM_PROCESSES;i++){
        pid[i] = fork();
        if(pid[i] == -1){
            perror("Erro na criação de processos");
            return EXIT_FAILURE;
        }
    }

    return 0;
}