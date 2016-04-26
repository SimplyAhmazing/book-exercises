#include <stdio.h>
#include <stdlib.h>


#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/* Fake readline ftn */
void readline(char* prompt) {
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char *cpy = malloc(strlen(buffer)+1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy)-1] = '\0';
    return cpy;
}

void add_history(char* unused) {}

#else

#include <editline/readline.h>

#endif



int main(int argc, char** argv) {
    /* Print version and exit information */
    puts("Lispy version 0.0.1");
    puts("Press ctrl-C to exit");

    while (1) {
        char* input = readline("lispy> ");  // Get input

        add_history(input); // Allows for up/down arrow history

        /* echo input back to user */
        printf("You typed: %s\n", input);
    }
    return 0;
}
