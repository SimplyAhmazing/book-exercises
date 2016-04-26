#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{
    printf("hello world, (pid:%d)\n", (int) getpid());

    int rc = fork();

    if (rc < 0) { // fork failed exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // child (process)
        close(STDOUT_FILENO); // close the std out file descriptor
        open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

        // now exec "wc"

        char *myargs[3];
        myargs[0] = strdup("wc"); // program: "wc" wor count
        myargs[1] = strdup("p3.c"); // argument: file to count
        myargs[2] = NULL; // marks end of array

        execvp(myargs[0], myargs);
        printf("this should not print");
    } else { // parent goes down this path
        int wc = wait(NULL);
    }

    return 0;
}
