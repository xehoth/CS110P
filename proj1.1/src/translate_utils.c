/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "translate_utils.h"

void write_inst_string(FILE *output, const char *name, char **args,
                       int num_args) {
  int i;
  /*write name*/
  fprintf(output, "%s", name);
  /*write args*/
  for (i = 0; i < num_args; i++) {
    fprintf(output, " %s", args[i]);
  }
  /*write end line*/
  fprintf(output, "\n");
}

void write_inst_hex(FILE *output, uint32_t instruction) {
  /*write hex with 8 width*/
  fprintf(output, "%08x\n", instruction);
}

int is_valid_label(const char *str) {
  int first = 1;
  /*check null pinter*/
  if (!str) {
    return 0;
  }
  /*loop through str*/
  while (*str) {
    if (first) {
      if (!isalpha((int)*str) && *str != '_') {
        return 0; /* does not start with letter or underscore */
      } else {
        first = 0;
      }
    } else if (!isalnum((int)*str) && *str != '_') {
      return 0; /* subsequent characters not alphanumeric */
    }
    str++;
  }
  return first ? 0 : 1; /* empty string is invalid  */
}

/* Translate the input string into a signed number. The number is then
   checked to be within the correct range (note bounds are INCLUSIVE)
   ie. NUM is valid if LOWER_BOUND <= NUM <= UPPER_BOUND.

   The input may be in either positive or negative, and be in either
   decimal or hexadecimal format. It is also possible that the input is not
   a valid number. Fortunately, the library function strtol() can take
   care of all that (with a little bit of work from your side of course).
   Please read the documentation for strtol() carefully. Do not use strtoul()
   or any other variants.

   You should store the result into the location that OUTPUT points to. The
   function returns 0 if the conversion proceeded without errors, or -1 if an
   error occurred.
 */
int translate_num(long int *output, const char *str, long int lower_bound,
                  long int upper_bound) {
  long res = 0;
  /*char * after parsing strtol*/
  char *end = NULL;
  if (!str || !output) {
    return -1;
  }
  /* YOUR CODE HERE */
  res = strtol(str, &end, 0);
  /*check if LOWER_BOUND <= NUM <= UPPER_BOUND*/
  if (lower_bound <= res && res <= upper_bound && *end == '\0') {
    *output = res;
    return 0;
  }
  /*failed*/
  return -1;
}
/* Translates the register name to the corresponding register number. Please
   see the RISC-V Green Sheet for information about register numbers.

   Returns the register number of STR or -1 if the register name is invalid.
 */
int translate_reg(const char *str) {
  /*translate REG/NAME -> VAL*/
#define TRANS_REG(REG, NAME, VAL) \
  else if (strcmp(str, REG) == 0 || strcmp(str, NAME) == 0) return VAL
  /*zero*/
  if (strcmp(str, "zero") == 0 || strcmp(str, "x0") == 0)
    return 0;
  /*ra*/
  else if (strcmp(str, "ra") == 0 || strcmp(str, "x1") == 0)
    return 1;
  /*t0*/
  else if (strcmp(str, "t0") == 0 || strcmp(str, "x5") == 0)
    return 5;
  /* YOUR CODE HERE */
  TRANS_REG("x2", "sp", 2);    /*x2 sp*/
  TRANS_REG("x3", "gp", 3);    /*x3 gp*/
  TRANS_REG("x4", "tp", 4);    /*x4 tp*/
  TRANS_REG("x6", "t1", 6);    /*x6 t1*/
  TRANS_REG("x7", "t2", 7);    /*x7 t2*/
  TRANS_REG("x8", "s0", 8);    /*x8 s0*/
  TRANS_REG("x8", "fp", 8);    /*x8 fp*/
  TRANS_REG("x9", "s1", 9);    /*x9 s1*/
  TRANS_REG("x10", "a0", 10);  /*x10 a0*/
  TRANS_REG("x11", "a1", 11);  /*x11 a1*/
  TRANS_REG("x12", "a2", 12);  /*x12 a2*/
  TRANS_REG("x13", "a3", 13);  /*x13 a3*/
  TRANS_REG("x14", "a4", 14);  /*x14 a4*/
  TRANS_REG("x15", "a5", 15);  /*x15 a5*/
  TRANS_REG("x16", "a6", 16);  /*x16 a6*/
  TRANS_REG("x17", "a7", 17);  /*x17 a7*/
  TRANS_REG("x18", "s2", 18);  /*x18 s2*/
  TRANS_REG("x19", "s3", 19);  /*x19 s3*/
  TRANS_REG("x20", "s4", 20);  /*x20 s4*/
  TRANS_REG("x21", "s5", 21);  /*x21 s5*/
  TRANS_REG("x22", "s6", 22);  /*x22 s6*/
  TRANS_REG("x23", "s7", 23);  /*x23 s7*/
  TRANS_REG("x24", "s8", 24);  /*x24 s8*/
  TRANS_REG("x25", "s9", 25);  /*x25 s9*/
  TRANS_REG("x26", "s10", 26); /*x26 s10*/
  TRANS_REG("x27", "s11", 27); /*x27 s11*/
  TRANS_REG("x28", "t3", 28);  /*x28 t3*/
  TRANS_REG("x29", "t4", 29);  /*x29 t4*/
  TRANS_REG("x30", "t5", 30);  /*x30 t5*/
  TRANS_REG("x31", "t6", 31);  /*x31 t6*/
  else return -1;
  /*prevent side effects*/
#undef TRANS_REG
}
