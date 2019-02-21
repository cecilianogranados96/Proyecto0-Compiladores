#ifndef FAUXS_H_
#define FAUXS_H_
#include "micro.h"
	void Match(TOKEN t);
	TOKEN next_token();
	void lexical_error();
    void lexical_error2(char c);
	void sintax_error();
	void generate(char * co, char * a, char * b, char * c);
	char *  extract(REG_EXPRESION * preg);
	int lookup(char * id, TOKEN * t);
	void enter(char * id);
	void check_id(char * s);
	void start(void);
	void finish(void);
	void assign(REG_EXPRESION izq, REG_EXPRESION der);
#endif