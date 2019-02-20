#ifndef semrecs
#define semrecs

#include "scanner.h"
#include "main.h"
#include "parser.h"

/* primero se declaran los structs */

#define MAXIDLEN 31
#define MAXSYMBOLS 99

typedef char string[MAXIDLEN];

typedef struct operator { /* for operators*/
	enum op { PLUS, MINUS } operator;
} op_rec;

/* expression types */
enum expr { INDEXPR, LITERALEXPR, TEMPEXPR };

/* for <primary> and <expression> */
typedef struct expression {
	enum expr kind;
	union {
		string name;	/* for IDEXPR, TEMPEXPR */
		int val;	/* for LITERALEXPR */
	};
} expr_rec;



/* symbol table */

string symbolsTable[MAXSYMBOLS];



/* falta de crear las siguentes funciones */

/* is S in the symbol table? */
extern int lookup(string s);

/* put S unconditionally into the symbol table. */
extern void enter(string s);
/* aqui abajo se declaran las funciones de semrecs.c*/

void check_id(string s);
char *get_temp(void);
void generate(char* opcode, char* op1, char* op2, char* field);
void expression(expr_rec *result);

#endif
