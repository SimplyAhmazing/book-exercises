#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{

    int *x = NULL;
    printf("x is %p\n", x);

    free(x);
    printf("x is %p\n", x);
}
