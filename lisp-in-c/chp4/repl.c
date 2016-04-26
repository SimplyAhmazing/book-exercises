#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
// #include <editline/history.h>

// static keyword makes this variable local to this file!
// global array of 2048 chars
// static char input[2048];


int main(int argc, char** argv) {
    /* Print version and exit information */
    puts("Lispy version 0.0.1");
    puts("Press ctrl-C to exit");

    while (1) {
        /* output our prompt */
        // fputs("lispy> ", stdout);
        /* Read a line of input with a max size of 2048 */
        // fgets(input, 2048, stdin);

        char* input = readline("lispy> ");  // Get input

        add_history(input); // Allows for up/down arrow history

        /* echo input back to user */
        printf("You typed: %s\n", input);
    }
    return 0;
}
