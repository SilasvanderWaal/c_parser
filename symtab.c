/**********************************************************************/
/* lab 1 DVG C01 - Symbol Table OBJECT                                */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define TABSIZE     1024 /* symbol table size           */
#define NAMELEN     20   /* name length                 */
#define PROGRAMSIZE 32
#define PROGRAMADDR 0
#define INTSIZE     4
#define REALSIZE    8
#define BOOLSIZE    4
#define WIDTH       50

typedef char tname[NAMELEN];

static tname  name[TABSIZE];
static toktyp role[TABSIZE];
static toktyp type[TABSIZE];
static int    size[TABSIZE];
static int    addr[TABSIZE];

static int numrows = 0; /* number of rows in the ST    */
static int startp  = 0; /* start position program in ST*/

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/*  GET methods (one for each attribute)                              */
/**********************************************************************/
static char  *get_name(int ftref) { return name[ftref]; }
static toktyp get_role(int ftref) { return role[ftref]; }
static toktyp get_type(int ftref) { return type[ftref]; }
static int    get_size(int ftref) { return size[ftref]; }
static int    get_addr(int ftref) { return addr[ftref]; }

/**********************************************************************/
/*  SET methods (one for each attribute)                              */
/**********************************************************************/
static void set_name(int ftref, char *fpname) { strcpy(name[ftref], fpname); }
static void set_role(int ftref, toktyp frole) { role[ftref] = frole; }
static void set_type(int ftref, toktyp ftype) { type[ftref] = ftype; }
static void set_size(int ftref, int fsize) { size[ftref] = fsize; }
static void set_addr(int ftref, int faddr) { addr[ftref] = faddr; }

/**********************************************************************/
/*  Add a row to the symbol table                                     */
/**********************************************************************/
static void addrow(char *fname, toktyp frole, toktyp ftype, int fsize,
                   int faddr) {
    set_name(numrows, fname);
    set_role(numrows, frole);
    set_type(numrows, ftype);
    set_size(numrows, fsize);
    set_addr(numrows, faddr);
    numrows++;
}
/**********************************************************************/
/*  Initialise the symbol table                                       */
/**********************************************************************/
static void initst() {
    addrow(tok2lex(predef), typ, predef, 0, 0);
    addrow(tok2lex(undef), typ, predef, 0, 0);
    addrow(tok2lex(error), typ, predef, 0, 0);
    addrow(tok2lex(integer), typ, predef, 4, 0);
    addrow(tok2lex(boolean), typ, predef, 4, 0);
    addrow(tok2lex(real), typ, predef, 8, 0);
}
/**********************************************************************/
/*  return a reference to the ST (index) if name found else nfound    */
/**********************************************************************/
static int get_ref(char *fpname) {
    for (int i = 0; i < numrows; i++) {
        if (strcmp(fpname, get_name(i)) == 0)
            return i;
    }
    return nfound;
}

/**********************************************************************/
/*  Print out a - line in order to separate parts in the UI           */
/**********************************************************************/
static void p_line() {
    for (int i = 0; i < WIDTH; i++) {
        printf("-");
    }
    printf("\n");
}

/**********************************************************************/
/*  return the size of a type, if no size can be mapped return 0      */
/**********************************************************************/
static int map_size(toktyp type) {
    switch (type) {
    case integer:
        return INTSIZE;
    case real:
        return REALSIZE;
    case boolean:
        return BOOLSIZE;
    default:
        return 0;
    }
}
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/*  Display the symbol table                                          */
/**********************************************************************/
static void p_symrow(int ftref) {
    printf("%s\t", get_name(ftref));
    printf("%6s\t", tok2lex(get_role(ftref)));
    printf("%6s\t", tok2lex(get_type(ftref)));
    printf("%6d\t", get_size(ftref));
    printf("%6d\n", get_addr(ftref));
}

void p_symtab() {
    p_line();
    printf(" | SYMBOL TABLE |\n");
    p_line();
    printf("%6s\t %6s\t %6s\t %6s\t %6s\n", "NAME", "ROLE", "TYPE", "SIZE",
           "ADDR");
    p_line();
    for (int i = 0; i < numrows; i++) {
        p_symrow(i);
    }
    p_line();
}

/**********************************************************************/
/*  Add a program name to the symbol table                            */
/**********************************************************************/
void addp_name(char *fpname) {
    if (get_ref(fpname) == nfound)
        addrow(fpname, program, program, PROGRAMSIZE, PROGRAMADDR);
    else
        perror("Program name already exists!");
}

/**********************************************************************/
/*  Add a variable name to the symbol table                           */
/**********************************************************************/
void addv_name(char *fpname) {
    if (get_ref(fpname) == nfound)
        addrow(fpname, var, undef, 0, 0);
    else
        perror("Variable name already exists!");
}

/**********************************************************************/
/*  Find a name in the the symbol table                               */
/*  return a Boolean (true, false) if the name is in the ST           */
/**********************************************************************/
int find_name(char *fpname) {
    if ((get_ref(fpname)) == nfound) {
        return false;
    } else {
        return true;
    }
}

/**********************************************************************/
/*  Set the type of an id list in the symbol table                    */
/**********************************************************************/
void setv_type(toktyp ftype) {
    for (int i = 0; i < numrows; i++) {
        if (get_type(i) == undef) {
            set_type(i, ftype);
            set_size(i, map_size(ftype));
            set_addr(i, startp);
            startp += map_size(ftype);
        }
    }
}

/**********************************************************************/
/*  Get the type of a variable from the symbol table                  */
/**********************************************************************/
toktyp get_ntype(char *fpname) { return get_type(get_ref(fpname)); }

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
