#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "headers/micro.h"
#include "headers/parser.h"
FILE * in;
FILE * out;
RegTS TS[1000] = { {"begin", BEGIN}, {"end", END}, {"read", READ}, {"write", WRITE},{"scaneof", SCANEOF}, {"$", 99} };
char token_buffer[MAXIDLEN];
TOKEN token;
int flagToken = 0;
int main(int argc, char * argv[])
{
    printf("\n\n*******************COMPILADOR DE MICRO*******************\n\n");
    int l = strlen(argv[1]);
    if ( argc == 1 ){
        printf("Error. Ingresar el nombre del archivo fuente y el nombre del archivo de salida\n");
        return -1;
    }
    if ( argv[1][l-1] != 'm' || argv[1][l-2] != '.' ){
        printf("Error. El archivo fuente debe finalizar con \".m\"\n");
        return -1;
    }
    if ( (in = fopen(argv[1], "r") ) == NULL ){
        printf("No se pudo abrir archivo fuente\n");
        return -1;
    }
    out = fopen("bin/salida.micro", "w");
    /* --------------  Iniciamos el proceso de compilacion:  -------------- */
    system_goal();
    /* --------------  Terminado el proceso, cerramos los archivos:  -------------- */
    fclose(in);
    fclose(out);
    
      
    system("gcc -o Traductor Ensamblador/ensamblador.c -w");

    system("./Traductor");
    
    system("nasm -f elf bin/salida.asm");

    system("ld -m elf_i386 bin/salida.o -o bin/salida");
    
    printf("\n\n*******************EJECUTANDO ENSAMBLADOR*******************\n\n");
    system("./bin/salida");
    printf("\n\n");
    
    
    return 0;
}
