/**********************************************************************/
/* lab 1 DVG C01 - Operator Table OBJECT                              */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define NENTS 4
#define WIDTH 56

static int optab[][NENTS] = {
    {'+', integer, integer, integer},
    {'+',    real,    real,    real},
    {'+', integer,    real,    real},
    {'+',    real, integer,    real},
    {'*', integer, integer, integer},
    {'*',    real,    real,    real},
    {'*', integer,    real,    real},
    {'*',    real, integer,    real},
    {'$',   undef,   undef,   undef}
};

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
static void p_line() {
    for (int i = 0; i < WIDTH; i++) {
        printf("_");
    }
    printf("\n");
}
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* display the op tab                                                 */
/**********************************************************************/
void p_optab() {
    p_line();
    printf(" THE OPERATOR TABLE\n");
    p_line();
    printf("%10s %10s %10s %10s \n", "operator", "arg1", "arg2", "result");
    p_line();
    for (int i = 0; optab[i][0] != '$'; i++) {
        printf("%10c, %10s, %10s, %10s \n", optab[i][0], tok2lex(optab[i][1]),
               tok2lex(optab[i][2]), tok2lex(optab[i][3]));
    }
    p_line();
}

/**********************************************************************/
/* return the type of a binary expression op arg1 arg2                */
/**********************************************************************/
int get_otype(int op, int arg1, int arg2) {
    for (int i = 0; (optab[i][0] != '$'); i++) {
        if (optab[i][0] == op && optab[i][1] == arg1 && optab[i][2] == arg2) {
            return optab[i][3];
        }
    }
    return undef;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
