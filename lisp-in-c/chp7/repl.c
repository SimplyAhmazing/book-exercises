#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include <editline/readline.h>

#include "mpc.h"


long eval_op(long x, char* op, long y) {
    /* Multichar operators */
    if (strcmp(op, "min")) {
        if (x > y) {
            return x;
        } else {
            return y;
        }
    }

    if (strcmp(op, "max")) {
        if (x < y) {
            return x;
        } else {
            return y;
        }
    }

    /* Single char operators */
    switch (op[0]) {
        case '+':
            return x + y;
            break;
        case '-':
            return x - y;
            break;
        case '*':
            return x * y;
            break;
        case '/':
            return x / y;
            break;
        case '^':
            puts("here");
            return pow(x, y);
            break;
        case '%':
            return x % y;
            break;
        default:
            printf("Unknown operator: %s", op);
            exit(1);
    }
    return 1;
}


long eval(mpc_ast_t* t) {
    /* If tagged as a number return it directly */
    if (strstr(t->tag, "number")) {
        return atoi(t->contents);
    }

    /* Operator is always a 2nd child */
    char* op = t->children[1]->contents;

    long x = eval(t->children[2]);

    int i = 3;
    while (strstr(t->children[i]->tag, "expr")) {
        x = eval_op(x, op, eval(t->children[i]));
        i++;
    }

    return x;
}


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
        operator : '+' | '-' | '*' | '/' | \
                   /min/ | /max/ | '%' | '\\^' ; \
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
            long res = eval(r.output);
            printf("%li\n", res);
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
