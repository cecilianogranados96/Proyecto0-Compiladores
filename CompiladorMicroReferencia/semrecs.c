#include "semrecs.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

/* hace falta
 * 	generate():
 * 	recibe cuatro arguemntos string, corresponde a operation code, dos operandos, y el field de resltado. Produce la instruccion correcta al output file.
 */

void generate(char* opcode, char* op1, char* op2,
	char* field)
{
	//write to file
	//
	//aqui tienen que ir las instrucciones ya asm
	fprintf(file_out, "\t%s  %s,%s,%s\n",opcode,op1,op2,field);
	printf("\t%s  %s,%s,%s\n",opcode,op1,op2,field);
}


/* 	extract():
 * 	recibe un semantic recod, y retorna un string que corresponde a la informacion que contiene el semantic record. La informacion extraida se le pasa a generate() para completar la instruccion
 */

char * extract_op(op_rec record){
	return (record.operator?"MINUS_OP":"PLUS_OP");
}

char * extract_expr(expr_rec record){
	char *a=malloc(MAXIDLEN);
	switch(record.kind){
		case INDEXPR:
		case TEMPEXPR:
			strcpy(a,record.name);
			return a;
		case LITERALEXPR:
			snprintf(a,MAXIDLEN,record.name);
		default:
			return a;
	}
}

/* rutina auxiliar que se usa por varias semantic routines
 *
 * chehck_id revisa si hay una variable en el symbol table, s no lo hay, declara la variable guardandola en el symbol table y genera una directiva de ensamblador para reservar espacio para la variable. Aqui se usa "Declare" como pseudo operacion que declara el nombre al ensambador y define su tipo.
 *
 * supuestamente el ensamblador decide el espacio requerido para la variable y donde guardarla.
 *
 */
/* RUTINAS AUXILIARES DE TABLA DE SIMBOLOS */

//falta completar
extern int lookup(string s){
	for(int i=0;symbolsTable[i][0];i++)
		if(!strcmp(s,symbolsTable[i]))
			return 1;
	return 0;
}


//falta completar
extern void enter(string s){
	int i;
	for(i=0;symbolsTable[i][0];i++){}
	strcpy(symbolsTable[i],s);
}

/* RUTINAS AUXILIARES DE LAS RUTINAS SEMANTICAS: */

void check_id(string s)
{
	if (! lookup(s)) {
		enter(s);
		generate("Declare", s, "Integer","");
	}
}

// la funcion lookup esta en el symboltable.c

/* get_temp se utiliza para asignar nombres a variables temporares, reciben el nombre de Temp&1, Temp&2, etc...*/
char *get_temp(void)
{
	/* max temporary allocated so far */
	static int max_temp = 0;
	static char tempname[MAXIDLEN];

	max_temp++;
	sprintf(tempname, "Temp&%d", max_temp);
	check_id(tempname);
	return tempname;
}


/* RUTINAS SEMANTICAS DE LOS ACTION SYMBOLS */

void start(void)
{
	/* semantic initializations, none needed */
}

void finish(void)
{
	/* generate code to finish program */
	generate("Halt", "", "", "");
}

void assign(expr_rec target, expr_rec source)
{
	/* Generate code for assignment. */
	generate("Store", extract_expr(source), target.name, "");
}

op_rec process_op(void)
{
	/* produce operator descriptor */
	op_rec o;
    printf("ESTA PROCESADO : %d \n",current_token);
	if (current_token == PLUSOP)
		o.operator = PLUS;
	else
		o.operator = MINUS;
	return o;
}

expr_rec gen_infix(expr_rec e1, op_rec op, expr_rec e2)
{
	expr_rec e_rec;
	/* An expr_rec with temp variant set. */
	e_rec.kind = TEMPEXPR;

	/*
	 * Generate code for infix operation.
	 * Get result temp and set up semantic record
	 * for result.
	 *
	 */
	strcpy(e_rec.name, get_temp());
	generate(extract_op(op), extract_expr(e1),
		extract_expr(e2), e_rec.name);
	return e_rec;
}

void read_id(expr_rec in_var)
{
	/* Generate code for read */
	generate("Read", in_var.name, "Integer", "");
}

expr_rec process_id(void)
{
	expr_rec t;

	/*
	 * Declare ID and build a
	 * corresponding semantic record.
	 */
	check_id(token_buffer);
	t.kind = INDEXPR;
	strcpy(t.name, token_buffer);
	return t;
}

expr_rec process_literal(void)
{
	expr_rec t;
	/*
	 * convert literal to a numeric representation
	 * and build a semantic record
	 */
	t.kind= LITERALEXPR;
	(void) scanf(token_buffer, "%d", &t.val);
	return t;
}

void write_expr(expr_rec out_expr)
{
	generate("Write", extract_expr(out_expr), "Integer", "");
}

/*
void expression(expr_rec *result)
{
  expr_rec left_operand,right_operand;
  op_rec op;
  primary(&left_operand);
  while (next_token() == PLUSOP || next_token() == MINUSOP){
    add_op(&op);
    primary(&right_operand);
    left_operand=gen_infix(left_operand,op,right_operand);
  }
  *result=left_operand;
}
*/








