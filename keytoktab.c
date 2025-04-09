/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */
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
/**********************************************************************/
/* type definitions                                                   */
/**********************************************************************/
typedef struct tab {
    char *text;
    int   token;
} tab;

/**********************************************************************/
/* data objects (tables)                                              */
/**********************************************************************/
static tab tokentab[] = {
    {    "id",     id},
    {"number", number},
    {    ":=", assign},
    { "undef",  undef},
    {"predef", predef},
    {"tempty", tempty},
    { "error",  error},
    {  "type",    typ},
    {     "$",    '$'},
    {     "(",    '('},
    {     ")",    ')'},
    {     "*",    '*'},
    {     "+",    '+'},
    {     ",",    ','},
    {     "-",    '-'},
    {     ".",    '.'},
    {     "/",    '/'},
    {     ":",    ':'},
    {     ";",    ';'},
    {     "=",    '='},
    {"TERROR", nfound}
};

static tab keywordtab[] = {
    {"program", program},
    {  "input",   input},
    { "output",  output},
    {    "var",     var},
    {  "begin",   begin},
    {    "end",     end},
    {"boolean", boolean},
    {"integer", integer},
    {   "real",    real},
    { "KERROR",  nfound}
};

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Display the tables                                                 */
/**********************************************************************/
void p_toktab() {
    int toktab_length = sizeof(keywordtab) / sizeof(tab);
    printf("--------------------\nToken Table\n--------------------\n");
    for (int i = 0; i < toktab_length; i++) {
        printf("Lexum:%s \t Token: %d \n", keywordtab[i].text,
               keywordtab[i].token);
    }
    printf("\n--------------------\nKeyword Table\n--------------------\n");
    for (int i = 0; i < toktab_length; i++) {
        printf("Lexum:%s \t Keyword: %d \n", keywordtab[i].text,
               keywordtab[i].token);
    }
}

/**********************************************************************/
/* lex2tok - convert a lexeme to a token                              */
/**********************************************************************/
toktyp lex2tok(char *fplex) {
    int toktab_length = sizeof(tokentab) / sizeof(tab);
    for (int i = 0; i < toktab_length; i++) {
        if (strcmp(fplex, tokentab[i].text) == 0)
            return tokentab[i].token;
    }

    return key2tok(fplex);
}

/**********************************************************************/
/* key2tok - convert a keyword to a token                             */
/**********************************************************************/
toktyp key2tok(char *fplex) {
    int keyword_length = sizeof(keywordtab) / sizeof(tab);
    for (int i = 0; i < keyword_length; i++) {
        if (strcmp(keywordtab[i].text, fplex) == 0)
            return keywordtab[i].token;
    }
    return id;
}

/**********************************************************************/
/* tok2lex - convert a token to a lexeme                              */
/**********************************************************************/
char *tok2lex(toktyp ftok) {
    int toktab_length  = sizeof(tokentab) / sizeof(tab);
    int keyword_length = sizeof(keywordtab) / sizeof(tab);
    for (int i = 0; i < toktab_length; i++) {
        if (ftok == tokentab[i].token)
            return tokentab[i].text;
    }
    for (int i = 0; i < keyword_length; i++) {
        if (keywordtab[i].token == ftok)
            return keywordtab[i].text;
    }
    return "KERROR";
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
