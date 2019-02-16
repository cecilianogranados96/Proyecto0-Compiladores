#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "headers/scanner.h"


extern TOKEN scanner (void);

extern char token_buffer[];

extern FILE * in;


TOKEN check_reserved() {
	const static char *ReserveWord[4] = { "begin", "end", "read", "write" };
	for (int x = 0; x < 4; x++){
        printf("OJO: %s -- %s \n", token_buffer, ReserveWord[x]);
		if (token_buffer == ReserveWord[x]){
            printf("I VALE OJO: %d \n ",x);
			return x;
        }
    }
    printf("ID OJO: %d \n",ID);
    
	return ID;
}

void clear_buffer(){
    memset(token_buffer, 0, strlen(token_buffer));
}

void buffer_char(char c){
    size_t len = strlen(token_buffer);
    clear_buffer();
    token_buffer[len+1] = c;
}


TOKEN scanner(){
    
    int in_char,c;
    
    clear_buffer();
    
    if(feof(stdin))
        return SCANEOF;
    
    while((in_char = fgetc(in)) != EOF){
        //printf("%d",in_char);
        
        if(isspace(in_char))
            continue;
        
        if (isalpha(in_char)){
            buffer_char(in_char);
            for(c = fgetc(in); isalnum(c) || c == '_' ; c = fgetc(in))
                buffer_char(c);
            ungetc(c,in);
            return check_reserved();
        } 
        
        if (isdigit(in_char)){
            buffer_char(in_char);
            for(c = fgetc(in);  isdigit(c); c = fgetc(in))
                buffer_char(c);
            ungetc(c,in);
            return INTLITERAL;
        } 
        
        if (in_char == '(')
            return LPAREN;
        
        if (in_char == ')')
            return RPAREN;
        
        if (in_char == ';')
            return SEMICOLON;
        
        if (in_char == ',')
            return COMMA;
        
        if (in_char == '+')
            return PLUSSOP;
        
        
        if (in_char == ':'){
            c = fgetc(in);
            if (c == '=')
                return ASSIGNOP;
            else{
                ungetc(c,in);
                lexical_error2(in_char);
            }
        } 
        
        if (in_char == '-'){
            c = fgetc(in);
            if (c == '-'){
                do 
                    in_char = fgetc(in);
                while (in_char != '\n');
            } else {
                ungetc(c,in);
                return MINUSOP;
            }
        } 
        
        lexical_error2(in_char);
    }
    
}
