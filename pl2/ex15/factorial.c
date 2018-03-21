#include <stdio.h>
#include <stdlib.h>

int calculateFactorial(int n)
{
    if (n <= 0)
    {
        return 1;
    }
    return (n * calculateFactorial(n - 1));
}

int main(){
    int num;
    scanf("%d",&num);
    num =calculateFactorial(num);
    printf("%d",num);
    return 0;
}