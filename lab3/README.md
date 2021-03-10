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
`data` -> `t2`

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
