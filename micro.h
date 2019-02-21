#ifndef MICRO_H_
#define MICRO_H_
#define rowTS 15
#define ColumnsTS 13
#define MAXIDLEN 32+1
typedef enum token_types
{
	BEGIN,		//0
	END,		//1
	READ,		//2
	WRITE,		//3
	ID,		    //4
	INTLITERAL,	//5
	LPAREN,		//6
	RPAREN,		//7
	SEMICOLON,	//8
	COMMA,		//9
	ASSIGNOP,	//10
	PLUSSOP,	//11
	MINUSOP,	//12
	SCANEOF,    //13
    LEXICALERROR //14
} TOKEN;
typedef struct
{
    char lexema_identifier[MAXIDLEN];
    TOKEN t;
} RegTS;

typedef struct Token{
    TOKEN type;
    char tok[1025];
}Token;

typedef struct operator { /* for operators*/
	enum op { PLUS, MINUS } operator;
} op_rec;

/* expression types */
enum expr { INDEXPR, LITERALEXPR, TEMPEXPR };

typedef struct
{
    TOKEN clase;
    char name[MAXIDLEN];
    int value;
} REG_EXPRESION;
#endif