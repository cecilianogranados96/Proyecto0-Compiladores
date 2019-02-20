#ifndef SCANNER_H
#define	SCANNER_H
#include <string.h>

//Arreglo de tokens del lenguaje Micro
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
	PLUSOP,		//11
	MINUSOP,	//12
	SCANEOF		//13
} token;

//Variables
extern char token_buffer[0]; //Arreglo que va a contener los tokens
token current_token; //Va a contener el token actual
int token_flag;

//Funciones de manejo del buffer y scanner
extern token scanner(void);
extern void clear_buffer(void);
extern void buffer_char(int c);
extern token check_reserved(void);
extern int comparar(char* buffer, char* token, int pLargo);
extern void lexical_error(int in_char);
extern token next_token(void);
extern token next_token_fake(void);
void print_token(token tok);

#endif	/* SCANNER_H */
