#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "headers/parser.h"
#include "headers/semantic.h"
#include "headers/fAuxs.h"

/* -------------- PROCEDIMIENTOS DE ANALISIS SINTACTICO (PAS) -------------- */
void system_goal(void)
{
    /* <objetivo> -> <programa> FDT #terminar */
    program();
    Match(FDT);
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

void Identificador(REG_EXPRESION * resultado)
{
    /* <identificador> -> ID #procesar_id */
    Match(ID);
    *resultado = process_id();
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

void Expresion(REG_EXPRESION * resultado)
{
    /* <expresion> -> <primaria> { <operadorAditivo> <primaria> #gen_infijo } */
    REG_EXPRESION operandoIzq, operandoDer;
    char op[MAXIDLEN];
    TOKEN t;
    primary(&operandoIzq);
    for ( t = next_token(); t == PLUSSOP || t == MINUSOP; t = next_token() )
    {
        add_op(op);
        primary(&operandoDer);
        operandoIzq = gen_infix(operandoIzq, op, operandoDer);
    }
    *resultado = operandoIzq;
}

void primary(REG_EXPRESION * resultado)
{
    TOKEN tok = next_token();
    switch ( tok )
    {
        case ID :
            /* <primaria> -> <identificador> */
            Identificador(resultado);
            break;
        case INTLITERAL :
            /* <primaria> -> INTLITERAL #procesar_cte */
            Match(INTLITERAL);
            *resultado = process_literal();
            break;
        case LPAREN :
            /* <primaria> -> LPAREN <expresion> RPAREN */
            Match(LPAREN);
            Expresion(resultado);
            Match(RPAREN);
            break;
        default :
            return;
    }
}

void add_op(char * resultado)
{
    /* <operadorAditivo> -> PLUSSOP #procesar_op | MINUSOP #procesar_op */
    TOKEN t = next_token();
    if ( t == PLUSSOP || t == MINUSOP )
    {
        Match(t);
        strcpy(resultado, process_op());
    }
    else
        sintax_error();
}