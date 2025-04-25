/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
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
#include "keytoktab.h" /* when the keytoktab is added   */
#include "lexer.h"     /* when the lexer     is added   */
#include "optab.h"     /* when the optab     is added   */
#include "symtab.h"    /* when the symtab    is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 0
#define WIDTH 56

static int lookahead   = 0;
static int is_parse_ok = 1;

static void   prog();
static void   prog_header();
static void   var_part();
static void   var_dec_list();
static void   var_dec();
static void   id_list();
static void   type();
static void   stat_part();
static void   stat_list();
static void   stat();
static void   assign_stat();
static void   match(int t);
static toktyp expr();
static toktyp term();
static toktyp factor();
static toktyp operand();

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
static void in(char *s) {
    if (DEBUG)
        printf("\n *** In  %s", s);
}
static void out(char *s) {
    if (DEBUG)
        printf("\n *** Out %s", s);
}

static void p_leftovers() {
    printf("    ");
    while (lookahead != '$') {
        printf("%s ", get_lexeme());
        match(lookahead);
    }
    printf("\n");
}
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
/* The Parser functions                                               */
/**********************************************************************/
static void match(int t) {
    if (DEBUG)
        printf("\n --------In match expected: %s found: %s", tok2lex(t),
               tok2lex(lookahead));
    if (lookahead == t)
        lookahead = get_token();
    else {
        printf("SYNTAX:   Symbol expected %s found %s\n", tok2lex(t),
               get_lexeme());
        is_parse_ok = 0;
    }
}

/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
static void prog() {
    in("prog");
    prog_header();
    var_part();
    stat_part();
    out("prog");
}
static void prog_header() {
    in("program_header");
    match(program);
    if (lex2tok(get_lexeme()) == id)
        addp_name(get_lexeme());
    else
        addp_name("???");
    match(id);
    match('(');
    match(input);
    match(',');
    match(output);
    match(')');
    match(';');
    out("program_header");
}
static void var_part() {
    in("var_part");
    match(var);
    var_dec_list();
    out("var_part");
}
static void var_dec_list() {
    in("var_dec_list");
    var_dec();
    if (lookahead == id)
        var_dec_list();
    out("var_dec_list");
}
static void var_dec() {
    in("var_dec");
    id_list();
    match(':');
    type();
    match(';');
    out("var_dec");
}
static void id_list() {
    in("id_list");
    if (lookahead == id) {
        if (!find_name(get_lexeme())) {
            addv_name(get_lexeme());
        } else {
            is_parse_ok = 0;
            printf("SEMANTIC: ID already declared: %s\n", get_lexeme());
        }
    }
    match(id);
    if (lookahead == ',') {
        match(',');
        id_list();
    }
    out("id_list");
}
static void type() {
    in("type");
    toktyp type;
    if (lookahead == integer) {
        match(integer);
        type = integer;
    } else if (lookahead == real) {
        match(real);
        type = real;
    } else if (lookahead == boolean) {
        match(boolean);
        type = boolean;
    } else {
        type = error;
        printf("SYNTAX:   Type name expected found %s\n", get_lexeme());
    }
    setv_type(type);
    out("type");
}
static void stat_part() {
    in("start_part");
    match(begin);
    stat_list();
    match(end);
    match('.');
    out("start_part");
}
static void stat_list() {
    stat();
    if (lookahead == ';') {
        match(';');
        stat_list();
    }
}
static void stat() {
    in("stat");
    assign_stat();
    out("stat");
}
static void assign_stat() {
    in("assign_stat");
    toktyp dest_type = error, src_type = error;
    if (find_name(get_lexeme()))
        dest_type = get_ntype(get_lexeme());
    else if (lookahead == id) {
        is_parse_ok = 0;
        printf("SEMANTIC: ID NOT declared: %s\n", get_lexeme());
    }
    match(id);
    match(assign);
    src_type = expr();
    if (dest_type != src_type) {
        printf("SEMANTIC: Assign types: %s := %s\n", tok2lex(dest_type),
               tok2lex(src_type));
        is_parse_ok = 0;
    }
    out("assign_stat");
}
static toktyp expr() {
    toktyp type1, type2;
    in("expr");
    type1 = term();
    if (lookahead == '+') {
        match('+');
        type2 = expr();
        out("expr");
        return get_otype('+', type1, type2);
    }
    out("expr");
    return type1;
}
static toktyp term() {
    toktyp type1, type2;
    in("term");
    type1 = factor();
    if (lookahead == '*') {
        match('*');
        type2 = term();
        out("term");
        return get_otype('*', type1, type2);
    }
    return type1;
}
static toktyp factor() {
    toktyp type;
    in("factor");
    if (lookahead == '(') {
        match('(');
        type = expr();
        match(')');
    } else {
        type = operand();
    }
    out("factor");
    return type;
}
static toktyp operand() {
    toktyp type;
    in("operand");
    if (lookahead == id) {
        if (!find_name(get_lexeme())) {
            printf("SEMANTIC: ID NOT declared: %s\n", get_lexeme());
            is_parse_ok = 0;
        }
        type = get_ntype(get_lexeme());
        match(id);
    } else if (lookahead == number) {
        type = integer;
        match(number);
    } else {
        type        = error;
        is_parse_ok = 0;
        printf("SYNTAX:   Operand expected\n");
    }
    out("operand");
    return type;
}
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
int parser() {
    in("parser");
    lookahead = get_token(); // get the first token
    if (lookahead == '$') {
        p_line();
        printf("WARNING: Input file is empty\n");
        p_line();
        is_parse_ok = 0;
    }
    if (is_parse_ok)
        prog();
    if (lookahead != '$') {
        printf("SYNTAX:   Extra symbols after end of parse!\n");
        p_leftovers();
        is_parse_ok = 0;
    }
    p_line();
    p_symtab();
    out("parser");
    return is_parse_ok; // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
