#ifndef RSEMANTICAS_H_
#define RSEMANTICAS_H_
	
	#include "micro.h"
		
	REG_EXPRESION process_literal(void);
	REG_EXPRESION process_id(void);
	char * process_op(void);
	void read_id(REG_EXPRESION in);
	void write_expr(REG_EXPRESION out);
	REG_EXPRESION gen_infix(REG_EXPRESION e1, char * op, REG_EXPRESION e2);

#endif