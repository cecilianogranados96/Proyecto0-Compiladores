#ifndef ENSAMBLADOR_H_INCLUDED
#define ENSAMBLADOR_H_INCLUDED

const char *header_data ="section .data\n    msgIn    db      'Insert number: ', 0\n    msgOut    db      'Result: ', 0\n";
const char *header_temp ="section .bss\n    num resb 11\n";
const char *header_code ="section .text\n%%include        'functions.asm'\n   global _start\n_start:\n";

const char *inst[11]={"    %s resd 1\n",//declare op
                    "    mov eax, %s\n    mov [%s], eax\n",//store op1,op2: de op1 a op2
                    "    mov eax, msgIn\n    call sprint\n    mov eax, 3\n    mov ebx, 0\n    mov ecx, num\n    mov edx, 11\n    int 80h\n    mov eax, num\n    call atoi\n    mov [%s], eax\n",//read op1 guarda en op1 el input
                    "    mov eax, msgOut\n    call sprint\n    mov eax, [%s]\n    call itoa\n",//write op1 escribe lo que hay en op1
                    "    mov eax, %s\n    mov ebx, %s\n    sub eax, ebx \n    mov [%s], eax \n",//sub op1,op2,op3: op1-op2 a op3
                    "    mov eax, %s\n    mov ebx, %s\n    add eax, ebx \n    mov [%s], eax \n",//add op1,op2,op3: op1+op2 a op3
                    "    call quit \n",//halt
                    "    mov eax, [%s]\n    cmp eax, 0 \n    jz else_N_%d_L_%d \n",//if op1 IfCont IfLev
                    "then_N_%d_L_%d: \n", //then IfCont IfLev
                    "    jmp endif_N_%d_L_%d\nelse_N_%d_L_%d: \n",//else IfCont IfLev
                    "endif_N_%d_L_%d: \n"}; //endif  IfCont IfLev

char idList[200][33];
int posId;

FILE *funcAtoiItoa;
int isDAlr(const char *in);

void Traducir(FILE *in,FILE *out,FILE *code);
void Ensamblar(const char *input,const char *output);
int isDeclare(const char *in);
int isStore(const char *in);
int isRead(const char *in);
int isWrite(const char *in);
int isSub(const char *in);
int isAdd(const char *in);
int isHalt(const char *in);
int isIf(const char *in);//1000+
int isThen(const char *in);
int isElse(const char *in);
int isEndif(const char *in);
void getDirecc(char op1_temp[],char op1_buffer[]);

#endif // ENSAMBLADOR_H_INCLUDED
