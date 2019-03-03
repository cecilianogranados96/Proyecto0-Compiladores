#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "headers/semantic.h"
#include "headers/fAuxs.h"

extern char token_buffer[];
extern FILE * out_ensambler;


/* -------------------  RUTINAS SEMANTICAS  ------------------- */
REG_EXPRESION process_literal(void)
{
    /* Convierte cadena que representa numero a numero entero y construye un registro semantico */
    REG_EXPRESION reg;
    reg.clase = INTLITERAL;
    strcpy(reg.name, token_buffer);
    sscanf(token_buffer, "%d", &reg.value);
    return reg;
}

REG_EXPRESION process_id(void)
{
    /* Declare ID y construye el correspondiente registro semantico */
    REG_EXPRESION reg;
    check_id(token_buffer);
    reg.clase = ID;
    strcpy(reg.name, token_buffer);
    return reg;
}

char * process_op(void)
{
    /* Declare OP y construye el correspondiente registro semantico */
    return token_buffer;
}

void read_id(REG_EXPRESION in)
{
    /* Genera la instruccion para leer */
    generate("Read", in.name, "Integer", "");
    
    //**********ENSAMBLADOR**********************************************************************
    char *message = "; ------------------------  \n \
                    \t; Leer de consola -> read \n \
                    ; \n \n \
                    mov eax, 3 	; EAX -> read  \n \
                    mov ebx, 0 	; EBX -> input \n \
                    mov ecx, buffer_input ; ECX -> valor en que se almacena   \n \
                    mov edx, len_input	; EDX -> largo de lo que voy a leer \n \
                    int 0x80 	; llamada al sistema operativo    \n  \n \
                    mov edi, eax \n \
                    xor edx, edx \n \
                    xor esi, esi \n \
                    xor eax, eax \n \
                    mov ebx, 10  \n \
                                 \n \
                    dec edi		 \n \
                    call atoi   \n \
                    mov [esp + %s], ax \n \
                    ; Realiza el read -> y se almacena en memoria. \n \
                    ; ---------------------------  \n \n ";
	fprintf(out_ensambler, message, in.name);
    //**********ENSAMBLADOR**********************************************************************
    
}

void write_expr(REG_EXPRESION out)
{
    /* Genera la instruccion para escribir */
    generate("Write",  extract(&out), "Integer", "");
    
    //**********ENSAMBLADOR********************************************************************** 
    char *message = "; ---------------------------  \n \
                        \t; Escribir en consola -> write \n \
                        ; \n \
                        mov ax, [esp + %s]  ; pongo en el ax e\n \
                        mov ebx, 10 \n \
                        mov esi, 30 \n \
                        call itoa   \n \
                        \n \
                        mov eax, 4 	; EAX -> write  \n \
                        mov ebx, 1 	; EBX -> input  \n \
                        mov ecx, buffer_output ; ECX -> valor que se va a imprimir     \n \
                        mov edx, len_output	; EDX -> largo de lo que voy a escribir \n \
                        int 0x80 	; llamada al sistema operativo      \n \
                        ; ---------------------------  \n \n ";
	fprintf(out_ensambler, message, out.name);
    //**********ENSAMBLADOR**********************************************************************
    
    
    
}

REG_EXPRESION gen_infix(REG_EXPRESION e1, char *op, REG_EXPRESION e2)
{
    /* Genera la instruccion para una operacion infija y construye un registro semantico con el result */
    REG_EXPRESION reg,e_rec;
    static unsigned int numTemp = 1;
    char cadTemp[MAXIDLEN] ="Temp&";
    char cadNum[MAXIDLEN];
    char cadOp[MAXIDLEN];
    char arr[5];

    if ( op[0] == '-' ) strcpy(cadOp, "Sub");
    if ( op[0] == '+' ) strcpy(cadOp, "Add");

    sprintf(cadNum, "%d", numTemp);
    numTemp++;
    strcat(cadTemp, cadNum);

    if ( e1.clase == ID) check_id( extract(&e1));
    if ( e2.clase == ID) check_id( extract(&e2));

    //CONSTANT FOLDING SI SON DOS ENTEROS SEGUIDOS LOS CALCULA Y CONSTRUYE UNA NUEVA EXPRESION
        
    if (e1.clase == INTLITERAL && e2.clase == INTLITERAL){
            e_rec.clase = INTLITERAL;
            strcpy(e_rec.name, cadTemp);
            if ( op[0] == '-'){
                e_rec.value = e1.value - e2.value;
                strcpy(arr, "sub");
            }else{
                e_rec.value = e1.value + e2.value;
                strcpy(arr, "add");
            }
            sprintf(cadTemp, "%d", e_rec.value);
            
            //**********ENSAMBLADOR**********************************************************************
            char *messageTwoLi = " ; ---------------------------  \n \
                                    \t; %s Literal, result \n \
                                    mov eax, %s        \n \
                                    mov [esp + %s], ax   \n \
                                    ; ---------------------------  \n \n ";
            fprintf(out_ensambler, messageTwoLi, arr, e_rec.name, cadTemp);
            //**********ENSAMBLADOR**********************************************************************
            
            strcpy(e_rec.name, cadTemp);
            return e_rec;
	}else{
    
        check_id(cadTemp);
        generate(cadOp, e1.name,  e2.name, cadTemp);
        strcpy(reg.name, cadTemp);
     
        //**********ENSAMBLADOR**********************************************************************
        char *message;
        if (e1.clase == LITERALEXPR || e2.clase == LITERALEXPR){
		  message = "; ---------------------------  \n \
                     \t; %s  op1, Literal, result \n \
                     mov eax, %s \n \
                     mov ebx, [esp + %s] \n \
                     %s eax, ebx \n \
                     mov [esp + %s], ax \n \
                     ; ---------------------------  \n \n ";
        }else{
            message = "; ---------------------------  \n \
                        \t; %s  op1, op2, result \n \
                        mov eax, [esp + %s] \n \
                        mov ebx, [esp + %s] \n \
                        %s eax, ebx \n \
                        mov [esp + %s], ax \n \
                        ; ---------------------------  \n \n ";
        }  
        fprintf(out_ensambler, message, op[0], e1.name, e2.name, op[0], e_rec.name);
        //**********ENSAMBLADOR**********************************************************************

        
        return reg;
    }    
}

REG_EXPRESION conditional_expressions(REG_EXPRESION e1, REG_EXPRESION e2, REG_EXPRESION e3){

  REG_EXPRESION e_rec;
  char cadTemp[MAXIDLEN] ="Temp&";
  strcpy(e_rec.name, cadTemp);
  if(e1.value == 0){
    e_rec.value = e3.value;
  }else{
    e_rec.value = e2.value;
  }
  sprintf(cadTemp, "%d", e_rec.value);
  strcpy(e_rec.name, cadTemp);
  return e_rec;
}
