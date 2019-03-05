section .data
    msgIn    db      'Insert number: ', 0
    msgOut    db      'Result: ', 0
section .bss
    num resb 11
    NUM1 resd 1
    RESULTADO resd 1
    Temp_1 resd 1
    RESULTADO1 resd 1
    Temp_2 resd 1
    RESULTADO2 resd 1
    Temp_4 resd 1
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
    mov [NUM1], eax
    mov eax, [NUM1]
    mov ebx, [NUM1]
    add eax, ebx 
    mov [Temp_1], eax 
    mov eax, [Temp_1]
    mov [RESULTADO], eax
    mov eax, [RESULTADO]
    mov ebx, [RESULTADO]
    add eax, ebx 
    mov [Temp_2], eax 
    mov eax, [Temp_2]
    mov [RESULTADO1], eax
    mov eax, 800
    mov ebx, [RESULTADO]
    add eax, ebx 
    mov [Temp_4], eax 
    mov eax, [Temp_4]
    mov [RESULTADO2], eax
    mov eax, msgOut
    call sprint
    mov eax, [RESULTADO]
    call itoa
    mov eax, msgOut
    call sprint
    mov eax, [RESULTADO1]
    call itoa
    mov eax, msgOut
    call sprint
    mov eax, [RESULTADO2]
    call itoa
    call quit 
