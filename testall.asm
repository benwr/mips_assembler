add $t1, $00, $29 #somethingorother
addi $t2, $10, -4
addiu $t3, $ra, 0x10# 
addu $ra, $k1, $v1
and $ra, $k1, $v1
label: andi $ra, $k1, 100
beq $s1, $a1, label
bne $s4, $2, a
jr $2
lbu $s1, 30($t2 )
lhu $t4, -5($13)
ll $0, 0x60($3)
lui $s2, 0x300
lw $t1, -0x30($t1)
slt $t0, $t1, $t2
slti $s0, $s1, 40
mult $s0, $s1
sltiu $s0, $t0, 0x100
sltu $t0, $s7, $10
sll $3, $2, 0x10
srl $t0, $zero, 0x5
sb $t0, 0($3)
sw $t7, 0($t6)
sub $31, $30, $15
subu $31, $30, $15
a:
