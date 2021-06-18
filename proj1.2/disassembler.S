
#                               Project 1.2 RISC-V disassembler

#   You can suppose you will get the number of machine codes you should disassemble from 
#   "lines_of_machine_codes" and get the 32 bits-machine code from "machine_codes". 
#   Both of them are stored in file "input.S"

#   When we test your project, we will change the input files content.

# DO NOT REMOVE THIS LINE WHEN SUBMITTING
.import input.S

.data
# You can define your own data such as ".word", ".asciiz", ".byte"... here.


.text
.globl main
# print a number
print_int:
  mv a1, a0 # a1 to pass ecall
  li a0, 1 # 1 for print_int
  ecall # call the ecall
  ret # return

# print a space
print_space:
  li a1, 32 # ' ' == 32
  li a0, 11 # 11 for print_character
  ecall # call the ecall
  ret # return

# print a '('
print_left_bracket:
  li a1, 40 # '(' == 40
  li a0, 11 # 11 for print_character
  ecall # call the ecall
  ret # return

# print a ')'
print_right_bracket:
  li a1, 41 # ')' == 41
  li a0, 11 # 11 for print_character
  ecall # call the ecall
  ret # return

# print a '\n'
print_line:
  li a1, 10 # '\n' == 10
  li a0, 11 # 11 for print_character
  ecall # call the ecall
  ret # return
# print a ','
print_comma:
  li a1, ',' # ','
  li a0, 11 # 11 for print_character
  ecall # call the ecall
  ret # return
# print a char
print_char:
  mv a1, a0 # a1 for char
  li a0, 11 # 11 for print_character
  ecall
  ret
# exit with -1
error_exit:
  li a1, -1 # exit status with -1
  li a0, 17 # 17 for exit2
  ecall # ecall

print_reg:
  # a0 for reg number
  addi sp, sp, -8
  sw ra, 0(sp) # save ra
  sw s0, 4(sp) # save s0
  mv s0, a0 # s0 <- a0
  li t0, 0 # x0
  beq s0, t0, reg_x0
  li t0, 1 # x1 ra
  beq s0, t0, reg_x1
  li t0, 2 # x2 sp
  beq s0, t0, reg_x2
  li t0, 3 # x3 gp
  beq s0, t0, reg_x3
  li t0, 4 # x4 tp
  beq s0, t0, reg_x4
  li t0, 5 # x5 t0
  beq s0, t0, reg_x5_x7
  li t0, 6 # x6 t1
  beq s0, t0, reg_x5_x7
  li t0, 7 # x7 t2
  beq s0, t0, reg_x5_x7
  li t0, 8 # s0
  beq s0, t0, reg_x8_x9
  li t0, 9 # s1
  beq s0, t0, reg_x8_x9
  li t0, 10 # a0
  beq s0, t0, reg_x10_x17
  li t0, 11 # a1
  beq s0, t0, reg_x10_x17
  li t0, 12 # a2
  beq s0, t0, reg_x10_x17
  li t0, 13 # a3
  beq s0, t0, reg_x10_x17
  li t0, 14 # a4
  beq s0, t0, reg_x10_x17
  li t0, 15 # a5
  beq s0, t0, reg_x10_x17
  li t0, 16 # a6
  beq s0, t0, reg_x10_x17
  li t0, 17 # a7
  beq s0, t0, reg_x10_x17
  li t0, 18 # s2
  beq s0, t0, reg_x18_x27
  li t0, 19 # s3
  beq s0, t0, reg_x18_x27
  li t0, 20 # s4
  beq s0, t0, reg_x18_x27
  li t0, 21 # s5
  beq s0, t0, reg_x18_x27
  li t0, 22 # s6
  beq s0, t0, reg_x18_x27
  li t0, 23 # s7
  beq s0, t0, reg_x18_x27
  li t0, 24 # s8
  beq s0, t0, reg_x18_x27
  li t0, 25 # s9
  beq s0, t0, reg_x18_x27
  li t0, 26 # s10
  beq s0, t0, reg_x18_x27
  li t0, 27 # s11
  beq s0, t0, reg_x18_x27
  li t0, 28 # t3
  beq s0, t0, reg_x28_x31
  li t0, 29 # t4
  beq s0, t0, reg_x28_x31
  li t0, 30 # t5
  beq s0, t0, reg_x28_x31
  li t0, 31 # t6
  beq s0, t0, reg_x28_x31
  reg_x0: # x0
    li a0, 'x'
    call print_char # x
    li a0, 0
    call print_int # x0
    j print_reg_return # return
  reg_x1: # ra
    li a0, 'r'
    call print_char # r
    li a0, 'a'
    call print_char # ra
    j print_reg_return # return
  reg_x2: # sp
    li a0, 's'
    call print_char # s
    li a0, 'p'
    call print_char # sp
    j print_reg_return # return
  reg_x3: # gp
    li a0, 'g'
    call print_char # g
    li a0, 'p'
    call print_char # gp
    j print_reg_return # return
  reg_x4: # tp
    li a0, 't'
    call print_char # t
    li a0, 'p'
    call print_char # tp
    j print_reg_return # return
  reg_x5_x7: # t0 - t2
    li a0, 't'
    call print_char # t
    addi a0, s0, -5 # [5, 7] -> [0, 2]
    call print_int # tx
    j print_reg_return # return
  reg_x8_x9: # s0 s1
    li a0, 's'
    call print_char # s
    addi a0, s0, -8 # [8, 9] -> [0, 1]
    call print_int # sx
    j print_reg_return # return
  reg_x10_x17: # a0 - a7
    li a0, 'a'
    call print_char # a
    addi a0, s0, -10 # [10, 17] -> [0, 7]
    call print_int # ax
    j print_reg_return # return
  reg_x18_x27: # s2 - s11
    li a0, 's' # x
    call print_char
    addi a0, s0, -16 # [18, 27] -> [2, 11]
    call print_int
    j print_reg_return # return
  reg_x28_x31: # t3 - t6
    li a0, 't' # t
    call print_char
    addi a0, s0, -25 # [28, 31] -> [3, 6]
    call print_int # tx
    j print_reg_return
  # li a0, 120 # 'x'
  # call print_char # putchar('x')
  # mv a0, s0 # a0 reg num
  # call print_int
  print_reg_return:
  lw ra, 0(sp) # reload ra
  lw s0, 4(sp) # reload s0
  addi sp, sp, 8 # free stack
  ret # return

get_rd: # rd
  # a0 ins
  srli a0, a0, 7 # move down
  andi a0, a0, 0x1F # mask = 0b11111
  ret # return

get_funct3: # funct3
  # a0 ins
  srli a0, a0, 12 # move down
  andi a0, a0, 0x7 # mask = 0b111
  ret # return

get_rs1: # rs1
  # a0 ins
  srli a0, a0, 15 # move down
  andi a0, a0, 0x1F # mask = 0b11111
  ret # return

get_rs2: # rs2
  # a0 ins
  srli a0, a0, 20 # move down
  andi a0, a0, 0x1F # mask = 0b11111
  ret # return

get_funct7: # funct7
  # a0 ins
  srli a0, a0, 25 # move down
  andi a0, a0, 0x7F # mask = 0b111 1111
  ret # return

get_imm_itype: # imm
  # a0 ins
  li t0, 0xFFF00000 # mask
  and a0, a0, t0 # get imm
  srai a0, a0, 20 # move down with sign
  ret # return

get_imm_stype: # imm
  # a0 ins
  li t0, 0xFE000F80 # mask = 0b11111110000000000000111110000000
  and a0, a0, t0 # a0 &= mask
  srli t0, a0, 7 # move down imm[4:0]
  andi t0, t0, 0x1F # imm[4:0]
  srai a0, a0, 25 # move down imm[11:5] with sign
  slli a0, a0, 5 # for imm[4:0]
  or a0, a0, t0 # add imm[4:0]
  ret # return

get_imm_sbtype: # imm
  # a0 ins
  mv a1, x0 # a1 = 0
  srli t0, a0, 7 # move down imm[11]
  andi t0, t0, 1 # get imm[11]
  slli t0, t0, 10 # move to imm[11]
  or a1, a1, t0 # a1 = imm[11]
  srli t0, a0, 8 # move down imm[4:1]
  andi t0, t0, 0xF # get imm[4:1]
  or a1, a1, t0 # a1 |= imm[4:1]
  srli t0, a0, 25 # move down imm[10:5]
  andi t0, t0, 0x3F # get imm[10:5] 0b111111
  slli t0, t0, 4 # move to imm[10:5]
  or a1, a1, t0 # a1 |= imm[10:5]
  srai a0, a0, 31 # move down imm[12] with sign
  slli a0, a0, 11 # move to imm[12]
  or a0, a0, a1 # a0 |= a1 (a1 == imm[11:1])
  slli a0, a0, 1 # * 2
  ret # return

get_imm_utype: # imm
  # a0 ins
  srli a0, a0, 12 # move down
  ret # return

get_imm_ujtype: # imm
  # a0 ins
  srai a0, a0, 12 # move down with sign
  li t0, 0x7FE00 # 0b01111111111000000000
  mv a1, x0 # a1 = 0
  and t0, a0, t0 # imm[10:1]
  srli t0, t0, 9 # move down imm[10:1]
  or a1, a1, t0 # a1 = imm[10:1]
  srli t0, a0, 8 # move down imm[11]
  andi t0, t0, 1 # get imm[11]
  slli t0, t0, 10 # move to imm[11]
  or a1, a1, t0 # a1 |= imm[11]
  andi t0, a0, 0xFF # imm[19:12]
  slli t0, t0, 11 # move to imm[19:12]
  or a1, a1, t0 # a1 |= imm[19:12]
  li t0, 0xFFF80000 # mask for imm[20]
  and a0, a0, t0 # a0 = imm[20]
  or a0, a0, a1 # a0 |= a1 (a1 == imm[19:1)
  slli a0, a0, 1 # * 2
  ret # return

call_arithmetic_rtype: # far call
  call arithmetic_rtype
call_ins_lui: # far call ins_lui
  call ins_lui
call_arithmetic_rtype_word: # far call
  call arithmetic_rtype_word
call_ins_branch: # far call ins_branch
  call ins_branch
call_ins_jalr: # far call ins_jalr
  call ins_jalr
call_ins_jal: # far call ins_jal
  call ins_jal
call_ins_env: # far call ins_env
  call ins_env

parse_instruction: # parse one instruction
  addi sp, sp, -32 # alloc stack
  sw ra, 0(sp) # save ra
  sw s0, 4(sp) # save s0
  sw s1, 8(sp) # save s1
  sw s2, 12(sp) # save s2
  sw s3, 16(sp) # save s3
  sw s4, 20(sp) # save s4
  sw s5, 24(sp) # save s5
  sw s6, 28(sp) # svae s6
  mv s4, a0 # s4 instructions
  # s0 rd
  # s1 rs1
  # s2 rs2
  # s3 imm
  # s4 ins
  # s5 funct3
  # s6 funct7
  mv a0, s4 # a0 ins
  call get_rd # get rd
  mv s0, a0 # s0 rd
  mv a0, s4 # a0 ins
  call get_rs1 # get rs1
  mv s1, a0 # s1 rs1
  mv a0, s4 # a0 ins
  call get_rs2 # get rs2
  mv s2, a0 # s2 rs2
  andi t1, s4, 0x7F # t0 = opcode
  li t0, 0x03 # lb, lh, lw, ld, lbu, lhu, lwu
  beq t1, t0, ins_load_xxx
  li t0, 0x13 # addi, slli, slti, sltiu, xori, srli, srai, ori, andi
  beq t1, t0, arithmetic_itype
  li t0, 0x17 # auipc
  beq t1, t0, ins_auipc
  li t0, 0x1B # addiw, slliw, srliw, sraiw
  beq t1, t0, arithmetic_itype_word
  li t0, 0x23 # sb, sh, sw, sd
  beq t1, t0, ins_store_xxx
  li t0, 0x33 # add, sub, sll, slt, sltu, xor, srl, sra, or, and
  beq t1, t0, call_arithmetic_rtype
  li t0, 0x37 # lui
  beq t1, t0, call_ins_lui
  li t0, 0x3B # addw, subw, sllw, srlw, sraw
  beq t1, t0, call_arithmetic_rtype_word
  li t0, 0x63 # beq, bne, blt, bge, bltu, bgeu
  beq t1, t0, call_ins_branch
  li t0, 0x67 # jalr
  beq t1, t0, call_ins_jalr
  li t0, 0x6F # call
  beq t1, t0, call_ins_jal
  li t0, 0x73 # ecall, ebreak
  beq t1, t0, call_ins_env
  # no instructions
  j no_such_instructions
  # s0 rd
  # s1 rs1
  # s2 rs2
  # s3 imm
  # s4 ins
  # s5 funct3
  # s6 funct7
  ins_load_xxx:
    mv a0, s4 # a0 ins
    call get_funct3
    mv s5, a0 # s5 funct3
    mv a0, s4 # a0 ins
    call get_imm_itype # get imm[11:0]
    mv s3, a0 # s3 imm
    li t0, 0x0 # lb
    beq s5, t0, ins_lb
    li t0, 0x1 # lh
    beq s5, t0, ins_lh
    li t0, 0x2 # lw
    beq s5, t0, ins_lw
    li t0, 0x3 # ld
    beq s5, t0, ins_ld
    li t0, 0x4 # lbu
    beq s5, t0, ins_lbu
    li t0, 0x5 # lhu
    beq s5, t0, ins_lhu
    li t0, 0x6 # lwu
    beq s5, t0, ins_lwu
    # no instructions
    j no_such_instructions
    ins_lb: # lb
      li a0, 'l' # l
      call print_char
      li a0, 'b' # lb
      call print_char
      j after_ins_lx # after
    ins_lh: # lh
      li a0, 'l' # l
      call print_char
      li a0, 'h' # lh
      call print_char
      j after_ins_lx # after
    ins_lw: # lw
      li a0, 'l' # l
      call print_char
      li a0, 'w' # lw
      call print_char
      j after_ins_lx # after
    ins_ld: # ld
      li a0, 'l' # l
      call print_char
      li a0, 'd' # ld
      call print_char
      j after_ins_lx # after
    ins_lbu: # lbu
      li a0, 'l' # l
      call print_char
      li a0, 'b' # lb
      call print_char
      li a0, 'u' # lbu
      call print_char
      j after_ins_lx # after
    ins_lhu: # lhu
      li a0, 'l' # l
      call print_char
      li a0, 'h' # lh
      call print_char
      li a0, 'u' # lhu
      call print_char
      j after_ins_lx # after
    ins_lwu: # lwu
      li a0, 'l' # l
      call print_char
      li a0, 'w' # lw
      call print_char
      li a0, 'u' # lwu
      call print_char
      j after_ins_lx # after
    after_ins_lx:
      call print_space # lx 
      mv a0, s0 # rd
      call print_reg # lx rd
      call print_comma # lx rd,
      call print_space # lx rd, 
      mv a0, s3 # imm
      call print_int # lx rd, imm
      call print_left_bracket # lx rd imm(
      mv a0, s1 # rs1
      call print_reg # lx rd, imm(rs1
      call print_right_bracket # lx rd, imm(rs1)
      call print_line # '\n'
  ins_load_xxx_done: # finish ins load xxx
    j parse_instruction_return
  
  arithmetic_itype: # arithmetic itype
    mv a0, s4 # a0 ins
    call get_funct3 # get funct3
    mv s5, a0 # s5 funct3
    mv a0, s4 # a0 ins
    call get_imm_itype # get imm
    mv s3 a0 # s3 imm
    li t0, 0x0 # addi
    beq s5, t0, ins_addi
    li t0, 0x1 # slli
    beq s5, t0, ins_slli
    li t0, 0x2 # slti
    beq s5, t0, ins_slti
    li t0, 0x3 # sltiu
    beq s5, t0, ins_sltiu
    li t0, 0x4 # xori
    beq s5, t0, ins_xori
    li t0, 0x5 # srli srai
    beq s5, t0, ins_srli_srai
    li t0, 0x6 # ori
    beq s5, t0, ins_ori
    li t0, 0x7 # andi
    beq s5, t0, ins_andi
    # no instructions
    j no_such_instructions

    ins_addi: # addi
      li a0, 'a' # a
      call print_char
      li a0, 'd' # ad
      call print_char
      li a0, 'd' # add
      call print_char
      li a0, 'i' # addi
      call print_char
      j after_ins_arith_itype # after
    ins_slli: # slli
      andi t0, s3, -32 # 0b1111111 00000
      bne t0, x0, arithmetic_itype_no_instructions # != 0
      andi s3, s3, 0x1F # mask 0b11111
      li a0, 's' # s
      call print_char
      li a0, 'l' # sl
      call print_char
      li a0, 'l' # sll
      call print_char
      li a0, 'i' # slli
      call print_char
      j after_ins_arith_itype # after
    ins_slti: # slti
      li a0, 's' # s
      call print_char
      li a0, 'l' # sl
      call print_char
      li a0, 't' # slt
      call print_char
      li a0, 'i' # slti
      call print_char
      j after_ins_arith_itype # after
    ins_sltiu: # sltiu
      li a0, 's' # s
      call print_char
      li a0, 'l' # sl
      call print_char
      li a0, 't' # slt
      call print_char
      li a0, 'i' # slti
      call print_char
      li a0, 'u' # sltu
      call print_char
      j after_ins_arith_itype # after
    ins_xori: # xori
      li a0, 'x' # x
      call print_char
      li a0, 'o' # xo
      call print_char
      li a0, 'r' # xor
      call print_char
      li a0, 'i' # xori
      call print_char
      j after_ins_arith_itype # after
    ins_srli_srai:
      andi t0, s3, -32 # 0b1111111 00000
      beqz t0, ins_srli # srli
      li t1, 0x400 # 0b0100000 00000
      beq t0, t1, ins_srai # srai
      # no such instructions
      j no_such_instructions
    ins_srli: # srli
      andi s3, s3, 0x1F # mask = 0b11111
      li a0, 's' # s
      call print_char
      li a0, 'r' # sr
      call print_char
      li a0, 'l' # srl
      call print_char
      li a0, 'i' # srli
      call print_char
      j after_ins_arith_itype # after
    ins_srai:
      andi s3, s3, 0x1F # mask = 0b11111
      li a0, 's' # s
      call print_char
      li a0, 'r' # sr
      call print_char
      li a0, 'a' # sra
      call print_char
      li a0, 'i' # srai
      call print_char
      j after_ins_arith_itype # after
    ins_ori:
      li a0, 'o' # o
      call print_char
      li a0, 'r' # or
      call print_char
      li a0, 'i' # ori
      call print_char
      j after_ins_arith_itype # after
    ins_andi:
      li a0, 'a' # a
      call print_char
      li a0, 'n' # an
      call print_char
      li a0, 'd' # and
      call print_char
      li a0, 'i' # andi
      call print_char
      j after_ins_arith_itype # after
    after_ins_arith_itype:
      call print_space # ins 
      mv a0, s0 # rd
      call print_reg # ins rd
      call print_comma # ins rd,
      call print_space # ins rd, 
      mv a0, s1 # rs1
      call print_reg # ins rd, rs1
      call print_comma # ins rd, rs1,
      call print_space # ins rd, rs1, 
      mv a0, s3 # imm
      call print_int # ins rd, rs1, imm
      call print_line # '\n'
  arithmetic_itype_done: # finish arithmetic itype
    j parse_instruction_return  
  arithmetic_itype_no_instructions: # for far jump
    j no_such_instructions

  ins_auipc: # auipc
    mv a0, s4 # ins
    call get_imm_utype # imm
    mv s3, a0 # s3 imm
    li a0, 'a' # a
    call print_char
    li a0, 'u' # au
    call print_char
    li a0, 'i' # aui
    call print_char
    li a0, 'p' # auip
    call print_char
    li a0, 'c' # auipc
    call print_char
    call print_space # auipc 
    mv a0, s0 # rd
    call print_reg # auipc rd
    call print_comma # auipc rd,
    call print_space # auipc rd, 
    mv a0, s3 # imm
    call print_int # auipc rd, imm
    call print_line # '\n'
    # end
    j parse_instruction_return

  arithmetic_itype_word: # No need to support
  arithmetic_itype_word_done: # finish arithmetic itype word
    j parse_instruction_return

  ins_store_xxx: # store
    mv a0, s4 # a0 ins
    call get_funct3 # get funct3
    mv s5, a0 # s5 funct3
    mv a0, s4 # ins
    call get_imm_stype # imm
    mv s3, a0 # s3 imm
    li t0, 0x0 # sb
    beq s5, t0, ins_sb
    li t0, 0x1 # sh
    beq s5, t0, ins_sh
    li t0, 0x2 # sw
    beq s5, t0, ins_sw
    li t0, 0x3 # sd
    beq s5, t0, ins_sd
    ins_sb: # sb
      li a0, 's' # s
      call print_char
      li a0, 'b' # sb
      call print_char
      j after_ins_sx # after
    ins_sh: # sh
      li a0, 's' # s
      call print_char
      li a0, 'h' # sh
      call print_char
      j after_ins_sx # after
    ins_sw: # sw
      li a0, 's' # s
      call print_char
      li a0, 'w' # sw
      call print_char
      j after_ins_sx # after
    ins_sd: # sd
      li a0, 's' # s
      call print_char
      li a0, 'd' # sd
      call print_char
      j after_ins_sx # after
    after_ins_sx:
      call print_space # sx 
      mv a0, s2 # rs2
      call print_reg # sx rs2
      call print_comma # sx rs2,
      call print_space # sx rs2, 
      mv a0, s3 # imm
      call print_int # sx rs2, imm
      call print_left_bracket # sx rs2, imm(
      mv a0, s1 # rs1
      call print_reg # sx rs2 imm(rs1
      call print_right_bracket # sx rs2, imm(rs1)
      call print_line # '\n'
  ins_store_xxx_done: # finish ins store xxx
    j parse_instruction_return

  arithmetic_rtype: # arithmetic rtype
    mv a0, s4 # a0 ins
    call get_funct3 # get funct3
    mv s5, a0 # s5 funct3
    mv a0, s4 # a0 ins
    call get_funct7 # get funct7
    mv s6, a0 # s6 funct7
    mv a0, s4 # a0 ins
    call get_imm_itype # imm
    mv s3, a0 # s3 imm
    li t0, 0x0 # add sub
    beq s5, t0, ins_add_sub
    li t0, 0x1 # sll
    beq s5, t0, ins_sll
    li t0, 0x2 # slt
    beq s5, t0, ins_slt
    li t0, 0x3 # sltu
    beq s5, t0, ins_sltu
    li t0, 0x4 # xor
    beq s5, t0, ins_xor
    li t0, 0x5 # srl sra
    beq s5, t0, ins_srl_sra
    li t0, 0x6 # or
    beq s5, t0, ins_or
    li t0, 0x7 # and
    beq s5, t0, ins_and
    # no such instructions
    j no_such_instructions
    ins_add_sub: # add sub
      andi t0, s3, -32 # 0b1111111 00000
      beqz t0, ins_add # add
      li t1, 0x400 # 0b0100000 00000
      beq t0, t1, ins_sub # sub
      # no such instructions
      j no_such_instructions
    ins_add: # add
      li a0, 'a' # a
      call print_char
      li a0, 'd' # ad
      call print_char
      li a0, 'd' # add
      call print_char
      j after_ins_arith_rtype # after
    ins_sub: # sub
      li a0, 's' # s
      call print_char
      li a0, 'u' # su
      call print_char
      li a0, 'b' # sub
      call print_char
      j after_ins_arith_rtype # after
    ins_sll: # sll
      andi t0, s3, -32 # 0b1111111 00000
      bne t0, x0, no_such_instructions # != 0
      li a0, 's' # s
      call print_char
      li a0, 'l' # sl
      call print_char
      li a0, 'l' # sll
      call print_char
      j after_ins_arith_rtype # after
    ins_slt: # slt
      andi t0, s3, -32 # 0b1111111 00000
      bne t0, x0, no_such_instructions # != 0
      li a0, 's' # s
      call print_char
      li a0, 'l' # sl
      call print_char
      li a0, 't' # slt
      call print_char
      j after_ins_arith_rtype # after
    ins_sltu: # sltu
      andi t0, s3, -32 # 0b1111111 00000
      bne t0, x0, no_such_instructions # != 0
      li a0, 's' # s
      call print_char
      li a0, 'l' # sl
      call print_char
      li a0, 't' # slt
      call print_char
      li a0, 'u' # sltu
      call print_char
      j after_ins_arith_rtype # after
    ins_xor: # xor
      andi t0, s3, -32 # 0b1111111 00000
      bne t0, x0, no_such_instructions # != 0
      li a0, 'x' # x
      call print_char
      li a0, 'o' # xo
      call print_char
      li a0, 'r' # xor
      call print_char
      j after_ins_arith_rtype # after
    ins_srl_sra: # srl sra
      andi t0, s3, -32 # 0b1111111 00000
      beqz t0, ins_srl # srl
      li t1, 0x400 # 0b0100000 00000
      beq t0, t1, ins_sra # sra
      # no such instructions
      j no_such_instructions
    ins_srl: # srl
      li a0, 's' # s
      call print_char
      li a0, 'r' # sr
      call print_char
      li a0, 'l' # srl
      call print_char
      j after_ins_arith_rtype # after
    ins_sra: # sra
      li a0, 's' # s
      call print_char
      li a0, 'r' # sr
      call print_char
      li a0, 'a' # sra
      call print_char
      j after_ins_arith_rtype # after
    ins_or: # or
      andi t0, s3, -32 # 0b1111111 00000
      bne t0, x0, no_such_instructions # != 0
      li a0, 'o' # o
      call print_char
      li a0, 'r' # or
      call print_char
      j after_ins_arith_rtype # after
    ins_and: # and
      andi t0, s3, -32 # 0b1111111 00000
      bne t0, x0, no_such_instructions # != 0
      li a0, 'a' # a
      call print_char
      li a0, 'n' # an
      call print_char
      li a0, 'd' # and
      call print_char
      j after_ins_arith_rtype # after
    after_ins_arith_rtype:
      call print_space # ins 
      mv a0, s0 # rd
      call print_reg # ins rd
      call print_comma # ins rd,
      call print_space # ins rd, 
      mv a0, s1 # rs1
      call print_reg # ins rd, rs1
      call print_comma # ins rd, rs1,
      call print_space # ins rd, rs1, 
      mv a0, s2 # rs2
      call print_reg # ins rd, rs1, rs2
      call print_line # '\n'
  arithmetic_rtype_done: # finish arithmetic rtype
    j parse_instruction_return
  ins_lui: # lui
    mv a0, s4 # ins
    call get_imm_utype # imm
    mv s3, a0 # s3 imm
    li a0, 'l' # l
    call print_char
    li a0, 'u' # lu
    call print_char
    li a0, 'i' # lui
    call print_char
    call print_space # lui 
    mv a0, s0 # rd
    call print_reg # lui rd
    call print_comma # lui rd,
    call print_space # lui rd, 
    mv a0, s3 # imm
    call print_int # lui rd imm
    call print_line # '\n'
    # end
    j parse_instruction_return
  arithmetic_rtype_word: # No need to support
  arithmetic_rtype_word_done: # finish arithmetic rtype word
    j parse_instruction_return
  ins_branch: # branch
    mv a0, s4 # a0 ins
    call get_funct3 # get funct3
    mv s5, a0 # s5 funct3
    mv a0, s4 # ins
    call get_imm_sbtype # imm
    mv s3, a0 # s3 imm
    li t0, 0x0 # beq
    beq s5, t0, ins_beq
    li t0, 0x1 # bne
    beq s5, t0, ins_bne
    li t0, 0x4 # blt
    beq s5, t0, ins_blt
    li t0, 0x5 # bge
    beq s5, t0, ins_bge
    li t0, 0x6 # bltu
    beq s5, t0, ins_bltu
    li t0, 0x7 # bgeu
    beq s5, t0, ins_bgeu
    # no such instructions
    j no_such_instructions
    ins_beq: # beq
      li a0, 'b' # b
      call print_char
      li a0, 'e' # be
      call print_char
      li a0, 'q' # beq
      call print_char
      j after_ins_branch # after
    ins_bne: # bne
      li a0, 'b' # b
      call print_char
      li a0, 'n' # bn
      call print_char
      li a0, 'e' # bne
      call print_char
      j after_ins_branch # after
    ins_blt: # blt
      li a0, 'b' # b
      call print_char
      li a0, 'l' # bl
      call print_char
      li a0, 't' # blt
      call print_char
      j after_ins_branch # after
    ins_bge: # bge
      li a0, 'b' # b
      call print_char
      li a0, 'g' # bg
      call print_char
      li a0, 'e' # bge
      call print_char
      j after_ins_branch # after
    ins_bltu: # bltu
      li a0, 'b' # b
      call print_char
      li a0, 'l' # bl
      call print_char
      li a0, 't' # blt
      call print_char
      li a0, 'u' # bltu
      call print_char
      j after_ins_branch # after
    ins_bgeu: # bgeu
      li a0, 'b' # b
      call print_char
      li a0, 'g' # bg
      call print_char
      li a0, 'e' # bge
      call print_char
      li a0, 'u' # bgeu
      call print_char
      j after_ins_branch # after
    after_ins_branch:
      call print_space # ins 
      mv a0, s1 # rs1
      call print_reg # ins rs1
      call print_comma # ins rs1,
      call print_space # ins rs1, 
      mv a0, s2, # rs2
      call print_reg # ins rs1, rs2
      call print_comma # ins rs1, rs2,
      call print_space # ins rs1, rs2, 
      mv a0, s3 # imm
      call print_int # ins rs1, rs2, imm
      call print_line # '\n'

  ins_branch_done: # finish branch
    j parse_instruction_return
  ins_jalr: # jalr
    mv a0, s4 # ins
    call get_imm_itype # imm
    mv s3, a0 # s3 imm
    li a0, 'j' # j
    call print_char
    li a0, 'a' # ja
    call print_char
    li a0, 'l' # call
    call print_char
    li a0, 'r' # jalr
    call print_char
    call print_space # jalr 
    mv a0, s0 # rd
    call print_reg # jalr rd
    call print_comma # jalr rd,
    call print_space # jalr rd, 
    mv a0, s1 # rs1
    call print_reg # jalr rd, rs1
    call print_comma # jalr rd, rs1,
    call print_space # jalr rd, rs1, 
    mv a0, s3 # imm
    call print_int # jalr rd, rs1, imm
    call print_line # '\n'
    # end
    j parse_instruction_return
  ins_jal: # jal
    mv a0, s4 # ins
    call get_imm_ujtype # imm
    mv s3, a0 # s3 imm
    li a0, 'j' # j
    call print_char
    li a0, 'a' # ja
    call print_char
    li a0, 'l' # call
    call print_char
    call print_space # call 
    mv a0, s0 # rd
    call print_reg # call rd
    call print_comma # call rd,
    call print_space # call rd, 
    mv a0, s3 # imm
    call print_int # call rd, imm
    call print_line # '\n'
    # end
    j parse_instruction_return
  ins_env: # call, ebreak
    mv a0, s4 # ins
    call get_funct3 # funct3
    mv s5, a0 # s5 funct3
    bne s5, x0, no_such_instructions
    mv a0, s4 # ins
    call get_imm_itype # imm
    mv s3, a0 # s3 imm
    beq s3, x0, ins_ecall
    li t0, 1 # 1
    beq s3, t0, ins_ebreak # ebreak
    # no instructions
    j no_such_instructions
    ins_ecall: # call
      li a0, 'e' # e
      call print_char
      li a0, 'c' # ec
      call print_char
      li a0, 'a' # eca
      call print_char
      li a0, 'l' # ecal
      call print_char
      li a0, 'l' # ecall
      call print_char
      call print_line # '\n'
      # end
      j parse_instruction_return
    ins_ebreak: # ebreak
      li a0, 'e' # e
      call print_char
      li a0, 'b' # eb
      call print_char
      li a0, 'r' # ebr
      call print_char
      li a0, 'e' # ebre
      call print_char
      li a0, 'a' # ebrea
      call print_char
      li a0, 'k' # ebreak
      call print_char
      call print_line # '\n'
      # end
      j parse_instruction_return
  no_such_instructions: # error
    call error_exit
  parse_instruction_return: # return
  lw ra, 0(sp) # reload ra
  lw s0, 4(sp) # reload s0
  lw s1, 8(sp) # reload s1
  lw s2, 12(sp) # reload s2
  lw s3, 16(sp) # reload s3
  lw s4, 20(sp) # reload s4
  lw s5, 24(sp) # reload s5
  lw s6, 28(sp) # reload s6
  addi sp, sp, 32 # free stack
  ret # return

main:
  la t0, lines_of_machine_codes # get line address
  lw s0, 0(t0) # get n
  la s1, machine_codes
  li s2, 0 # i = 0
  main_loop: # loop
    beq s2, s0, main_loop_done # i != n
    lw a0, 0(s1) # ins
    call parse_instruction
    addi s1, s1, 4 # move next
    addi s2, s2, 1 # i++
    j main_loop # continue loop
  main_loop_done:
  li a0, 17 # exit2
  li a1, 0 # exit 0
  ecall