#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "main.h"

void system_goal(void);
void program(void);
void statement_list(void);
void statement(void);
void id_list(void);
//void expression(void);
void expr_list(void);
void add_op(void);
void primary(void);

extern void match(token tok);
//extern token next_token(void);
extern void syntax_error(token tok);


#endif
