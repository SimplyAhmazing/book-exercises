#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>

#include "mpc.h"


int main(int argc, char** argv) {
    /* Print version and exit information */
    puts("Lispy version 0.0.1");
    puts("Press ctrl-C to exit");

    /* Build a parser adjective to recognize descriptions */
    mpc_parser_t* Number   = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr     = mpc_new("expr");
    mpc_parser_t* Lispy    = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
        " \
        number   : /-?[0-9]+/ ; \
        operator : '+' | '-' | '*' | '/' ; \
        expr     : <number> | '(' <operator> <expr>+ ')'; \
        lispy    : /^/ <operator> <expr>+ /$/ ; \
        ",
        Number, Operator, Expr, Lispy);


    while (1) {
        char* input = readline("lispy> ");  // Get input

        add_history(input); // Allows for up/down arrow history

        mpc_result_t r;

        if (mpc_parse("<stdin>", input, Lispy, &r)) {
            /* on Success print the ast */
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        } else {
            /* Otherwise print the error */
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);
    }

    /* Do some parsing here */

    /* Undefine and delete our Parsers */
    mpc_cleanup(4, Number, Operator, Expr, Lispy);

    return 0;
}
