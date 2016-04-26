#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include <editline/readline.h>

#include "mpc.h"


typedef struct lval {
    int type;
    long num;

    /* Error and symbol types are string data */
    char* err;
    char* sym;

    /* Count and pointer to a list of "lval" */
    int count;
    struct lval** cell;
} lval;  // Lisp Value


/* enumeration for lval types, under the hood these enums
 * have integer constants
 */
enum {
    LVAL_ERR,
    LVAL_NUM,
    LVAL_SEXPR,
    LVAL_SYM
};


/* Construct a pointer to a new Number lval */
lval* lval_num(long x) {
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_NUM;
    v->num = x;
    return v;
}

/* Construct a pointer to a new Error lval */
lval* lval_err(char* m) {
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_ERR;
    v->err = malloc(strlen(m)+1);
    strcpy(v->err, m);
    return v;
}

/* Construct a pointer to a new Error lval */
lval* lval_sym(char* s) {
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_SYM;
    v->sym = malloc(strlen(s)+1);
    strcpy(v->sym, s);
    return v;
}

/* Construct a pointer to a new Error lval */
lval* lval_sexpr(void) {
    lval* v = malloc(sizeof(lval));
    v->type = LVAL_SEXPR;
    v->count = 0;
    v->cell = NULL;
    return v;
}

/* Delete an lval and free its memory allocations */
void lval_del(lval* v) {
    switch (v->type) {
        // Do nothing for number type
        case LVAL_NUM:
            break;
        case LVAL_ERR:
            free(v->err);
            break;
        case LVAL_SYM:
            free(v->sym);
            break;
        case LVAL_SEXPR:
            for (int i = 0; i < v->count; i++) {
                lval_del(v->cell[i]);
            }
            /* Also free the memory allocations containing the pointer */
            free(v->cell);
            break;
    }
    /* Free the memory allocation for the lval struct itself */
    free(v);
}

lval* lval_read_num(mpc_ast_t* t) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE
        ? lval_num(x)
        : lval_err("invalid number");
}

lval* lval_add(lval* v, lval* x) {
    v->count++;
    v->cell = realloc(v->cell, sizeof(lval*) * v->count);
    v->cell[v->count-1] = x;
    return v;
}

lval* lval_read(mpc_ast_t* t) {
    /* If symbol or number return conversion to that type */
    if (strstr(t->tag, "number") == 0) { return lval_read_num(t); }
    if (strstr(t->tag, "symbol") == 0) { return lval_sym(t->contents); }

    /* If root (>) or S-expr then create an empty list */
    lval* x = NULL;
    if (strstr(t->tag, ">") == 0) { x = lval_sexpr(); }
    if (strstr(t->tag, "sexpr") == 0) { x = lval_sexpr(); }

    /* Fill this list with any valid expression contained within */
    for (int i = 0; i < t->children_num; i++) {
        if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
        if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
        if (strcmp(t->children[i]->contents, "{") == 0) { continue; }
        if (strcmp(t->children[i]->contents, "}") == 0) { continue; }
        if (strcmp(t->children[i]->tag, "regex") == 0) { continue; }
        x = lval_add(x, lval_read(t->children[i]));
    }
    return x;
}

void lval_print(lval* v);  // Function signature for forward referencing

void lval_expr_print(lval* v, char open, char close) {
    putchar(open);
    for (int i = 0; i < v->count; i++) {
        // Print value contained w/in
        lval_print(v->cell[i]);

        // Don't print trailing space if last element
        if (i != (v->count-1)) {
            putchar(' ');
        }
    }
    putchar(close);
}

/* print an lval */
void lval_print(lval* v) {
    switch (v->type) {
        case LVAL_NUM:
            printf("%li", v->num);
            break;
        /* In the case the type is an error */
        case LVAL_ERR:
            printf("Error: %s", v->err);
            break;
        case LVAL_SYM:
            printf("%s", v->sym);
            break;
        case LVAL_SEXPR:
            lval_expr_print(v, '(', ')');
            break;
    }
}


/* print an lval followed by a new line */
void lval_println(lval* v) { lval_print(v); putchar('\n'); }


/* lval* eval_op(lval* x, char* op, lval* y) { */
/*  */
/*     if (x->type == LVAL_ERR) return x; */
/*     if (y->type == LVAL_ERR) return y; */
/*  */
/*     #<{(| Multichar operators |)}># */
/*     if (strcmp(op, "min") == 0) { */
/*         return x->num < y->num ? x : y; */
/*     } */
/*  */
/*     if (strcmp(op, "max") == 0) { */
/*         return x->num > y->num ? x : y; */
/*     } */
/*  */
/*     #<{(| Single char operators |)}># */
/*     switch (op[0]) { */
/*         case '+': */
/*             return lval_num(x->num + y->num); */
/*             break; */
/*         case '-': */
/*             return lval_num(x->num - y->num); */
/*             break; */
/*         case '*': */
/*             return lval_num(x->num * y->num); */
/*             break; */
/*         case '/': */
/*             return y->num == 0 */
/*                 ? lval_err("division by zero") */
/*                 : lval_num(x->num / y->num); */
/*             break; */
/*         case '^': */
/*             puts("here"); */
/*             return lval_num(pow(x->num, y->num)); */
/*             break; */
/*         case '%': */
/*             return lval_num(x->num % y->num); */
/*             break; */
/*         default: */
/*             printf("Unknown operator: %s", op); */
/*             exit(1); */
/*     } */
/*     return lval_err("Invalid number"); */
/* } */


/* lval* eval(mpc_ast_t* t) { */
/*     #<{(| If tagged as a number return it directly |)}># */
/*     if (strstr(t->tag, "number")) { */
/*         // Check for an error in conversion */
/*         errno = 0; */
/*         long x = strtol(t->contents, NULL, 10); */
/*         return errno != ERANGE ? lval_num(x) : lval_err("bad number"); */
/*     } */
/*  */
/*     #<{(| Operator is always a 2nd child |)}># */
/*     char* op = t->children[1]->contents; */
/*  */
/*     lval* x = eval(t->children[2]); */
/*  */
/*     int i = 3; */
/*     while (strstr(t->children[i]->tag, "expr")) { */
/*         x = eval_op(x, op, eval(t->children[i])); */
/*         i++; */
/*     } */
/*  */
/*     return x; */
/* } */


int main(int argc, char** argv) {
    /* Print version and exit information */
    puts("Lispy version 0.0.1");
    puts("Press ctrl-C to exit");

    /* Build a parser adjective to recognize descriptions */
    mpc_parser_t* Number   = mpc_new("number");
    mpc_parser_t* Symbol = mpc_new("symbol");
    mpc_parser_t* Sexpr     = mpc_new("sexpr");
    mpc_parser_t* Expr     = mpc_new("expr");
    mpc_parser_t* Lispy    = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                 \
        number   : /-?[0-9]+/ ;                           \
        symbol : '+' | '-' | '*' | '/' |                  \
                   /min/ | /max/ | '%' | /^/ ;            \
        sexpr     : '(' <expr>* ')';                      \
        expr     : <number> | <operator> | <sexpr> ;      \
        lispy    : /^/ <expr>* /$/ ;                      \
        ",
        Number, Symbol, Sexpr, Expr, Lispy);


    while (1) {
        char* input = readline("lispy> ");  // Get input

        add_history(input); // Allows for up/down arrow history

        mpc_result_t r;

        if (mpc_parse("<stdin>", input, Lispy, &r)) {
            /* on Success print the ast */
            // lval result = eval(r.output);
            // lval_println(result);
            // mpc_ast_delete(r.output);
            lval* x = lval_read(r.output);
            lval_print(x);
            lval_del(x);
        } else {
            /* Otherwise print the error */
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);
    }

    /* Do some parsing here */

    /* Undefine and delete our Parsers */
    mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Lispy);

    return 0;
}
