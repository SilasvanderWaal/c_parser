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
#define PROGRAMADDR 0
#define INTSIZE     4
#define REALSIZE    8
#define BOOLSIZE    4
#define WIDTH       56

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
/* Print a _ line                                                     */
/**********************************************************************/
static void p_line() {
    for (int i = 0; i < WIDTH; i++) {
        printf("_");
    }
    printf("\n");
}
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
    printf("%11s", get_name(ftref));
    printf("%11s", tok2lex(get_role(ftref)));
    printf("%11s", tok2lex(get_type(ftref)));
    printf("%10d", get_size(ftref));
    printf("%10d \n", get_addr(ftref));
}

void p_symtab() {
    p_line();
    printf(" THE SYMBOL TABLE \n");
    p_line();
    printf("%11s%11s%11s%10s%10s \n", "NAME", "ROLE", "TYPE", "SIZE", "ADDR");
    p_line();
    for (int i = 0; i < numrows; i++) {
        p_symrow(i);
    }
    p_line();
    printf(" STATIC STORAGE REQUIRED is %d BYTES\n", get_size(startp));
    p_line();
}

/**********************************************************************/
/*  Add a program name to the symbol table                            */
/**********************************************************************/
void addp_name(char *fpname) {
    if (get_ref(fpname) == nfound)
        addrow(fpname, program, program, startp, PROGRAMADDR);
}

/**********************************************************************/
/*  Add a variable name to the symbol table                           */
/**********************************************************************/
void addv_name(char *fpname) { addrow(fpname, var, undef, 0, 0); }

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
    for (int i = startp + 1; i < numrows; i++) {
        if (get_type(i) == undef) {
            set_type(i, ftype);
            set_size(i, map_size(ftype));
            set_addr(i,
                     get_addr(i - 1) + (get_size(i - 1) * (i > (startp + 1))));
        }
    }
    int last_index = numrows - 1;
    set_size(startp, get_addr(last_index) + get_size(last_index));
}

/**********************************************************************/
/*  Get the type of a variable from the symbol table                  */
/**********************************************************************/
toktyp get_ntype(char *fpname) { return get_type(get_ref(fpname)); }
/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
