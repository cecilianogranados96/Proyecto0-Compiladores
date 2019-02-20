#include "main.h"
#include "scanner.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

//Variables
char token_buffer[0]; //Arreglo que va a contener los tokens

/*
	FUNCIONES DE MANEJO DEL BUFFER Y SCANNER
*/

//Funcion scanner que va a leer caracteres y los va a agrupar en tokens
//Este es el codigo que venia en el folleto/capitulo
token scanner(void){
    printf("scanner\n");
    int in_char, c;
    clear_buffer();
    if(feof(file_in))
        return SCANEOF;
    while ((in_char = fgetc(file_in))!=EOF){
        if (isspace(in_char))
            continue;
        else if (isalpha(in_char)){
            buffer_char(in_char);
            for(c = fgetc(file_in); isalnum(c) || c=='_'; c = fgetc(file_in))
                buffer_char(c);
            ungetc(c,file_in);
            return check_reserved();
        }else if(isdigit(in_char)){
            buffer_char(in_char);
            for(c = fgetc(file_in); isdigit(c); c = fgetc(file_in))
                buffer_char(c);
            ungetc(c,file_in);
            return INTLITERAL;
        }else if (in_char=='(')
            return LPAREN;
        else if (in_char==')')
            return RPAREN;
        else if (in_char==';')
            return SEMICOLON;
        else if (in_char==',')
            return COMMA;
        else if (in_char=='+')
            return PLUSOP;
        else if (in_char==':'){
            c = fgetc(file_in);
            if(c=='=')
            return ASSIGNOP;
            else{
            ungetc(c,file_in);
            lexical_error(in_char);
            }
        }
        else if(in_char=='-'){
            c=fgetc(file_in);
            if(c=='-'){
                    do
                        in_char=fgetc(file_in);
                    while(in_char!='\n');
                }
            else{
                    ungetc(c,file_in);
                    return MINUSOP;
                }
        }
        else
            lexical_error(in_char);
    }
    if(feof(file_in))
        return SCANEOF;
    printf("el programa no deberia de llegar hasta aqui, ver linea 71 de scanner.c\n");
}

//Funcion para limpiar el buffer
void clear_buffer(void)
{
    int contador; //se crea un contador para recorrer el buffer (arreglo de tokens)
    for(contador = strlen(token_buffer); contador >= 0; contador--)
    {
	/*
	1) El valor inicial de contador va a ser el largo del buffer (arreglo de tokens)
	2) Mientras el contador sea mayor o igual a 0, va a entrar en el ciclo
	3) Decrementa el contador
	*/
        token_buffer[contador] = 0; //limpia el buffer colocando 0s en las distitnas posiciones
    }
}

//Funcion para cargar el buffer con un caracter
void buffer_char(int c)
{
    //lo que hace es asignarle en la ultima posicion del arreglo el caracter actual
    token_buffer[strlen(token_buffer)] = c;
}

/*
Rutina del scanner que toma los identificadores conforme se van reconociendo y regresa ya sea:
1) ID de un token
2) Una palabra reservada
*/
token check_reserved(void)
{
    /*
    Primero se van a declarar los punteros a las palabras reservadas.
    En el lenguaje C se utiliza char * cuando se necesita almacenar datos y no se sabe
    el tamaño porque va a ser dinámico.
    */
    char * scaneof  = "scaneof";
    char * read     = "read";
    char * write    = "write";
    char * begin    = "begin";
    char * end      = "end";
    char * temp     = token_buffer;
    /*
    Primero, empieza a comparar el token almacenado en la variable temporal 'temp' y lo
    va a ir comparando con las palabras reservadas para confirmar o descartar que se tenga
    una palabra reservada como token actual.

    La función comparar recibe tres parámetros:
    1) El token actual almacenado en el buffer
    2) La palabra reservada con la que va a ser comparado
    3) La cantidad de caracteres que tiene esta palabra reservada
    Por ejemplo, en el primer caso, se envía el token actual, la palabra reservada 'read'
    y su tamaño en caracteres, 4.
    */
    if(comparar(temp, read, 4))
    {
        return READ; //si se cumple, retorna el token 'READ'
    }
    else if(comparar(temp, write, 5))
    {
        return WRITE; //si se cumple, retorna el token 'WRITE'
    }
    else if(comparar(temp, begin, 5))
    {
        return BEGIN; //si se cumple, retorna el token 'BEGIN'
    }
    else if(comparar(temp, end, 3))
    {
        return END; //si se cumple, retorna el token 'END'
    }
    else if(comparar(temp, scaneof, 7))
    {
        return
        SCANEOF; //si se cumple, retorna el token 'END'
    }
    else
    {
	//si no se cumple ninguno de los casos, el token actual es un ID
	return ID;
    }
}

//Otras funciones
/*
El propósito de la funcion comparar es verificar si un elemento de tipo token actual es igual a
alguna de las palabras reservadas del lengauje Micro.
Recibe tres parámetros:
    1) El token actual almacenado en el buffer
    2) La palabra reservada con la que va a ser comparado
    3) La cantidad de caracteres que tiene esta palabra reservada
*/
int comparar(char* actual, char* reservado, int tamano_reservado)
{
    int resultado = 1; //variable que va a contener el resultado final
    int contador = 0; //variable contador para recorrer string que forma la palabra reservada
    for (contador; contador <= tamano_reservado; contador++)
    {
	/*
	Mientras el contador sea menor al numero de caracteres de la palabra reservada
	va a entrar al ciclo y va a incrementar a contador cada vez.
	tolower -> es una funcion de para convertir caracteres en minusculas
	Compara los caracteres de la misma posicion:
	1) Si son iguales sigue en el ciclo
	2) Si son distintos se detiene
	*/
        if(tolower(actual[contador]) != reservado[contador])
        {
            resultado = 0; //negativo -> error
	    //printf("char actual: %c\n char reservado: %c\n",actual[contador],reservado[contador]);
            break; //se sale del ciclo
        }
    }
    return resultado; //si tuvo exito retorna el resultado
}

//En caso de que se genere un error lexico
void lexical_error(int in_char)
{
    //imprime un mensjae de error
    fprintf(stderr, "Error de tipo lexico en el caracter: %c \n", in_char);
    exit(1); //envia un codigo usando la funcion exit() del lenguaje C
}

//Funcion para obtener el siguiente token
token next_token()
{
    //Consigue el nuevo token llamando a la funcion de scanner
    //Esto para que realice las validaciones necesarias
    current_token = scanner();
    return current_token; //retorna el token encontrado
}

// funcion que funciona como next_token, solo que despues de leerlo devuelve la posicion del archivo al token anterior
token next_token_fake()
{
	long int currentPosition;
	currentPosition = ftell(file_in);
	token tok = next_token();
	fseek(file_in,currentPosition,SEEK_SET);
	return tok;

}

void print_token(token tok){
	switch(tok){
		case BEGIN:
			printf("BEGIN\n");
			break;
		case 1:
			printf("END\n");
			break;
		case 2:
			printf("READ\n");
			break;
		case 3:
			printf("WRITE\n");
			break;
		case 4:
			printf("ID\n");
			break;
		case 5:
			printf("INTLITERAL\n");
			break;
		case 6:
			printf("LPAREN\n");
			break;
		case 7:
			printf("RPAREN\n");
			break;
		case 8:
			printf("SEMICOLON\n");
			break;
		case 9:
			printf("COMMA\n");
			break;
		case 10:
			printf("ASSIGNOP\n");
			break;
		case 11:
			printf("PLUSOP\n");
			break;
		case 12:
			printf("MINUSOP\n");
			break;
		case 13:
			printf("SCANEOF\n");
			break;
		default:
			printf("token no valido\n");
			break;
	}
}

