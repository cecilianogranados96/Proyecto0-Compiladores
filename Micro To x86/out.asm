.globl main
.text
 main:
li $t0,300400Temp&1
lw $t1,Declare
add $t2,$t1, $t0
sw $t2,Temp&2
lw $t0,Temp&1
li $t1,500Temp&2
sub $t2,$t1, $t0
sw $t2,Declare
lw $t0,Temp&2
lw $t1,B
add $t2,$t1, $t0
sw $t2,Temp&3
lw $t0,Temp&3 
sw $t0,A 
li $v0, 10
 syscall 
.data
A : .word 24 
Temp&1 : .word 24 
Temp&3 : .word 24 
