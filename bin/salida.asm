section .data
    msgIn    db      'Insert number: ', 0
    msgOut    db      'Result: ', 0
section .bss
    num resb 11
    C resd 1
    B resd 1
    Temp_1 resd 1
section .text
%include        'functions.asm'
   global _start
_start:
    mov eax, msgIn
    call sprint
    mov eax, 3
    mov ebx, 0
    mov ecx, num
    mov edx, 11
    int 80h
    mov eax, num
    call atoi
    mov [C], eax
    mov eax, 300
    mov ebx, [C]
    add eax, ebx 
    mov [Temp_1], eax 
    mov eax, [Temp_1]
    mov [B], eax
    mov eax, msgOut
    call sprint
    mov eax, [B]
    call itoa
    call quit 
