#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "user.h"

#define READ 0
#define WRITE 1

#define CODE_SUCCESS 0
#define CODE_ERROR 1
#define CODE_NOT_EXISTS 2

int main()
{
    User test;
    User user;
    int state = CODE_NOT_EXISTS;
    strcpy(test.username, "user");
    strcpy(test.password, "pw");

    scanf("%s", user.username);
    scanf("%s", user.password);

    if (strcmp(user.username, test.username) == 0)
    {
        state = CODE_ERROR;
        if (strcmp(user.password, test.password) == 0)
        {
            state = CODE_SUCCESS;
        }
    }
    
    return state;
}