#ifndef MICRO_H_
#define MICRO_H_
	
	#define rowTS 15
	#define ColumnsTS 13
	#define MAXIDLEN 32+1

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
	} TOKEN;

	typedef struct
	{
	    char lexema_identifier[MAXIDLEN];
	    TOKEN t;
	} RegTS;


	typedef struct
	{
	    TOKEN clase;
	    char name[MAXIDLEN];
	    int value;
	} REG_EXPRESION;

#endif