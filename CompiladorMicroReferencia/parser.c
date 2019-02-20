/*  copiada del libro
 *  falta funciones:
 *  	match(), next_token(); syntax_error(token);
 *
 *		match():
 *		llama al scanner para conseguir el sigguiente token. Ademas, en caso de ser exitoso, le hace update al current_token. Si falla, produce un Syntax Error.
 *
 *		next_token():
 *		retorna el siguiente token a ser matched, es como un "match next". No le hace update al current_token.	NOTA: next_token() ha sido remplazada por next_token_fake(), funcion que se encuentra en el scanner.
 *
 *		syntax_error(token):
 *		retorna un mensaje de error sobre 'token'.
 *
 *  falta variables globales (deberian de ir en el scanner):
 *
 *  	current_token:
 *	se guarda el token en caso de que match(t) encuentre el token correcto
 *
 *  falta agregarle el semantic processing al parser
 */

#include "parser.h"
#include <stdlib.h>

// system goal es como se inicia el parsing //
void system_goal(void)
{
	printf("system_goal\n");
	/* <system goal> ::= <program> SCANEOF */
	program();
	match(SCANEOF);
    fclose(file_in);
    fclose(file_out);
}

void program(void)
{
	/* <program> ::= BEGIN <statement list> END */
	printf("program\n");
	match(BEGIN);
	statement_list();
	match(END);
}

void statement_list(void)
{
	/*
	 * <statement list> :: = <statement>
	 * 			{ <statement> }
	 */
	printf("statement_list\n");
	statement();
	while(true){
		switch(next_token_fake()) {
			case ID:
			case READ:
			case WRITE:
			default:
				return;
		}
	}
}

void statement(void)
{
	token tok = next_token_fake();
	printf("statment");
	switch (tok) {
	case ID:
		/* <statement> :: ID := <expression> ; */
		match(ID); match(ASSIGNOP);
		expression(); match(SEMICOLON);
		break;

	case READ:
		/* <statement> ::= READ ( <id list> ) ; */
		match(READ); match(LPAREN);
		id_list(); match(RPAREN);
		match(SEMICOLON);
		break;

	case WRITE:
		/* <statement> ::= WRITE ( <expr list> ; */
		match(WRITE); match(LPAREN);
		expr_list(); match(RPAREN);
		match(SEMICOLON);
		break;
	default:
	    printf("statment error");
		syntax_error(tok);
		break;
	}
}

void id_list(void)
	{
		printf("id_list\n");
		/* <id list> ::= ID {, ID } */
		match(ID);

		while (next_token_fake() == COMMA) {
			match(COMMA);
			match(ID);
		}
	}

void expression(void)
{
	token t;
	printf("expression\n");
	// <expression> ::= <primary>			{ <add op> <primaray> }
	primary();
	for (t = next_token_fake(); t == PLUSOP || t== MINUSOP;
					t = next_token_fake())
	{
		add_op();
		primary();
	}
}

void expr_list(void)
{
	printf("expr_list\n");
	/* <expr list> ::= <expression> {, <expression> } */
	expression();

		while (next_token_fake() == COMMA) {
			match(COMMA);
			expression();
		}
}

void add_op(void)
{
	token tok = next_token_fake();
	printf("add_op\n");
	/* <addop> ::= PLUSOP | MINUSOP */
	if (tok == PLUSOP || tok == MINUSOP)
		match(tok);
	else{
        printf("add_op error");
		syntax_error(tok);
		}
}

void primary(void)
{
	token tok = next_token_fake();
	printf("primary\n");
	switch (tok) {
	case LPAREN:
		/* <primary> ::= ( <expression> ) */
		match(LPAREN); expression();
		match(RPAREN);
		break;

	case ID:
		/* <primary>::= ID */
		match(ID);
		break;

	case INTLITERAL:
		/* <primary> ::= INTLITERAL */
		match(INTLITERAL);
		break;

	default:
	    printf("primary error");
		syntax_error(tok);
		break;
	}
}

/*
Funcion que llama al scanner para conseguir el siguiente token.
En caso de ser exitoso, le hace update al current_token.
Si falla, produce un Syntax Error.
Recibe como parametro el token que debe ser encajado/igualado (matched).
*/
void match(token tok)
{
    printf("match: ");
    print_token(tok);
    if(tok != next_token()) {
        printf("match error: ");
        print_token(current_token);
        syntax_error(tok);
	}
	printf("token matched: ");
	print_token(tok);
}


//Funcion para generar mensaje de error en caso de que exista un error de semantica
void syntax_error(token tok)
{
    fprintf(stdout, "Error sintactico \n"); //imprime mensaje de error
    exit(4); //envia un codigo usando la funcion exit() del lenguaje C
}
