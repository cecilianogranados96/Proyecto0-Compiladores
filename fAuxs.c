#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "headers/fAuxs.h"
#include "headers/scanner.h"

extern FILE * out;
extern FILE * out_ensambler;
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
        
        
        //**********ENSAMBLADOR**********************************************************************
        char *message = " \n \tpush eax \n";
        fprintf(out_ensambler, "%s", message);
        //**********ENSAMBLADOR**********************************************************************
        
        
    }
}

void start(void)
{
    /* Inicializaciones Semanticas */
    
    //**********ENSAMBLADOR**********************************************************************
    char *messageData = "section     .data	  	  \n \
											      \n \
	;-------------------------------------------- \n \
	; En esta sección se declaran las variables.  \n \
	; Se reserva la memoria necesaria.			  \n \
	; Se declaran las constantes.				  \n \
	;-------------------------------------------  \n \
												  \n \
	global buffer_input   ; Buffer donde se almacenarán los valores.			\n \
	global len_input	  ; largo del buffer.									\n \
	global buffer_output  ; Buffer donde se escribirán los valores a imprimir.	\n \
	global len_output	  ; largo del buffer.									\n \
																				\n \
	buffer_input  db '*******************************', 0xa						\n \
	len_input      equ $ - buffer_input											\n \
	buffer_output db '*******************************', 0xa                     \n \
	len_output      equ $ - buffer_input 										\n \
	\n \
    ; -----------------------------------------------------------------------------------";
    fprintf(out_ensambler, "%s \n", messageData);
    char *messageText = "section     .text	  	\n \
											 \n \
	;----------------------------------------------- \n \
	; En esta sección se realizan las operaciones. 	\n \
	; Se escribe el código necesario para ejecutar.	\n \
	; Realiza los llamados adecuados.				\n \
	;---------------------------------------------- \n \
                                                    \n \
    extern buffer_input  	  	  \n \
    extern len_input			  \n \
    extern buffer_output		  \n \
    extern len_output			  \n \
    global      _start  		  \n \
                                  \n \
    ; Funciones con etiquetas para desplazarme por el archivo						\n \
    ; ///////////////////////////////////////////////////// \n \n \
    ; Funciones \n \
                                                    \n \
    ; # Atoi = edi -> contador; esi -> largo; eax -> número_final; \n \n \
    atoi:						\n \
        xor ecx, ecx   						; Limpio el buffer \n \
        mov cl, [buffer_input + esi]  	 						 \n \
        sub cl, '0'							; Convierto a decimal  \n \
        mul ebx 								; Multiplico por diez  \n \
        add eax, ecx 						; Agrego el caracter convertido \n \
        inc esi 							\n \
        cmp edi, esi 						; comparo si ya llegue al final \n \
        jne atoi							; siga  \n \
        ret     							 \n \n \n \
    itoa:  						\n 	\
        ; Me convierte a ASCII \n \n	\
        xor edx, edx						; Limpio el registro edx  \n 	\
        div ebx								; eax:dividendo y resultado, edx:residuo, ebx:divisor \n 	\
        add dl, '0'							; Aplique la conversion a ASCII   \n 	\
        mov [buffer_output + esi], dl 		; Mueva a resultado en la posicion (UM, C, D, U) \n 	\
        dec esi								; Decremento la actual posición. \n 	\
        cmp esi, 0							; Si ya llegue al inicio, ponga cero. \n 	\
        jne itoa  							; Si no realice de nuevo	\n 	\
                                            ; siga  \n \
        ret 								\n \n \n \
    _start:  	\n \n ; Code \n \
        xor eax, eax \n ";
	fprintf(out_ensambler, "%s \n", messageText);
    //**********ENSAMBLADOR**********************************************************************
    
}

void finish(void)
{
    /* Genera la instruccion para terminar la ejecucion del programa */
    generate("Halt", "", "", "");
    
    //**********ENSAMBLADOR**********************************************************************
    char *messageFinish = "; ---------------------------  \n  \
    mov eax, 1 \n \
    int 0x80 \n \
; ---------------------------  \n  ";
    fprintf(out_ensambler, "%s \n", messageFinish);
    //**********ENSAMBLADOR**********************************************************************    
}


void assign(REG_EXPRESION izq, REG_EXPRESION der)
{
    /* Genera la instruccion para la asignacion */
    generate("Store",  extract(&der), izq.name, "");
        
    //**********ENSAMBLADOR**********************************************************************
    char *message;
	if (der.clase == LITERALEXPR){
		message = "; ---------------------------  \n \
                    \t; movLiteral destino, origen L \n \
                    mov eax, %s         \n \
                    mov [esp + %s], ax  \n \
                    ; ---------------------------  \n ";
	}else{
		message = "; ---------------------------  \n \
                    \t; mov destino, origen \n \
                    mov eax, [esp + %s] \n \
                    mov [esp + %s], ax \n \
                    ; ---------------------------  \n ";
	}
	fprintf(out_ensambler, message, der.name, izq.name);
    //**********ENSAMBLADOR**********************************************************************
}