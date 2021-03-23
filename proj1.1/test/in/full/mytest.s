add t0, t1, t2
or t1, t2, t3
slt zero, ra, sp
sltu gp, tp, t3
sll s0, fp, t4
addi s1, s2, 0xFFFFFFFF
ori s3, s4, 0xFFFFFF12
lui s5, 0xFFFFF
lb s6, -4(s7)
lbu a0, 4(a1)
lw a3, -2048(a4)
sb a5, 2044(a6)
sw a7, 0(s8)
beq s9, s10, test
blt s11, t5, test
bne t6, x0, test
bge x1, x2, test
test:
jal test
jal zero, test
jalr x3
beqz x4, test
mv x5, x6
j test
jr x7
jalr ra, x9, 12
li x8, 0x7FF
li x9, 0x800
li x10, 123134
li x11, 1231313
li x12, 1
add x12, x13, x14
add x15, x16, x17
add x17, x18, x19
add x20, x21, x22
add x23, x24, x25
add x25, x26, x27
add x28, x29, x30
li x31, 0