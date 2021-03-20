# Lab3

## Exercise 1

1  
`.data` data section  
`.word` 32-bit words  
`.text` code section

2  
34  
9th fibonacci number (starting from 0, i.e. 0, 1, 1, 2, 3, 5, 8, 13, 21, 34)

3  
0x10000010

4  
change `t3` to `0xD` after `lw t3, 0(t3)`.

## Exercise 2

`k` -> `t0`

`source` -> `t1`  
`dest` -> `t2`

```
loop:
	slli t3, t0, 2
	add t4, t1, t3
	lw t5, 0(t4)
	beq t5, x0, exit
	add t6, t2, t3
	sw t5, 0(t6)
	addi t0, t0, 1
	jal x0, loop
exit:
```

`t3 = t0 << 2 = t0 * 4`, `t4 = t1 + t3`  
`address = start + offset * 4`

### A C perspective of pointer in assembly:

All pointers could be stored in a register, seeing that their sizes are exactly 4 bytes on 32-bit system.

Pointers in assembly are just like `char*`/`unsigned char*` in C. Adding one to a pointer (i.e. p+1) means moving one byte forward(i.e. p+1).

