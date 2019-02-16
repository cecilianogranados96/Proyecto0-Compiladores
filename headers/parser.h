#ifndef PARSER_H_
#define PARSER_H_
	
	#include "micro.h"
	
	void system_goal(void);
	void program(void);
	void statement_list(void);
	void statement(void);
	void id_list(void);
	void Identificador(REG_EXPRESION * resultado);
	void expr_list(void);
	void Expresion(REG_EXPRESION * resultado);
	void primary(REG_EXPRESION * resultado);
	void add_op(char * resultado);



#endif