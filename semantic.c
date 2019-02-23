#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "headers/semantic.h"
#include "headers/fAuxs.h"

extern char token_buffer[];

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
}

void write_expr(REG_EXPRESION out)
{
    /* Genera la instruccion para escribir */
    generate("Write",  extract(&out), "Integer", "");
}

REG_EXPRESION gen_infix(REG_EXPRESION e1, char *op, REG_EXPRESION e2)
{
    /* Genera la instruccion para una operacion infija y construye un registro semantico con el result */
    REG_EXPRESION reg,e_rec;
    static unsigned int numTemp = 1;
    char cadTemp[MAXIDLEN] ="Temp&";
    char cadNum[MAXIDLEN];
    char cadOp[MAXIDLEN];    
    
    if ( op[0] == '-' ) strcpy(cadOp, "Sub");
    if ( op[0] == '+' ) strcpy(cadOp, "Add");

    sprintf(cadNum, "%d", numTemp);
    numTemp++;
    strcat(cadTemp, cadNum);
    
    if ( e1.clase == ID) check_id( extract(&e1));
    if ( e2.clase == ID) check_id( extract(&e2));
    
    //CONSTANT FOLDING SI SON DOS ENTEROS SEGUIDOS LOS CALCULA Y CONSTRUYE UNA NUEVA EXPRECION
    if (e1.clase == INTLITERAL && e2.clase == INTLITERAL){
            e_rec.clase = INTLITERAL;
            strcpy(e_rec.name, cadTemp);
            if ( op[0] == '-'){
                e_rec.value = e1.value - e2.value;
            }else{
                e_rec.value = e1.value + e2.value;
            }
            sprintf(cadTemp, "%d", e_rec.value);        
            //generate(cadOp, e1.name,  e_rec.name, cadTemp);
            strcpy(e_rec.name, cadTemp);
            return e_rec;
	}else{
        check_id(cadTemp); 
        generate(cadOp, e1.name,  e2.name, cadTemp);
        strcpy(reg.name, cadTemp);
        return reg;
    }
}