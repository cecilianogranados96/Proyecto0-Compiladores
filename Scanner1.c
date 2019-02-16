#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "headers/scanner.h"

extern FILE * in;
extern char token_buffer[];


/* -------------------  SCANNER ------------------- */
TOKEN scanner()
{
    int tabla[rowTS][ColumnsTS] = {    {  1,  3,  5,  6,  7,  8,  9, 10, 11, 14, 13,  0, 14 },
                                       {  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2 },
                                       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
                                       {  4,  3,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4 },
                                       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
                                       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
                                       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
                                       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
                                       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
                                       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
                                       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
                                       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 12, 14, 14, 14 },
                                       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
                                       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },
                                       { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 } };
    int character;
    int col;
    int state = 0;
    int i = 0;
    do
    {
        character = fgetc(in);
        col = colTS(character);
        state = tabla[state][col];
        if ( col != 11 )
        {
            token_buffer[i] = character;
            i++;
        }
    }while ( !stateFinal(state) && !(state == 14) );

    token_buffer[i] = '\0';
    switch ( state )
    {
        case 2 :
            if ( col != 11 )
            {
            ungetc(character,in);
            token_buffer[i-1] = '\0';
            }
            return ID;
        case 4 :
            if ( col != 11 )
            {
            ungetc(character, in);
            token_buffer[i-1] = '\0';
            }
            return INTLITERAL;
        case 5 : return PLUSSOP;
        case 6 : return MINUSOP;
        case 7 : return LPAREN;
        case 8 : return RPAREN;
        case 9 : return COMMA;
        case 10 : return SEMICOLON;
        case 12 : return ASSIGNOP;
        case 13 : return FDT;
        case 14 : return LEXICALERROR;
    }
    return 0;
}

int stateFinal(int e)
{
    if ( e == 0 || e == 1 || e == 3 || e == 11 || e == 14 ) return 0;
    return 1;
}

int colTS(int c)
{
    if ( isalpha(c) ) return 0;
    if ( isdigit(c) ) return 1;
    if ( c == '+' ) return 2;
    if ( c == '-' ) return 3;
    if ( c == '(' ) return 4;
    if ( c == ')' ) return 5;
    if ( c == ',' ) return 6;
    if ( c == ';' ) return 7;
    if ( c == ':' ) return 8;
    if ( c == '=' ) return 9;
    if ( c == EOF ) return 10;
    if ( isspace(c) ) return 11;
    return 12;
}