.data
test_input: .word 3 6 7 8

.text
main:
	add t0, x0, x0
	addi t1, x0, 4
	la t2, test_input
main_loop:
	beq t0, t1, main_exit
	slli t3, t0, 2
	add t4, t2, t3
	lw a0, 0(t4)

	addi sp, sp, -20
	sw t0, 0(sp)
	sw t1, 4(sp)
	sw t2, 8(sp)
	sw t3, 12(sp)
	sw t4, 16(sp)

	jal ra, factorial

	lw t0, 0(sp)
	lw t1, 4(sp)
	lw t2, 8(sp)
	lw t3, 12(sp)
	lw t4, 16(sp)
	addi sp, sp, 20

	addi a1, a0, 0
	addi a0, x0, 1
	ecall # Print Result
	addi a1, x0, ' '
	addi a0, x0, 11
	ecall
	
	addi t0, t0, 1
	jal x0, main_loop
main_exit:  
	addi a0, x0, 10
	ecall # Exit

factorial:
	addi t0, x0, 1
	beq a0, t0 fac_return
	begin_fac_stack:
		addi sp, sp, -8
		sw ra, 0(sp)
		sw s0, 4(sp)

	mv s0, a0
	addi a0, a0, -1
	call factorial
	mul s0, s0, a0
	mv a0, s0

	end_fac_stack:
		lw ra, 0(sp)
		lw s0, 4(sp)
		addi sp, sp, 8
	fac_return:
		ret
