#include <stdio.h>
#include <ctype.h>

typedef enum token_types {
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
    PLUSOP,
    MINUSOP,
    SCANEOF
} token;

extern token scanner (void);

extern char token_buffer[];


void lexical_error(char c) {
	printf("lexical error : can\'t recognize %c this character",c);
}

token check_reserved() {
	const static char *ReserveWord[4] = { "begin", "end", "read", "write" };
	for (int i = 0; i < 4; i++)
		if (token_buffer == ReserveWord[i])
			return (token) i;
	return ID;
}

void clear_buffer(){
    //DEBERIA DE LIMPIAR EL VECTOR token_buffer
}

void buffer_char(int c){
    //DEBERIA DE INSERTAR "C" EN EL VECTOR token_buffer
}

token scanner(void){
    int in_char,c;
    clear_buffer();
    if(feof(stdin))
        return SCANEOF;
    while((in_char = getchar()) != EOF){
        if(isspace(in_char))
            continue; /* do nothing */
        else if (isalpha(in_char)){
            /*
            * ID ::= LETTER | ID LETTER
            *               | ID DIGIT
            *               | ID UNDERSCORE
            *
            */
            buffer_char(in_char);
            for(c = getchar(); isalnum(c) || c == '_' ; c = getchar())
                buffer_char(c);
            ungetc(c,stdin);
            return check_reserved();
        }else if (isdigit(in_char)){
            /*
            * ID ::= DIGIT | 
            *              | INTLITERAL DIGIT
            *
            */
            buffer_char(in_char);
            for(c=getchar();isdigit(c);c=getchar())
                buffer_char(c);
            ungetc(c,stdin);
            return INTLITERAL;
        } else if (in_char == '(')
            return LPAREN;
        else if (in_char == ')')
            return RPAREN;
        else if (in_char == ';')
            return SEMICOLON;
        else if (in_char == ',')
            return COMMA;
        else if (in_char == '+')
            return PLUSOP;
        else if (in_char == ':'){
            /* Looking for ":=" */
            c = getchar();
            if (c == '=')
                return ASSIGNOP;
            else{
                ungetc(c,stdin);
                lexical_error(in_char);
            }
        }else if (in_char == '-'){
            c = getchar();
            if (c == '-'){
                do 
                    in_char = getchar();
                while (in_char != '\n');
            } else {
                ungetc(c,stdin);
                return MINUSOP;
            }
        } else 
            lexical_error(in_char);
    }
}

int main() {    
    printf("HOLA MUNDO ");
	return 0;
}
