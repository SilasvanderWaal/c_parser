/**********************************************************************/
/* constants & public functions                                       */
/**********************************************************************/

#ifndef KEYTOK_H
#define KEYTOK_H

/**********************************************************************/
/* constants                                                          */
/**********************************************************************/
#define nfound -1
#define WIDTH  56

typedef enum tvalues {
    tstart = 257,
    id,
    number,
    assign,
    predef,
    tempty,
    undef,
    error,
    typ,
    tend,
    kstart,
    program,
    input,
    output,
    var,
    begin,
    end,
    boolean,
    integer,
    real,
    kend
} toktyp;

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

void   p_toktab();           /* display token/keyword tables    */
toktyp lex2tok(char *fplex); /* convert a lexeme  to a token    */
toktyp key2tok(char *fplex); /* convert a keyword to a token    */
char  *tok2lex(toktyp ftok); /* convert a token   to a lexeme   */
void   p_line(); /* print a _ line out on the scrren, width defined by WIDTH
                    constant */

#endif

/**********************************************************************/
/* end of definitions                                                 */
/**********************************************************************/
