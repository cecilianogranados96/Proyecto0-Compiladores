#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "headers/scanner.h"
char token_buffer[200];

FILE *in;
/*Descrito por el libro, la idea es guardar el token actual*/
TOKEN current_token = SCANEOF;
TOKEN next;
int flag=0; //0=falso
int flag_next_token=0;

void clear_buffer(void){
	//Borra el buffer de token buffer.
	memset(token_buffer,'\0',strlen(token_buffer));
}

void buffer_char(int x){
	//Convierte el entero a caracter y lo agrega al token_buffer
	if (strlen(token_buffer)==0){
		char dato=(char)x;
		char auxiliar[]={(char)x,'\0'};
		strcat(token_buffer,auxiliar);
	}else{
        char dato=(char)x;
        char auxiliar[]={dato,'\0'};
        strcat(token_buffer,auxiliar);
	}
}

TOKEN check_reserved(){
	//Revise el token_buffer y si este es una palabra reservada retorna el token al que pertenece
	if ((strcmp(token_buffer,"READ")==0) || (strcmp(token_buffer,"read")==0)){
		return READ;
    }
	if ((strcmp(token_buffer,"WRITE")==0) || (strcmp(token_buffer,"write")==0)){
		return WRITE;
    }
	if ((strcmp(token_buffer,"BEGIN")==0) || (strcmp(token_buffer,"begin")==0)){
		return BEGIN;
    }
	if ((strcmp(token_buffer,"END")==0) || (strcmp(token_buffer,"end")==0)){
		return END;
    }
    if ((strcmp(token_buffer,"SCANEOF")==0) || (strcmp(token_buffer,"scaneof")==0)){
		return SCANEOF;
    }else{
	 return ID;
  }
}


TOKEN scanner(void)
{
    int in_char,c;

	clear_buffer();
	if (feof(in))
		return SCANEOF;

	while (feof(in)==0){
		in_char=fgetc(in);
		if (isspace(in_char))
			continue; /*do nothing */
		else if (isalpha(in_char)){
			/*
				ID::=LETTER | ID LETTER
							| ID DIGIT
							| ID UNDERSCORE
			*/
			buffer_char(in_char);
			for (c= fgetc(in);isalnum(c)||c=='-';c=fgetc(in))
				buffer_char(c);
				ungetc(c,stdin);
				return check_reserved();
		}else if (isdigit(in_char)){
			/*
				INTLITERAL :: = DIGIT |
								INTLITERAL DIGIT
			*/
			buffer_char(in_char);
			for (c=fgetc(in);isdigit(c);c=fgetc(in))
				buffer_char(c);
			ungetc(c,stdin);
			return INTLITERAL;
		}else if (in_char== '(')
			return LPAREN;
		else if (in_char==')')
			return RPAREN;
		else if (in_char==';')
			return SEMICOLON ;
		else if (in_char==',')
			return COMMA ;
		else if (in_char == '|'){
			return PIPE;
		}else if (in_char=='+'){
            buffer_char(in_char);
			return  PLUSSOP;
        }else if (in_char==':'){
			/*looking for ":="*/
			c=fgetc(in);
			if (c=='=')
				return ASSIGNOP;
			else{
				ungetc(c,stdin);
				lexical_error(in_char);
			}
		}else if (in_char=='-'){
			/*is it --;comment start */
			c=fgetc(in);
			if (c=='-'){
				do
					in_char=fgetc(in);
				while (in_char!='\n');
			}else{
				ungetc(c,stdin);
                buffer_char(in_char);
				return MINUSOP;
			}
		}else{
            //lexical_error(in_char); //ESTO ES UN CARACTER NO RECONOCIDO
            //printf("ERROR AQUI");
        }
			
    }
    return SCANEOF;
}
