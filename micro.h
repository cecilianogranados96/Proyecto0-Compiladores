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
	ID,		//4
	INTLITERAL,	//5
	LPAREN,		//6
	RPAREN,		//7
	SEMICOLON,	//8
	COMMA,		//9
	ASSIGNOP,	//10
	PLUSSOP,	//11
	MINUSOP,	//12
	SCANEOF,
    LEXICALERROR
} TOKEN;


/*
	typedef enum
	{
	    BEGIN,
        END,
        READ,
        WRITE,
        ID,
        INTLITERAL,
        LPAREN,
        RPAREN,
        SEMICOLON,
        COMMA,
        ASSIGNOP,
        PLUSSOP,
        MINUSOP,
        FDT,
        LEXICALERROR,
        SCANEOF
	} TOKEN1;
*/
	typedef struct
	{
	    char lexema_identifier[MAXIDLEN];
	    TOKEN t;
	} RegTS;


typedef struct Token{
    TOKEN type;
    char tok[1025];
}Token;

//////////////////////

//semrecs.h line 15

//////////////////////

	typedef struct
	{
	    TOKEN clase;
	    char name[MAXIDLEN];
	    int value;
	} REG_EXPRESION;

#endif
