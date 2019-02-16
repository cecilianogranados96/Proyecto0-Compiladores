#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "headers/fAuxs.h"
#include "headers/scanner.h"

extern FILE * out;
extern RegTS TS[];
extern char token_buffer[];
extern TOKEN token;
extern int flagToken;

/* -------------------  FUNCIONES AUXILIARES  ------------------- */
void Match(TOKEN t)
{
    if ( !(t == next_token()) ) sintax_error();
    flagToken = 0;
}

TOKEN next_token()
{
    if ( !flagToken )
    {
        token = scanner();
        if ( token == LEXICALERROR ) lexical_error();
        flagToken = 1;
        if ( token == ID ) lookup(token_buffer, &token);
    }
    return token;
}

void lexical_error()
{
    printf("\t Lexical Error\n");
    fprintf(out, "Lexical Error\n");
    
}
void lexical_error2(char c)
{
    printf("\t Lexical Error %d \n",c);
    fprintf(out, "Lexical Error\n");
    
}

void sintax_error()
{
    printf("\t Syntax Error\n");
    fprintf(out, "Syntax Error\n");
}

void generate(char * accion, char * a, char * b, char * c)
{
    /* Produce la salida de la instruccion para la MV por stdout */
    printf("%s %s%c%s%c%s\n", accion, a, ',', b, ',', c);
    fprintf(out,"%s %s%c%s%c%s\n", accion, a, ',', b, ',', c );
}

char *  extract(REG_EXPRESION * preg)
{
    /* Retorna la cadena del registro semantico */
    return preg->name;
}

int lookup(char * id, TOKEN * t)
{
    /* Determina si un identificador esta en la TS */
    int i = 0;
    while ( strcmp("$", TS[i].lexema_identifier) )
    {
        if ( !strcmp(id, TS[i].lexema_identifier) )
        {
            *t = TS[i].t;
            return 1;
        }
        i++;
    }
    return 0;
}

void enter(char * id)
{
    /* Agrega un identificador a la TS */
    int i = 4;
    while ( strcmp("$", TS[i].lexema_identifier) ) i++;
    {
        if ( i < 999 )
        {
            strcpy(TS[i].lexema_identifier, id );
            TS[i].t = ID;
            strcpy(TS[++i].lexema_identifier, "$" );
        }
    }
}

void check_id(char * s)
{
    /* Si la cadena No esta en la Tabla de Simbolos la agrega,y si es el name de una variable genera la instruccion */
    TOKEN t;
    if ( !lookup(s, &t) )
    {
        enter(s);
        generate("Declare", s, "Integer", "");
    }
}

void start(void)
{
    /* Inicializaciones Semanticas */
    
}

void finish(void)
{
    /* Genera la instruccion para terminar la ejecucion del programa */
    generate("Halt", "", "", "");
}

void assign(REG_EXPRESION izq, REG_EXPRESION der)
{
    /* Genera la instruccion para la asignacion */
    generate("Store",  extract(&der), izq.name, "");
}
