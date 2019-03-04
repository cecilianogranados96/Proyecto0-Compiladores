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
    mov eax, 700
    mov [A], eax
    mov eax, msgOut
    call sprint
    mov eax, [A]
    call itoa
    call quit 
