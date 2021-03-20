/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "tables.h"
#include "translate_utils.h"
#include "translate.h"

/* define macro for some numbers */
#define INT12_MAX (2047)
#define INT12_MIN (-2048)
/*UINT12 MAX*/
#define UINT12_MAX (4095)
/*bounds for INT20*/
#define INT20_MAX (524287)
#define INT20_MIN (-524288)
/*UINT20 MAX*/
#define UINT20_MAX (1048575)

/* Writes instructions during the assembler's first pass to OUTPUT. The case
   for general instructions has already been completed, but you need to write
   code to translate the li, bge and mv pseudoinstructions. Your
   pseudoinstruction expansions should not have any side effects.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.

   Error checking for regular instructions are done in pass two. However, for
   pseudoinstructions, you must make sure that ARGS contains the correct number
   of arguments. You do NOT need to check whether the registers / label are
   valid, since that will be checked in part two.

   Also for li:
    - make sure that the number is representable by 32 bits. (Hint: the number
        can be both signed or unsigned).
    - if the immediate can fit in the imm field of an addiu instruction, then
        expand li into a single addi instruction. Otherwise, expand it into
        a lui-addi pair.

   And for bge and move:
    - your expansion should use the fewest number of instructions possible.

   venus has slightly different translation rules for li, and it allows numbers
   larger than the largest 32 bit number to be loaded with li. You should follow
   the above rules if venus behaves differently.

   Use fprintf() to write. If writing multiple instructions, make sure that
   each instruction is on a different line.

   Returns the number of instructions written (so 0 if there were any errors).
 */
static int translate_num_unchecked(long int *output, const char *str) {
  long res = 0;
  /*char * after parsing strtol*/
  char *end = NULL;
  if (!str || !output) {
    return -1;
  }
  /* YOUR CODE HERE */
  *output = res = strtol(str, &end, 0);
  if (*end != '\0') return -1; 
  return 0;
}

/*check if str is hex*/
static int isHex(const char *str) {
  return strlen(str) > 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X');
}

/*translate 32-bit int*/
static int translate_num32(long *output, const char *str) {
  long res = 0;
  /*try translate failed*/
  if (translate_num_unchecked(&res, str) == -1) return -1;
  if (isHex(str)) {
    /*hex value*/
    if (res > UINT_MAX || res < 0) return -1;
    /*> 0xffffffff or < 0*/
    if (res > INT_MAX) res = (res - UINT_MAX) - 1;
  }
  /*decimal value*/
  if (res > INT_MAX || res < INT_MIN) return -1;
  *output = res;
  /*success*/
  return 0;
}

/*translate 12-bit int*/
static int translate_num12(long *output, const char *str) {
  long res = 0;
  /*try translate failed*/
  if (translate_num_unchecked(&res, str) == -1) return -1;
  if (isHex(str)) {
    /*hex value*/
    if ((res > UINT12_MAX || res < 0) && translate_num32(&res, str) == -1)
      return -1;
    /*> 0xffffffff or < 0*/
    if (res > INT12_MAX) res -= 2 * 2048;
  }
  /*decimal value*/
  if (res > INT12_MAX || res < INT12_MIN) return -1;
  *output = res;
  /*success*/
  return 0;
}

/*write insts for pass one*/
unsigned write_pass_one(FILE *output, const char *name, char **args,
                        int num_args) {
  long imm;
  /*declare imm, upper_imm, and buffers*/
  unsigned int upper_imm;
  char *arg_buf[3], buf[1024];
  /*null pointers*/
  if (!output || !name || !args) return 0;

  /* Deal with pseudo-instructions. */
  if (strcmp(name, "li") == 0) {
    /* YOUR CODE HERE */
    if (num_args != 2) return 0;
    if (translate_num32(&imm, args[1]) == -1) {
      /*imm > 0xffffffff or imm too small*/
      if (imm > UINT32_MAX || imm < (long)INT32_MIN) return 0;
      /*trans to int*/
      imm = (int)imm;
    }

    /*if ((unsigned long)imm <= UINT12_MAX) imm -= (imm >> 11 & 1) * 2 * 2048;*/
    if (INT12_MIN <= imm && imm <= INT12_MAX) {
      /*with in 12 bits just use addi*/
      arg_buf[0] = args[0];
      arg_buf[1] = "x0";
      arg_buf[2] = args[1];
      /*use addi*/
      write_inst_string(output, "addi", arg_buf, 3);
      return 1;
    }
    /*first use lui, if sign of 12 is 1 then should add 1*/
    upper_imm = (imm >> 12 & 0xFFFFF) + (imm >> 11 & 1);
    sprintf(buf, "%u", upper_imm);
    arg_buf[0] = args[0];
    arg_buf[1] = buf;
    /*write lui*/
    write_inst_string(output, "lui", arg_buf, 2);
    /*get the 12 bits imm*/
    imm &= 0xFFF;
    /*trans to signed*/
    imm -= (imm >> 11 & 1) * 2 * 2048;
    sprintf(buf, "%d", (int)imm);
    arg_buf[0] = arg_buf[1] = args[0];
    arg_buf[2] = buf;
    /*write addi*/
    write_inst_string(output, "addi", arg_buf, 3);
    return 2;
  } else if (strcmp(name, "beqz") == 0) {
    /* YOUR CODE HERE */
    if (num_args != 2) return 0;
    arg_buf[0] = args[0];
    arg_buf[1] = "x0";
    arg_buf[2] = args[1];
    /*beqz xxx, label -> beq xxx, x0, label*/
    write_inst_string(output, "beq", arg_buf, 3);
    return 1;
  } else if (strcmp(name, "mv") == 0) {
    /* YOUR CODE HERE */
    if (num_args != 2) return 0;
    arg_buf[0] = args[0];
    arg_buf[1] = args[1];
    arg_buf[2] = "0";
    /*mv a, b -> addi a, b, 0*/
    write_inst_string(output, "addi", arg_buf, 3);
    return 1;
  } else if (strcmp(name, "j") == 0) {
    /*invalid args*/
    if (num_args != 1) return 0;
    arg_buf[0] = "zero";
    arg_buf[1] = args[0];
    /*j label -> jal x0, label*/
    write_inst_string(output, "jal", arg_buf, 2);
    return 1;
  } else if (strcmp(name, "jr") == 0) {
    /*invalid args*/
    if (num_args != 1) return 0;
    arg_buf[0] = "zero";
    arg_buf[1] = args[0];
    arg_buf[2] = "0";
    /*jr x -> jalr x0, x, 0*/
    write_inst_string(output, "jalr", arg_buf, 3);
    return 1;
  } else if (strcmp(name, "jal") == 0) {
    /*jal x, label*/
    if (num_args == 2) {
      write_inst_string(output, "jal", args, 2);
      return 1;
    }
    if (num_args != 1) return 0;
    /*jal label*/
    arg_buf[0] = "ra";
    arg_buf[1] = args[0];
    /*jal label -> jal ra, label*/
    write_inst_string(output, "jal", arg_buf, 2);
    return 1;
  } else if (strcmp(name, "jalr") == 0) {
    /*jalr a, b, offset*/
    if (num_args == 3) {
      write_inst_string(output, "jalr", args, 3);
      return 1;
    }
    if (num_args != 1) return 0;
    /*jalr x*/
    arg_buf[0] = "ra";
    arg_buf[1] = args[0];
    arg_buf[2] = "0";
    /*jalr x -> jalr ra, x, 0*/
    write_inst_string(output, "jalr", arg_buf, 3);
    return 1;
  } else {
    /*others*/
    write_inst_string(output, name, args, num_args);
    return 1;
  }
  return 0;
}

/* Writes the instruction in hexadecimal format to OUTPUT during pass #2.

   NAME is the name of the instruction, ARGS is an array of the arguments, and
   NUM_ARGS specifies the number of items in ARGS.

   The symbol table (SYMTBL) is given for any symbols that need to be resolved
   at this step.

   You must perform error checking on all instructions and make sure that their
   arguments are valid. If an instruction is invalid, you should not write
   anything to OUTPUT but simply return -1. venus may be a useful resource for
   this step.

   Note the use of helper functions. Consider writing your own! If the function
   definition comes afterwards, you must declare it first (see translate.h).

   Returns 0 on success and -1 on error.
 */
int translate_inst(FILE *output, const char *name, char **args, size_t num_args,
                   uint32_t addr, SymbolTable *symtbl) {
  /* R-format */
  /* add */
  if (strcmp(name, "add") == 0)
    return write_rtype(0x33, 0x00, 0x00, output, args, num_args);
  /* or */
  else if (strcmp(name, "or") == 0)
    return write_rtype(0x33, 0x06, 0x00, output, args, num_args);
  /* sll */
  else if (strcmp(name, "sll") == 0)
    return write_rtype(0x33, 0x01, 0x00, output, args, num_args);
  /* slt */
  else if (strcmp(name, "slt") == 0)
    return write_rtype(0x33, 0x02, 0x00, output, args, num_args);
  /* sltu */
  else if (strcmp(name, "sltu") == 0)
    return write_rtype(0x33, 0x03, 0x00, output, args, num_args);
  /* YOUR CODE HERE */
  /* I-format */
  /* addi */
  else if (strcmp(name, "addi") == 0)
    return write_itype(0x13, 0x00, output, args, num_args);
  /* ori */
  else if (strcmp(name, "ori") == 0)
    return write_itype(0x13, 0x06, output, args, num_args);
  /* lb */
  else if (strcmp(name, "lb") == 0)
    return write_itype(0x03, 0x00, output, args, num_args);
  /* lbu */
  else if (strcmp(name, "lbu") == 0)
    return write_itype(0x03, 0x04, output, args, num_args);
  /* lw */
  else if (strcmp(name, "lw") == 0)
    return write_itype(0x03, 0x02, output, args, num_args);
  /* jalr */
  else if (strcmp(name, "jalr") == 0)
    return write_itype(0x67, 0x00, output, args, num_args);
  /* S-format */
  /* sw */
  else if (strcmp(name, "sw") == 0)
    return write_stype(0x23, 0x02, output, args, num_args);
  /* sb */
  else if (strcmp(name, "sb") == 0)
    return write_stype(0x23, 0x00, output, args, num_args);
  /* SB-format */
  /* beq */
  else if (strcmp(name, "beq") == 0)
    return write_sbtype(0x63, 0x00, output, args, num_args, addr, symtbl);
  /* bne */
  else if (strcmp(name, "bne") == 0)
    return write_sbtype(0x63, 0x01, output, args, num_args, addr, symtbl);
  /* blt */
  else if (strcmp(name, "blt") == 0)
    return write_sbtype(0x63, 0x04, output, args, num_args, addr, symtbl);
  /* bge */
  else if (strcmp(name, "bge") == 0)
    return write_sbtype(0x63, 0x05, output, args, num_args, addr, symtbl);
  /* U-format */
  /* lui */
  else if (strcmp(name, "lui") == 0)
    return write_utype(0x37, output, args, num_args);
  /* UJ-format */
  /* jal */
  else if (strcmp(name, "jal") == 0)
    return write_ujtype(0x6f, output, args, num_args, addr, symtbl);
  /* exceptions */
  else
    return -1;
}

/* A helper function for writing most R-type instructions. You should use
   translate_reg() to parse registers and write_inst_hex() to write to
   OUTPUT. Both are defined in translate_utils.h.

   This function is INCOMPLETE. Complete the implementation below. You will
   find bitwise operations to be the cleanest way to complete this function.
 */
int write_rtype(uint8_t opcode, uint8_t funct3, uint8_t funct7, FILE *output,
                char **args, size_t num_args) {
  /*  Perhaps perform some error checking? */
  int rd, rs, rt;
  uint32_t instruction = 0;

  /* exceptions */
  if (num_args != 3) return -1;

  /* get r* */
  rd = translate_reg(args[0]);
  rs = translate_reg(args[1]);
  rt = translate_reg(args[2]);
  /* REMOVE THE FOLLOWING LINE. IT'S FOR MAKING YOUR CODE TO COMPILE */
  /* printf("%d %d %d\n", rd, rs, rt); */

  /* YOUR CODE HERE */
  /* error checking for register ids */
  if (rd == -1 || rs == -1 || rt == -1) return -1;
  /* generate instruction */
  instruction = opcode | (rd << 7) | (funct3 << 12) | (rs << 15) | (rt << 20) |
                (funct7 << 25);
  /*write insts*/
  write_inst_hex(output, instruction);
  return 0;
}

int write_itype(uint8_t opcode, uint8_t funct3, FILE *output, char **args,
                size_t num_args) {
  /* YOUR CODE HERE */

  int rd, rs, imm_valid;
  long imm;
  /* declare instruction, imm */
  uint32_t instruction;
  /* exceptions */
  if (num_args != 3) return -1;

  /* destination register */
  rd = translate_reg(args[0]);
  if (opcode == 0x03) {
    /* source register */
    rs = translate_reg(args[2]);
    /* immediate */
    imm_valid = translate_num12(&imm, args[1]);
  } else {
    /* source register */
    rs = translate_reg(args[1]);
    /* immediate */
    imm_valid = translate_num12(&imm, args[2]);
  }
  /* error checking for register ids and number validation */
  if (rd == -1 || rs == -1 || imm_valid == -1) return -1;
  /* generate instruction */
  instruction = opcode | (rd << 7) | (funct3 << 12) | (rs << 15) | (imm << 20);
  /*write insts*/
  write_inst_hex(output, instruction);
  return 0;
}

int write_stype(uint8_t opcode, uint8_t funct3, FILE *output, char **args,
                size_t num_args) {
  /* YOUR CODE HERE */
  int rs1, rs2, imm_valid;
  uint32_t instruction = 0;
  /* declare instruction, imm */
  long imm;
  /* exceptions */
  if (num_args != 3) return -1;
  /* source register */
  rs2 = translate_reg(args[0]);
  /* immediate */
  imm_valid = translate_num12(&imm, args[1]);
  /** source register */
  rs1 = translate_reg(args[2]);
  /* error checking for register ids and number validation */
  if (rs2 == -1 || imm_valid == -1 || rs1 == -1) return -1;
  /* generate instruction */
  instruction = opcode | ((imm & 0x1F) << 7) | (funct3 << 12) | (rs1 << 15) |
                (rs2 << 20) | ((imm >> 5) << 25);
  write_inst_hex(output, instruction);
  return 0;
}

/* Hint: the way for branch to calculate relative address. e.g. bne
     bne rs rt label
   assume the byte_offset(addr) of label is L,
   current instruction byte_offset(addr) is A
   the relative address I for label satisfy:
     L = A + I
   so the relative addres is
     I = L - A              */
int write_sbtype(uint8_t opcode, uint8_t funct3, FILE *output, char **args,
                 size_t num_args, uint32_t addr, SymbolTable *symtbl) {
  /* YOUR CODE HERE */
  int rs1, rs2;
  uint32_t instruction;
  int64_t label;
  /* declare instruction, imm */
  long imm;
  /* exceptions */
  if (num_args != 3) return -1;
  /* source register */
  rs1 = translate_reg(args[0]);
  rs2 = translate_reg(args[1]);
  /* error checking for register ids and number validation */
  if (rs1 == -1 || rs2 == -1 ||
      (label = get_addr_for_symbol(symtbl, args[2])) == -1)
    return -1;
  if (label < 0) return -1;
  /* immediate */
  imm = label - addr;
  /** if imm is out of range of 12-bits */
  if (imm > INT12_MAX || imm < INT12_MIN) return -1;
  /* generate instruction */
  instruction = opcode | ((imm >> 11 & 1) << 7) | ((imm >> 1 & 0xF) << 8) |
                (funct3 << 12) | (rs1 << 15) | (rs2 << 20) |
                ((imm >> 5 & 0x3F) << 25) | ((imm >> 12 & 1) << 31);
  /*write insts*/
  write_inst_hex(output, instruction);
  return 0;
}

int write_utype(uint8_t opcode, FILE *output, char **args, size_t num_args) {
  /* YOUR CODE HERE */
  /* declare instruction, imm */
  int rd, imm_valid;
  long imm;
  uint32_t instruction;
  /* exceptions */
  if (num_args != 2) return -1;
  /* source register */
  rd = translate_reg(args[0]);
  /* immediate */
  imm_valid = translate_num(&imm, args[1], 0, UINT20_MAX);
  /* error checking for register ids and number validation */
  if (rd == -1 || imm_valid == -1) return -1;
  /* generate instruction */
  instruction = opcode | (rd << 7) | (imm << 12);
  write_inst_hex(output, instruction);
  return 0;
}

/* In this project there is no need to relocate labels,
   you may think about the reasons. */
int write_ujtype(uint8_t opcode, FILE *output, char **args, size_t num_args,
                 uint32_t addr, SymbolTable *symtbl) {
  /* YOUR CODE HERE */
  int rd;
  int64_t label;
  long imm;
  /* declare instruction, imm */
  uint32_t instruction;
  /* exceptions */
  if (num_args != 2) return -1;
  /* source register */
  rd = translate_reg(args[0]);
  /* error checking for register ids and number validation */
  if (rd == -1 || (label = get_addr_for_symbol(symtbl, args[1])) == -1)
    return -1;
  if (label < 0) return -1;
  /* immediate */
  imm = (label - addr);
  /** if imm is out of range of 20-bits */
  if ((imm >> 1) < INT20_MIN || (imm >> 1) > INT20_MAX) return -1;
  /* generate instruction */
  instruction = opcode | (rd << 7) | ((imm >> 12 & 0xFF) << 12) |
                ((imm >> 11 & 1) << 20) | ((imm >> 1 & 0x3FF) << 21) |
                ((imm >> 20 & 1) << 31);
  /*write insts*/
  write_inst_hex(output, instruction);
  return 0;
}
