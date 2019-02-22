#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define MAXIDLEN 33

typedef char string[MAXIDLEN];

char *table[100];
int dim=0;
void enter(string s){
	table[dim]=strdup(s);
	dim=dim +1;
}

char token_buffer[200];
FILE *archivo;
FILE *salida;

#define MAXIDLEN 33
typedef char string[MAXIDLEN];

extern void enter(string s);

typedef enum tipos_palabras{
	ADD,SUB,DECLARE,STORE,MOVE,HALT,FIN,VAR
} palabras;

//Variable para saber el temporal
int val_temp=0;
int var_ready=0;

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
	}
	else{
	   char dato=(char)x;
	   char auxiliar[]={dato,'\0'};
 	   strcat(token_buffer,auxiliar);
	}
}

palabras reservado(){
	//Revise el token_buffer y si este es una palabra reservada retorna el token al que pertenece
	if (strcmp(token_buffer,"Add")==0){
		return ADD;}
	if (strcmp(token_buffer,"Sub")==0){
		return SUB;}
	if (strcmp(token_buffer,"Declare")==0){
		return DECLARE;}
	if (strcmp(token_buffer,"Store")==0){
		return STORE;
	}
	if (strcmp(token_buffer,"Move")==0){
		return MOVE;
	}
	if (strcmp(token_buffer,"Halt")==0){
		return HALT;
	}else{
	 return VAR;
  }
}

palabras escaner(void)
{
	int in_char,c;
	clear_buffer();
	if (feof(archivo))
		return FIN;
	while (feof(archivo)==0){
		in_char=fgetc(archivo);
		if (isspace(in_char))
			continue; /*do nothing */
		else if (isalpha(in_char)){
			buffer_char(in_char);
			for (c= fgetc(archivo);isalnum(c)||c=='-'||c=='&';c=fgetc(archivo))
                if (c!=','){
					buffer_char(c);
			     }else{
					return reservado();
				}
				return reservado();
		}else if (isdigit(in_char)){
			buffer_char(in_char);
			for (c=fgetc(archivo);isdigit(c)||isspace(c);c=fgetc(archivo))
			 	if (isspace(c)){
			 		return reservado();
                }else{
					buffer_char(c);
                }
        }   
   }
}

int es_numer(char x[]){
	if (x[0]>='0' && x[0]<='9'){
		return 1;
    }else{
	   return 0;	
    }
}

void crear_temporal(int x ,char * usar){
		char num[10];
		char registro[10];
        sprintf(registro,"%d",val_temp);
		sprintf(num,"$t%s",registro);
		strcpy(usar,num);
		val_temp = val_temp+1; 
}

void operacion(int op){
	char t1[10];
	char t2[10];
	char t3[10];
	char operando1[33];
	escaner();
	strcpy(operando1,token_buffer);
	char operando2[33];
	escaner();
	strcpy(operando2,token_buffer);
	char registro[33];
	escaner();
	strcpy(registro,token_buffer);
    crear_temporal(0,&t1);
    crear_temporal(1,&t2);
    crear_temporal(2,&t3);
    if (es_numer(operando1)){
        fprintf(salida,"li %s,%s\n",t1,operando1);
    }else{
        fprintf(salida,"lw %s,%s\n",t1,operando1);
    }

    if (es_numer(operando2)){
        fprintf(salida,"li %s,%s\n",t2,operando2);
    }else{
        fprintf(salida,"lw %s,%s\n",t2,operando2);
    }

    if (op==0){
        fprintf(salida,"%s %s,%s, %s\n","sub",t3,t2,t1);
    }else{
        fprintf(salida,"%s %s,%s, %s\n","add",t3,t2,t1 );
    }
    fprintf(salida,"sw %s,%s\n",t3,registro);
    val_temp=0;
}

void imprimir_variables(){
	extern int dim;
	extern char * table[];
	int i;
	fprintf(salida,"%s\n",".data");
	for (i=0;i<dim;i++){

		fprintf(salida,"%s : .word 24 \n",table[i]);
	}
}
void declare(){
	//Si no se han declarado las variables llama a imprimir variables. Y cambia la bandera para solo realizar una impresión
	//de estas
	escaner();
	enter(token_buffer);
	if (var_ready==0){
	    var_ready=1;
   		fprintf(salida,"%s\n %s\n",".text","main:");
   }
}

void terminar(){
	fprintf(salida, "%s\n %s \n","li $v0, 10","syscall");
	imprimir_variables();
	exit(1);
}

void almacenar(){
	char registro1[33];
	char registro2[33];
	char tem[33];
	escaner();
	strcpy(registro1,token_buffer);
	escaner();
	strcpy(registro2,token_buffer);
	crear_temporal(0,&tem);
	fprintf(salida,"lw %s,%s \n",tem,registro1);
	fprintf(salida, "sw %s,%s \n",tem,registro2);
}

void micro(){
    fprintf(salida,"%s\n",".globl main");
	palabras palabras = escaner();
	while (palabras!=FIN){
		switch(palabras){
			case DECLARE:
				declare();
				break;
			case SUB:
				operacion(0);
				break;
			case ADD:
				operacion(1);
				break;
			case HALT:
				terminar();
				break;
			case STORE:
				almacenar();
				break;
			default:
				break;
		}
		palabras=escaner();
	}

}

int main(int argc, char const *argv[]){
   
	archivo=fopen("salida.m","r+");
    salida=fopen("out.asm","w+");
	micro();
	fclose(salida);
	fflush(stdout);
	return 0;
}