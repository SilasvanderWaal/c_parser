/**********************************************************************/
/* lab 1 DVG C01 - Lexer OBJECT                                       */
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
#define BUFSIZE 1024
#define LEXSIZE 30
static char buffer[BUFSIZE];
static char lexbuf[LEXSIZE];
static int  pbuf = 0; /* current index program buffer  */
static int  plex = 0; /* current index lexeme  buffer  */

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/* buffer functions                                                   */
/**********************************************************************/
/**********************************************************************/
/* Read the input file into the buffer                                */
/**********************************************************************/

static void get_prog() {
    char char_buff;
    while ((char_buff = fgetc(stdin)) != EOF) {
        if (pbuf < BUFSIZE - 1)
            buffer[pbuf++] = char_buff;
        else
            break;
    }
    buffer[pbuf] = '$';
}
/**********************************************************************/
/* Display the buffer                                                 */
/**********************************************************************/
static void pbuffer() {
    p_line();
    printf(" THE PROGRAM TEXT\n");
    p_line();
    printf("%s\n", buffer);
    p_line();
}

/**********************************************************************/
/* Copy a character from the program buffer to the lexeme buffer      */
/**********************************************************************/

static void get_char() { lexbuf[plex++] = buffer[pbuf++]; }

/**********************************************************************/
/* End of buffer handling functions                                   */
/**********************************************************************/

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Return a token                                                     */
/**********************************************************************/
int get_token() {
    static int start_flag = 1;
    plex                  = 0;
    memset(lexbuf, 0, LEXSIZE);

    if (start_flag == 1) {
        get_prog();
        pbuffer();
        start_flag = 0;
        pbuf       = 0;
    }

    while (isspace(buffer[pbuf]))
        pbuf++;

    if (isdigit(buffer[pbuf])) {
        while (isdigit(buffer[pbuf])) {
            get_char();
        }
        return number;
    } else if (isalpha(buffer[pbuf])) {
        while (isalnum(buffer[pbuf])) {
            get_char();
        }
        return key2tok(lexbuf);
    } else if (ispunct(buffer[pbuf])) {
        if (buffer[pbuf] == ':' && buffer[pbuf + 1] == '=') {
            get_char();
            get_char();
            return assign;
        } else {
            get_char();
        }
        return lex2tok(lexbuf);
    }
    return error;
}

/**********************************************************************/
/* Return a lexeme                                                    */
/**********************************************************************/
char *get_lexeme() { return lexbuf; }

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
