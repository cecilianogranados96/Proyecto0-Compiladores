.globl main
.text
 main:
lw $t0,BB
li $t1,314Temp&1
sub $t2,$t1, $t0
sw $t2,Declare
lw $t0,Temp&1
lw $t1,A
add $t2,$t1, $t0
sw $t2,Temp&2
lw $t0,Temp&2 
sw $t0,A 
li $v0, 10
 syscall 
.data
A : .word 24 
BB : .word 24 
Temp&1 : .word 24 
