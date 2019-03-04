#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "ensamblador.h"
#define STRSZ 256
#define INSTSZ 33

void Ensamblar(const char *input,const char *output){
    FILE *in=fopen (input, "r" );
    FILE *out=fopen (output, "w" );
    FILE *code=tmpfile();       //si no funciona archivo temporal:  FILE *code=fopen ("Temp.txt", "w+" );

    if (in == NULL)
        {
            printf("Error! Could not open input file\n");
        }
    else if (out == NULL)
        {
            printf("Error! Could not open output file\n");
        }
    else {
        fprintf(out,header_data);
        fprintf(out,header_temp);
        posId=0;
        Traducir(in,out,code);
        fclose ( in );
        fclose ( out );
        fclose ( code );
    }
}


void Traducir(FILE *in,FILE *out,FILE *code){
    char inst_buffer[INSTSZ]="";
    char op1_buffer[INSTSZ]="";
    char op2_buffer[INSTSZ]="";
    char op3_buffer[INSTSZ]="";
    char Line_buffer[STRSZ]="";
    char op1_temp[INSTSZ+2]="";
    char op2_temp[INSTSZ+2]="";
    int opN,varN,ind,av,IfCont,IfLev;
    char c;
    IfCont=IfLev=opN=0;
    while ( fgets ( Line_buffer, STRSZ, in ) != NULL ) /* lectura de instruccion completa para su futura analizacion */
    {
        ind=av=0;  //separacion de los operandos
        memset(inst_buffer, '\0', INSTSZ);
        memset(op1_buffer, '\0', INSTSZ);
        memset(op2_buffer, '\0', INSTSZ);
        memset(op3_buffer, '\0', INSTSZ);
        memset(op1_temp, '\0', INSTSZ+2);
        memset(op2_temp, '\0', INSTSZ+2);
        while(Line_buffer[ind]!='\0'&&Line_buffer[ind]!='\n'&&Line_buffer[ind]!='\r'&& Line_buffer[ind]!=44&& Line_buffer[ind]!=' '){
            inst_buffer[av]=Line_buffer[ind];
            ind++;
            av++;
            }

        ind++;
        av=0;
        while(Line_buffer[ind]!='\0'&&Line_buffer[ind]!='\n'&&Line_buffer[ind]!='\r'&&Line_buffer[ind]!=44){
            op1_buffer[av]=Line_buffer[ind];
            ind++;
            av++;
            }
        ind++;
        av=0;
        while(Line_buffer[ind]!='\0'&&Line_buffer[ind]!='\n'&&Line_buffer[ind]!='\r'&&Line_buffer[ind]!=44){
            op2_buffer[av]=Line_buffer[ind];
            ind++;
            av++;
            }
        ind++;
        av=0;
        while(Line_buffer[ind]!='\0'&&Line_buffer[ind]!='\n'&&Line_buffer[ind]!='\r'&&Line_buffer[ind]!=44){
            op3_buffer[av]=Line_buffer[ind];
            ind++;
            av++;
            }
        //printf("\n--%s--%s--%s--%s--\n",inst_buffer,op1_buffer,op2_buffer,op3_buffer);
        memset(Line_buffer, '\0', STRSZ);
                //Analisis
        if (opN=isDeclare(inst_buffer)){
            if (!(isRepeated(op1_buffer))){
                fprintf(out,inst[opN-1],op1_buffer);
                strcpy(idList[posId],op1_buffer);
                posId++;
                }
        }else if (opN=isStore(inst_buffer)){
			if (isRepeated(op1_buffer))
                getDirecc(op1_temp,op1_buffer);
            else
                strcpy(op1_temp,op1_buffer);
            fprintf(code,inst[opN-1],op1_temp,op2_buffer);
        }else if (opN=isRead(inst_buffer)){
            fprintf(code,inst[opN-1],op1_buffer);
        }else if (opN=isWrite(inst_buffer)){
            fprintf(code,inst[opN-1],op1_buffer);
        }else if (opN=isSub(inst_buffer)){
            if (isRepeated(op1_buffer))
                getDirecc(op1_temp,op1_buffer);
            else
                strcpy(op1_temp,op1_buffer);
            if (isRepeated(op2_buffer))
                getDirecc(op2_temp,op2_buffer);
            else
                strcpy(op2_temp,op2_buffer);
            fprintf(code,inst[opN-1],op1_temp,op2_temp,op3_buffer);
        }else if (opN=isAdd(inst_buffer)){
            if (isRepeated(op1_buffer))
                getDirecc(op1_temp,op1_buffer);
            else
                strcpy(op1_temp,op1_buffer);
            if (isRepeated(op2_buffer))
                getDirecc(op2_temp,op2_buffer);
            else
                strcpy(op2_temp,op2_buffer);
            fprintf(code,inst[opN-1],op1_temp,op2_temp,op3_buffer);
        }else if (opN=isHalt(inst_buffer)){
            fprintf(code,inst[opN-1]);
            printf(inst[opN-1]);
        }else if (opN=isIf(inst_buffer)){
            varN=opN-8000;
            opN=opN/1000;
            fprintf(code,inst[opN-1],op1_buffer,IfCont,IfLev);
            if(IfLev==0)
                IfCont++;
            IfLev++;
        }else if (opN=isThen(inst_buffer)){
            fprintf(code,inst[opN-1],op1_buffer,IfCont,IfLev);
        }else if (opN=isElse(inst_buffer)){
            fprintf(code,inst[opN-1],op1_buffer,IfCont,IfLev);
        }else if (opN=isEndif(inst_buffer)){
            fprintf(code,inst[opN-1],op1_buffer,IfCont,IfLev);
            IfLev--;
        }else if (strcmp(inst_buffer,"")){
            printf("Traduction complete\n");
            break;
        }else{
            printf("Error! Unknown instruction\n");
            break;
        }
    }
    fprintf(out,header_code);
    rewind(code);
    c = fgetc(code);
    while (c != EOF)
    {
        fputc(c, out);
        c = fgetc(code);
    }
}

int isDeclare(const char *in){
    int resp=1;
    if(strcmp(in,"Declare"))
        resp=0;
    return resp;
}
int isStore(const char *in){
    int resp=2;
    if(strcmp(in,"Store"))
        resp=0;
    return resp;
}
int isRead(const char *in){
    int resp=3;
    if(strcmp(in,"Read"))
        resp=0;
    return resp;
}
int isWrite(const char *in){
    int resp=4;
    if(strcmp(in,"Write"))
        resp=0;
    return resp;
}
int isSub(const char *in){
    int resp=5;
    if(strcmp(in,"Sub"))
        resp=0;
    return resp;
}
int isAdd(const char *in){
    int resp=6;
    if(strcmp(in,"Add"))
        resp=0;
    return resp;
}
int isHalt(const char *in){
    int resp=7;
    if(strncmp(in,"Halt",4))
        resp=0;
    return resp;
}
int isIf(const char *in){//8000+varN
    int resp=0;
    int var=0;
    char *temp=in;
    if(!(strncmp(in,"IF_",3))){
        resp=8000;
        temp+=3;
        var = atoi(temp);
        resp+=var;
    }
    return resp;
}
int isThen(const char *in){
    int resp=9;
    if(strncmp(in,"THEN_",5))
        resp=0;
    return resp;
}
int isElse(const char *in){
    int resp=10;
    if(strncmp(in,"ELSE_",5))
        resp=0;
    return resp;
}
int isEndif(const char *in){
    int resp=11;
    if(strncmp(in,"ENDIF_",6))
        resp=0;
    return resp;
}
int isRepeated(const char *in){
    int i=0;
    while (i<posId && strcmp(idList[i],in))
        i++;
    if(i==posId)
        return 0;
    return 1;
}
void getDirecc(char op1_temp[],char op1_buffer[]){
    int optemp=32;
    while(optemp>=0){
        op1_temp[optemp+1]=op1_buffer[optemp];
        optemp--;
    }
    op1_temp[0]='[';
    optemp=0;
    while(op1_temp[optemp]!='\0')
        optemp++;
    op1_temp[optemp]=']';
}

int main(){
    const char*input=   "bin/salida.micro";
    const char*output=  "bin/salida.asm";
    Ensamblar(input,output);
    return 1;
}
