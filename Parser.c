#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "headers/parser.h"
#include "headers/semantic.h"
#include "headers/fAuxs.h"

/* -------------- PROCEDIMIENTOS DE ANALISIS SINTACTICO (PAR) -------------- */
void system_goal(void)
{
    /* <objetivo> -> <programa> FDT #terminar */
    program();
    Match(SCANEOF);
    finish();
}

void program(void)
{
    /* <programa> -> #comenzar BEGIN <listastatements> END */
    start();
    Match(BEGIN);
    statement_list();
    Match(END);
}

void statement_list(void)
{
    /* <listastatements> -> <sentencia> {<sentencia>} */
    statement();
    while ( 1 )
    {
        switch ( next_token() )
        {
        case ID : case READ : case WRITE :
            statement();
            break;
        default : return;
        }
    }
}

void statement(void)
{
    TOKEN tok = next_token();
    REG_EXPRESION izq, der;    
    switch ( tok )
    {
        case ID :
            /* <sentencia>-> ID := <expresion> #asignar ; */
            Identificador(&izq);
            Match(ASSIGNOP);
            Expresion(&der);
            assign(izq, der);
            Match(SEMICOLON);
            break;
        case READ :
            /* <sentencia> -> READ ( <listaIdentificadores> ) */
            Match(READ);
            Match(LPAREN);
            id_list();
            Match(RPAREN);
            Match(SEMICOLON);
            break;
        case WRITE :
            /* <sentencia> -> WRITE ( <listaExpresiones> ) */
            Match(WRITE);
            Match(LPAREN);
            expr_list();
            Match(RPAREN);
            Match(SEMICOLON);
            break;
        default :
            return;
    }
}

void id_list(void)
{
    /* <listaIdentificadores> -> <identificador> #leer_id {COMMA <identificador> #leer_id} */
    TOKEN t;
    REG_EXPRESION reg;
    Identificador(&reg);
    read_id(reg);
    for ( t = next_token(); t == COMMA; t = next_token() )
    {
        Match(COMMA);
        Identificador(&reg);
        read_id(reg);
    }

}

void Identificador(REG_EXPRESION * result)
{
    /* <identificador> -> ID #procesar_id */
    Match(ID);
    *result = process_id();
}

void expr_list(void)
{
    /* <listaExpresiones> -> <expresion> #escribir_exp {COMMA <expresion> #escribir_exp} */
    TOKEN t;
    REG_EXPRESION reg;
    Expresion(&reg);
    write_expr(reg);
    for ( t = next_token(); t == COMMA; t = next_token() )
    {
        Match(COMMA);
        Expresion(&reg);
        write_expr(reg);
    }
}


void Expresion(REG_EXPRESION * result)
{
    
    REG_EXPRESION left_operand, rigth_operand;
    char op[MAXIDLEN];
    TOKEN t;
    primary(&left_operand);
    int calc_value = 0;
    for ( t = next_token(); t == PLUSSOP || t == MINUSOP; t = next_token() )
    {
        add_op(op);
   
        primary(&rigth_operand);
        if (left_operand.clase==INTLITERAL && rigth_operand.clase==INTLITERAL){
            if (t == MINUSOP){
                if(calc_value==0){
					calc_value = left_operand.value - rigth_operand.value;
				}else{
					calc_value -= rigth_operand.value;
				}
            }else{
                if(calc_value==0){
					calc_value = left_operand.value + rigth_operand.value;;
				}else{
					calc_value += rigth_operand.value;
				}
            }
			rigth_operand.value = calc_value;
        
            //printf("RIGTH OPERAND  CLASS: %d VALOR: %d \n",rigth_operand.clase,rigth_operand.value);
            //printf("LEFT OPERAND  CLASS: %d VALOR: %d NAME: %d \n",left_operand.clase,left_operand.value,left_operand.name[0]);
            printf("\t\t\t VALOR CALCULADOOOO: %d \n",calc_value);
            
		}
		left_operand = gen_infix(left_operand, op, rigth_operand);
    }
    *result = left_operand;
}


/*
void Expresion(REG_EXPRESION * result)
{
    REG_EXPRESION left_operand, rigth_operand;
    char op[MAXIDLEN];
    TOKEN t;
    primary(&left_operand);
    for ( t = next_token(); t == PLUSSOP || t == MINUSOP; t = next_token() )
    {
        add_op(op);
        primary(&rigth_operand);
        left_operand = gen_infix(left_operand, op, rigth_operand);
    }
    *result = left_operand;
}
*/




void primary(REG_EXPRESION * result)
{
    TOKEN tok = next_token();
    switch ( tok )
    {
        case ID :
            /* <primaria> -> <identificador> */
            Identificador(result);
            break;
        case INTLITERAL :
            /* <primaria> -> INTLITERAL #procesar_cte */
            Match(INTLITERAL);
            *result = process_literal();
            break;
        case LPAREN :
            /* <primaria> -> LPAREN <expresion> RPAREN */
            Match(LPAREN);
            Expresion(result);
            Match(RPAREN);
            break;
        default :
            return;
    }
}

void add_op(char * result)
{
    /* <operadorAditivo> -> PLUSSOP #procesar_op | MINUSOP #procesar_op */
    TOKEN t = next_token();
    if ( t == PLUSSOP || t == MINUSOP )
    {
        Match(t);
        strcpy(result, process_op());
    }
    else
        sintax_error();
}