section .data
    msgIn    db      'Insert number: ', 0
    msgOut    db      'Result: ', 0
section .bss
    num resb 11
    A resd 1
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
    mov [A], eax
    mov eax, [A]
    mov ebx, 1
    add eax, ebx 
    mov [A], eax 
    mov eax, msgOut
    call sprint
    mov eax, [A]
    call itoa
    call quit 
