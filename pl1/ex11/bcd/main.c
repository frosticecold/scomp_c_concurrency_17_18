#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <time.h>

#define ARRAY_SIZE 1000
#define LIMIT_BY_PROCESS 200
#define LIMIT_INTEGER 255
#define NUM_PROCESSES 5

int main(){
    pid_t pid [NUM_PROCESSES];
    int vec_values[ARRAY_SIZE]; //declaracao de vetor 
    int i = 0;                  //maxIntervalValue para vetor
    time_t t;                   //para poder criar numeros aleatorios (rand())
    int max_value=-1;           //valor maximo do array completo 
    int result[ARRAY_SIZE];      //array de resultados para alinea b) e c)
    srand((unsigned)time(&t));

    for(;i < ARRAY_SIZE;++i){
        vec_values[i] = rand()%LIMIT_INTEGER;
    }
    
    for(i=0;i<NUM_PROCESSES;++i){
        pid[i] = fork();
        if(pid[i] == -1){
            printf("Erro na criacao de processo");
            exit(-1);
        }
        if(pid[i]==0){
            break;
        }
    }
    if(i == NUM_PROCESSES) {
        i=0;
    }
    if(pid[i] == 0){
        int limitPerCycle;
        int beginingCycle;
        int max = -1;          //declaraçao de valor min para comparacao
        if(i == 0){ //caso seja o maxIntervalValue zero tem um tratamento especial
            beginingCycle=0;
            limitPerCycle = LIMIT_BY_PROCESS;
        }else{     
            beginingCycle=i*LIMIT_BY_PROCESS;   //tratamento de inicio de ciclo (1 * 200 = 200) começa na posicao 200
            limitPerCycle = (i+1) * LIMIT_BY_PROCESS; //tratamento de fim de ciclo ((1+1) * 200 = 400) o seu limite será 400 (nao incluido)
        }
        for(i=beginingCycle;i<limitPerCycle;++i){
            if(vec_values[i]>max){
                max=vec_values[i]; 
            }
        }
        exit(max);
    }
    if(pid[i] > 0){
        int p;
        int status[NUM_PROCESSES];
        
        for (p = 0; p < NUM_PROCESSES; ++p)//Então vamos esperar por todos os processos
        {
            waitpid(pid[p], &status[p], 0);
        }
        for (p = 0; p < NUM_PROCESSES; ++p)//Depois de esperar
        {
            if (WIFEXITED(status[p]))//Se acabou
            {
                int maxIntervalValue = WEXITSTATUS(status[p]);
                //se acabou mostrar valor maximo do intervalo do processo 0-255
                if (maxIntervalValue >= 0 && maxIntervalValue <= 255)//se numero valido
                {
                    if (maxIntervalValue > max_value
                    ){
                        max_value
                         = maxIntervalValue;
                    }
                    //int begin = (p==0)?0:p*LIMIT_BY_PROCESS;
                    //int end = (p==0)?LIMIT_BY_PROCESS:((p+1)*LIMIT_BY_PROCESS)-1;
                    //printf("Intervalo: %d - %d Valor: %d\n", begin,end,maxIntervalValue);
                    //printf("PID Id: %d\n", pid[p]);
                }            
            }
        }
        pid_t pid = fork(); // alinea b processo para calcular result[i]=((int) numbers[i]/max_value)*100 em metado do vetor
        int statusB;        //status para processo alinea b) e c) 
        int half = ARRAY_SIZE / 2;
        if(pid == -1){
            printf("Erro na criacao de processo");
            exit(-1);
        }
        if(pid > 0 ){
            waitpid(pid, &statusB, 0);
            if(WIFEXITED(statusB)){
                for(i=half;i<ARRAY_SIZE; i++){
                    result[i]=((int) vec_values[i]/max_value)*100;
                    printf("result[%d]=%d  (parent)\n", i,result[i]);
                }
            }
        }
        if(pid == 0){
            for(i=0;i<half; i++){
                result[i]=((int) vec_values[i]/max_value)*100;
                printf("result[%d]=%d  (children)\n", i,result[i]);
            }
        }

    }
    return 0;
}