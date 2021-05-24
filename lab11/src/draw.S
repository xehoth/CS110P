.section .text
.globl draw
.type draw, @function
draw:
	addi sp, sp, -4
	sw ra, 0(sp)
	li a0, 20
	li a1, 50
	li a2, 20
	li a3, 0x07E0
	call LCD_DrawCircle
	li a0, 5
	li a1, 5
	li a2, 20
	li a3, 20
	li a4, 0xF800
	call LCD_DrawRectangle
	li a0, 40
	li a1, 20
	la a2, 'a'
	li a3, 1
	li a4, 0xFFE0
	call LCD_ShowChar
	lw ra, 0(sp)
	addi sp, sp, 4
	ret