#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include <editline/readline.h>

#include "mpc.h"


typedef struct {
    int type;
    long num;
    int err;
} lval;  // Lisp Value


/* enumeration for lval types, under the hood these enums
 * have integer constants
 */
enum {
    LVAL_NUM,
    LVAL_ERR
};


/* enumberation for possible error types */
enum {
    LERR_DIV_ZERO,
    LERR_BAD_OP,
    LERR_BAD_NUM
};


lval lval_num(long x) {
    lval v;
    v.type = LVAL_NUM;
    v.num = x;
    return v;
}


lval lval_err(int x) {
    lval v;
    v.type = LVAL_ERR;
    v.err = x;
    return v;
}


/* print an lval */
void lval_print(lval v) {
    switch (v.type) {
        case LVAL_NUM:
            printf("%li", v.num);
            break;

        /* In the case the type is an error */
        case LVAL_ERR:
            if (v.err == LERR_DIV_ZERO) {
                printf("Error: Division by zero");
            }
            if (v.err == LERR_BAD_NUM) {
                printf("Error: Invalid Number!");
            }
            if (v.err == LERR_BAD_OP) {
                printf("Error: Invalid Operator");
            }
            break;
    }
}

/* print an lval followed by a new line */
void lval_println(lval v) { lval_print(v); putchar('\n'); }


lval eval_op(lval x, char* op, lval y) {

    if (x.type == LVAL_ERR) return x;
    if (y.type == LVAL_ERR) return y;

    /* Multichar operators */
    if (strcmp(op, "min") == 0) {
        return x.num < y.num ? x : y;
    }

    if (strcmp(op, "max") == 0) {
        return x.num > y.num ? x : y;
    }

    /* Single char operators */
    switch (op[0]) {
        case '+':
            return lval_num(x.num + y.num);
            break;
        case '-':
            return lval_num(x.num - y.num);
            break;
        case '*':
            return lval_num(x.num * y.num);
            break;
        case '/':
            return y.num == 0
                ? lval_err(LERR_DIV_ZERO)
                : lval_num(x.num / y.num);
            break;
        case '^':
            puts("here");
            return lval_num(pow(x.num, y.num));
            break;
        case '%':
            return lval_num(x.num % y.num);
            break;
        default:
            printf("Unknown operator: %s", op);
            exit(1);
    }
    return lval_err(LERR_BAD_NUM);
}


lval eval(mpc_ast_t* t) {
    /* If tagged as a number return it directly */
    if (strstr(t->tag, "number")) {
        // Check for an error in conversion
        errno = 0;
        long x = strtol(t->contents, NULL, 10);
        return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
    }

    /* Operator is always a 2nd child */
    char* op = t->children[1]->contents;

    lval x = eval(t->children[2]);

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
            lval result = eval(r.output);
            lval_println(result);
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
