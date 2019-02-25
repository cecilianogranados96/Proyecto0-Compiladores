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
    if ( !(t == next_token()) ) sintax_error(t);
    flagToken = 0;
}

TOKEN next_token()
{
    if ( !flagToken )
    {
        token = scanner();
        //printf("\t\t TOKEN: %d \n",token);
        if ( token == LEXICALERROR ) lexical_error(token);
        flagToken = 1;
        if ( token == ID ) lookup(token_buffer, &token);
    }
    return token;
}

void lexical_error(int error)
{
    printf("\t Lexical Error %d \n", error);
    fprintf(out, "Lexical Error\n");
}

void sintax_error(TOKEN error)
{
    char * mensaje;
	switch(error){
		case BEGIN: strcpy(mensaje,"Error de sintaxis: BEGIN"); break;
		case END:  strcpy(mensaje,"Error de sintaxis: END"); break;
		case READ:  strcpy(mensaje,"Error de sintaxis: READ"); break;
		case WRITE:  strcpy(mensaje,"Error de sintaxis: WRITE"); break;
		case ID:  strcpy(mensaje,"Error de sintaxis: ID"); break;
		case INTLITERAL:  strcpy(mensaje,"Error de sintaxis: INTLITERAL"); break;
		case LPAREN:  strcpy(mensaje,"Error de sintaxis: LPAREN"); break;
		case RPAREN:  strcpy(mensaje,"Error de sintaxis: RPAREN"); break;
		case SEMICOLON:  strcpy(mensaje,"Error de sintaxis: SEMICOLON"); break;
		case COMMA:  strcpy(mensaje,"Error de sintaxis: COMMA"); break;
		case ASSIGNOP:  strcpy(mensaje,"Error de sintaxis: ASSIGNOP"); break;
		case PLUSSOP:  strcpy(mensaje,"Error de sintaxis: PLUSOP"); break;
		case MINUSOP:  strcpy(mensaje,"Error de sintaxis: MINUSOP"); break;
		case SCANEOF:  strcpy(mensaje,"Error de sintaxis: SCANEOF"); break;
    case PIPE:  strcpy(mensaje,"Error de sintaxis: PIPE"); break;

		default: break;
	}

    printf("%s \n", mensaje);
    fprintf(out, "%s \n", mensaje);
}

void generate(char * accion, char * a, char * b, char * c)
{

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
    int i = 5;
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
