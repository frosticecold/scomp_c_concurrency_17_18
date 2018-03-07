#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 2048
int main()
{
    char buf[BUF_SIZE];
    int fd[2];
    if (pipe(fd) == -1)
    {
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if(pid == -1 ){
        return EXIT_FAILURE;
    }
    else{
        if(pid == 0){
            close(fd[1]);
            read(fd[0],(void*)buf,BUF_SIZE);
        }else{
            FILE *txtfile;
            txtfile = fopen("./text.txt","r+");

        }
    }

    return 0;
}