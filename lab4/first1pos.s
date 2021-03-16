.data
shouldben1:	.asciiz "Should be -1, and firstposshift and firstposmask returned: "
shouldbe0:	.asciiz "Should be 0 , and firstposshift and firstposmask returned: "
shouldbe16:	.asciiz "Should be 16, and firstposshift and firstposmask returned: "
shouldbe31:	.asciiz "Should be 31, and firstposshift and firstposmask returned: "

.text
main:
	la	a1, shouldbe31
	jal	print_str
	lui	a1, 0x80000	# should be 31
	jal	first1posshift
	mv	a1, a0
	jal	print_int
	jal	print_space

	lui	a1, 0x80000
	jal	first1posmask
	mv	a1, a0
	jal	print_int
	jal	print_newline

	la	a1, shouldbe16
	jal	print_str
	lui	a1, 0x0010	# should be 16
	jal	first1posshift
	mv	a1, a0
	jal	print_int
	jal	print_space

	lui	a1, 0x0010
	jal	first1posmask
	mv	a1, a0
	jal	print_int
	jal	print_newline

	la	a1, shouldbe0
	jal	print_str
	li	a1, 1		# should be 0
	jal	first1posshift
	mv	a1, a0
	jal	print_int
	jal	print_space

	li	a1, 1
	jal	first1posmask
	mv	a1, a0
	jal	print_int
	jal	print_newline

	la	a1, shouldben1
	jal	print_str
	mv	a1, zero		# should be -1
	jal	first1posshift
	mv	a1, a0
	jal	print_int
	jal	print_space

	mv	a1, zero
	jal	first1posmask
	mv	a1, a0
	jal	print_int
	jal	print_newline

	li	a0, 10
	ecall

first1posshift:
	# ret = -1
	li a0, -1
	beqz a1, first1posshift_return
	# cnt = -1
	li t1, -1
	first1posshift_loop:
		# while (a1 != 0)
		beqz a1, first1posshift_return
		# ret++
		addi t1, t1, 1
		# t0 = a1 & 1
		andi t0, a1, 1
		addi t0, t0, -1
		# if (t0 == 1) -> if (--t0 == 0)
		first1posshift_loop_if_t0_eq1_if:
			bne t0, x0, first1posshift_loop_if_t0_eq1_else
			# ret = cnt
			mv a0, t1
		first1posshift_loop_if_t0_eq1_else:
		# a1 >>= 1
		srli a1, a1, 1
		j first1posshift_loop
	
	first1posshift_return:
	ret


first1posmask:
	# ret = -1
	li a0, -1
	beqz a1, first1posmask_return
	li a0, 31
	# t0 = mask = 0x80000000
	li t0, 0x80000000
	# while (t0 != 0)
	first1posmask_loop:
		beqz t0, first1posmask_return
		# t1 = a1 & mask
		and t1, t0, a1
		# if ((a1 & mask) == mask) -> if(t1 == mask)
		beq t1, t0, first1posmask_return
		# mask >>= 1
		srli t0, t0, 1
		# ret--
		addi a0, a0, -1
		j first1posmask_loop

	first1posmask_return:
	ret

print_int:
	mv	a1, a0
	li	a0, 1
	ecall
	jr	ra

print_str:
	li	a0, 4
	ecall
	jr	ra

print_space:
	li	a1, ' '
	li	a0, 11
	ecall
	jr	ra

print_newline:
	li	a1, '\n'
	li	a0, 11
	ecall
	jr	ra
