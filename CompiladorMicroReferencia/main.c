#include <stdio.h>
#include "main.h"
#include "scanner.h"
#include "parser.h"
#include "semrecs.h"

/* 	variables globales del compilador	*/

	FILE * file_in;
	FILE * file_out;


int main(int argc, char *argv[])
{
 	if ( argc == 1 )
	    {
	        printf("El programa recibe dos argumentos: prog [inputFile] [outputFile]\n");
	        return -1;
	    }
	if ( argc == 2 )
	    {
	        printf("El programa recibe dos argumentos: prog [inputFile] [outputFile]\n");
	        return -1;
	    }
	if ( (file_in = fopen(argv[1], "r") ) == NULL )
	    {
	        printf("No se pudo abrir [inputFile]\n");
	        return -1;
	    }
	if ( (file_out = fopen(argv[2], "w") ) == NULL)
	    {
	        printf("No se pudo abrir [outputFile]\n");
	        return -1;
	    }

	//
	// aqui va el codigo del programa
	//

	/* se inicializa el programa con el parser */
	system_goal();




//#define SYMBOLTABLETEST
#if defined(SYMBOLTABLETEST)




	string sa = "id";
	string sb = "ab";
	string sc = "cd";

	printf("se busca: %s, %d\n",sa,lookup(sa));
	printf("se busca: %s, %d\n",sb,lookup(sb));
	enter(sb);
	enter(sc);
	printf("se busca: %s, %d\n",sc,lookup(sc));


#endif /* SYMBOLTABLETEST */

//#define SCANNERTEST
#if defined(SCANNERTEST)

	/* pruba del scanner */
	printf("ingresar 'n' para scanear el siguiente token, ingresar 'f' para ver el siguiente token sin avanzar a este mismo");
	char o;
	while(true){
		if(o=='f')
			printf("next value is: %d\n",next_token_fake());
		if(o=='n')
			printf("the value is %d\n",next_token());
		o = getchar();
	}

#endif  //SCANNERTEST

	printf("Se han ingresado los parametros correctamente\n");
	return 1;

}

