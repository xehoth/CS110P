/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/utils.h"
#include "src/tables.h"
#include "src/translate_utils.h"
#include "src/translate.h"

#include "assembler.h"

#define MAX_ARGS 3
#define BUF_SIZE 1024
const char *IGNORE_CHARS = " \f\n\r\t\v,()";

/*******************************
 * Helper Functions
 *******************************/

/* you should not be calling this function yourself. */
static void raise_label_error(uint32_t input_line, const char *label) {
  write_to_log("Error - invalid label at line %d: %s\n", input_line, label);
}

/* call this function if more than MAX_ARGS arguments are found while parsing
   arguments.

   INPUT_LINE is which line of the input file that the error occurred in. Note
   that the first line is line 1 and that empty lines are included in the count.

   EXTRA_ARG should contain the first extra argument encountered.
 */
static void raise_extra_argument_error(uint32_t input_line,
                                       const char *extra_arg) {
  write_to_log("Error - extra argument at line %d: %s\n", input_line,
               extra_arg);
}

/* You should call this function if write_pass_one() or translate_inst()
   returns -1.

   INPUT_LINE is which line of the input file that the error occurred in. Note
   that the first line is line 1 and that empty lines are included in the count.
 */
static void raise_instruction_error(uint32_t input_line, const char *name,
                                    char **args, int num_args) {
  write_to_log("Error - invalid instruction at line %d: ", input_line);
  log_inst(name, args, num_args);
}

/* Truncates the string at the first occurrence of the '#' character. */
static void skip_comments(char *str) {
  char *comment_start = strchr(str, '#');
  if (comment_start) {
    *comment_start = '\0';
  }
}

/* Reads STR and determines whether it is a label (ends in ':'), and if so,
   whether it is a valid label, and then tries to add it to the symbol table.

   INPUT_LINE is which line of the input file we are currently processing. Note
   that the first line is line 1 and that empty lines are included in this
   count.

   BYTE_OFFSET is the offset of the NEXT instruction (should it exist).

   Four scenarios can happen:
    1. STR is not a label (does not end in ':'). Returns 0.
    2. STR ends in ':', but is not a valid label. Returns -1.
    3a. STR ends in ':' and is a valid label. Addition to symbol table fails.
        Returns -1.
    3b. STR ends in ':' and is a valid label. Addition to symbol table succeeds.
        Returns 1.
 */
static int add_if_label(uint32_t input_line, char *str, uint32_t byte_offset,
                        SymbolTable *symtbl) {
  size_t len = strlen(str);
  /*get the length*/
  if (str[len - 1] == ':') {
    str[len - 1] = '\0';
    /*check if label is valid*/
    if (is_valid_label(str)) {
      if (add_to_table(symtbl, str, byte_offset) == 0) {
        /*success*/
        return 1;
      } else {
        /*failed*/
        return -1;
      }
    } else {
      /*failed*/
      raise_label_error(input_line, str);
      return -1;
    }
  } else {
    /*success*/
    return 0;
  }
}

/*******************************
 * Implement the Following
 *******************************/

/* First pass of the assembler. You should implement pass_two() first.

   This function should read each line, strip all comments, scan for labels,
   and pass instructions to write_pass_one(). The symbol table should also
   been built and written to specified file. The input file may or may not
   be valid. Here are some guidelines:

    1. Only one label may be present per line. It must be the first token
   present. Once you see a label, regardless of whether it is a valid label or
   invalid label, treat the NEXT token as the beginning of an instruction.
    2. If the first token is not a label, treat it as the name of an
   instruction. DO NOT try to check it is a valid instruction in this pass.
    3. Everything after the instruction name should be treated as arguments to
        that instruction. If there are more than MAX_ARGS arguments, call
        raise_extra_argument_error() and pass in the first extra argument. Do
   not write that instruction to the output file (eg. don't call
   write_pass_one())
    4. Only one instruction should be present per line. You do not need to do
        anything extra to detect this - it should be handled by guideline 3.
    5. A line containing only a label is valid. The address of the label should
        be the byte offset of the next instruction, regardless of whether there
        is a next instruction or not.
    6. If an instruction contains an immediate, you should output it AS IS.
    7. Comments should always be skipped before any further process.

   Just like in pass_two(), if the function encounters an error it should NOT
   exit, but process the entire file and return -1. If no errors were
   encountered, it should return 0.
 */
int pass_one(FILE *input, FILE *inter, FILE *symtbl) {
  /* YOUR CODE HERE */

  /* A buffer for line parsing. */
  char buf[BUF_SIZE];

  /* Variables for argument parsing. */
  char *args[MAX_ARGS], *token, *name;
  int num_args;
  /*line_number and byte_offset*/
  uint32_t line_number = 0, byte_offset = 0;
  SymbolTable *table = NULL;
  int err_flag = 0, add_table_flag = 0, write_insts = 0;
  /* For each line, there are some hints of what you should do:
      1. Skip all comments
      2. Use `strtok()` to read next token
      3. Deal with labels
      4. Parse the instruction
   */
  if (!(table = create_table(SYMBOLTBL_UNIQUE_NAME))) return -1;
  /*create a table*/
  while (fgets(buf, BUF_SIZE, input)) {
    /*read each line*/
    ++line_number;
    skip_comments(buf);
    /*skip comment and get the first token*/
    if (*buf == '\0' || !(token = strtok(buf, IGNORE_CHARS))) continue;
    /*try if it is a label*/
    if ((add_table_flag = add_if_label(line_number, token, byte_offset, table)) == -1) {
      /*failed to add*/
      err_flag = 1;
      continue;
    }
    if (add_table_flag == 1) token = strtok(NULL, IGNORE_CHARS);
    /*get token after label -> name*/
    if (!(name = token)) continue;
    for (num_args = 0; (token = strtok(NULL, IGNORE_CHARS));) {
      /*get each args*/
      if (++num_args > MAX_ARGS) {
        err_flag = 1;
        /*too many args*/
        raise_extra_argument_error(line_number, token);
        break;
      }
      /*store token to args*/
      args[num_args - 1] = token;
    }
    /*skip this line because of too many args*/
    if (num_args > MAX_ARGS) continue;
    if (!(write_insts = write_pass_one(inter, name, args, num_args))) {
      /*write instr failed*/
      raise_instruction_error(line_number, name, args, num_args);
      err_flag = 1;
      continue;
    }
    /*byte_offset += 4 * insts*/
    byte_offset += write_insts << 2;
  }
  /*writable*/
  write_table(table, symtbl);
  /*prevent memory leak*/
  free_table(table);
  return -err_flag;
}

/* Second pass of the assembler.

   This function should read an intermediate file and the corresponding symbol
   table file, translates it into machine code. You may assume:
    1. The input file contains no comments
    2. The input file contains no labels
    3. The input file contains at maximum one instruction per line
    4. All instructions have at maximum MAX_ARGS arguments
    5. The symbol table file is well formatted
    6. The symbol table file contains all the symbol required for translation
   If an error is reached, DO NOT EXIT the function. Keep translating the rest
   of the document, and at the end, return -1. Return 0 if no errors were
   encountered. */
int pass_two(FILE *inter, FILE *symtbl, FILE *output) {
  /* YOUR CODE HERE */

  /* A buffer for line parsing. */
  char buf[BUF_SIZE];

  /* Variables for argument parsing. */
  char *args[MAX_ARGS], *token, *name;
  int num_args, err_flag = 0;
  uint32_t line_number = 0, byte_offset = 0;
  SymbolTable *table = NULL;
  /* Write text segment. */
  fprintf(output, ".text\n");

  /* Create symbol table here by reading the symbol table file. */
  if (!(table = create_table_from_file(SYMBOLTBL_UNIQUE_NAME, symtbl))) return -1;
  /* For each line, there are some hints of what you should do:
      1. Get instruction name.
      2. Parse instruction arguments; Extra arguments should be filtered out in
       `pass_one()`, so you don't need to worry about that here.
      3. Use `translate_inst()` to translate the instruction and write to the
       output file;
      4. Or if an error occurs, call `raise_instruction_error()` instead of
     write the instruction.
   */
  for (; fgets(buf, BUF_SIZE, inter); byte_offset += 4) {
    /*loop thorugh each line*/
    ++line_number;
    /*get first token -> name*/
    if (!(name = strtok(buf, IGNORE_CHARS))) continue;
    /*get all args*/
    for (num_args = 0; (token = strtok(NULL, IGNORE_CHARS));) args[num_args++] = token;
    if (translate_inst(output, name, args, num_args, byte_offset, table) == -1) {
      /*translate failed*/
      err_flag = 1;
      raise_instruction_error(line_number, name, args, num_args);
      /*skip this line*/
      continue;
    }
  }
  
  /* Write symbol segment. */
  fprintf(output, "\n.symbol\n");
  /* Write symbols here by calling `write_table()` */
  write_table(table, output);
  free_table(table);
  return -err_flag;
}

/*******************************
 * Do Not Modify Code Below
 *******************************/

static int open_files_pass_one(FILE **input, FILE **inter, FILE **symtbl,
                               const char *input_name, const char *inter_name,
                               const char *symtbl_name) {
  /*open input_name file*/
  *input = fopen(input_name, "r");
  if (!*input) {
    /*failed*/
    write_to_log("Error: unable to open input file: %s\n", input_name);
    return -1;
  }
  /*open inter file*/
  *inter = fopen(inter_name, "w");
  if (!*inter) {
    write_to_log("Error: unable to open intermediate file: %s\n", inter_name);
    /*failed*/
    fclose(*input);
    return -1;
  }
  /*open symbol table file*/
  *symtbl = fopen(symtbl_name, "w");
  if (!*symtbl) {
    write_to_log("Error: unable to open symbol table file: %s\n", symtbl_name);
    /*failed*/
    fclose(*input);
    fclose(*inter);
    return -1;
  }
  /*success*/
  return 0;
}

static int open_files_pass_two(FILE **inter, FILE **symtbl, FILE **output,
                               const char *inter_name, const char *symtbl_name,
                               const char *output_name) {
  /*open inter file*/
  *inter = fopen(inter_name, "r");
  if (!*inter) {
    /*failed*/
    write_to_log("Error: unable to open intermediate file: %s\n", inter_name);
    return -1;
  }
  /*open symbol table file*/
  *symtbl = fopen(symtbl_name, "r");
  if (!*symtbl) {
    write_to_log("Error: unable to open symbol table file: %s\n", inter_name);
    /*failed*/
    fclose(*inter);
    return -1;
  }
  *output = fopen(output_name, "w");
  /*open output_name file*/
  if (!*output) {
    write_to_log("Error: unable to open output file: %s\n", output_name);
    fclose(*inter);
    /*failed*/
    fclose(*symtbl);
    return -1;
  }
  /*success*/
  return 0;
}

static void close_files(FILE *file1, FILE *file2, FILE *file3) {
  /*close files*/
  fclose(file1);
  fclose(file2);
  fclose(file3);
}

/* Runs the two-pass assembler. Most of the actual work is done in pass_one()
   and pass_two().
 */
int assemble(const char *in, const char *out, const char *int_,
             const char *sym) {
  FILE *input, *inter, *symtbl, *output;
  int err = 0;
  /*in is not null*/
  if (in) {
    printf("Running pass one: %s -> %s, %s\n", in, int_, sym);
    if (open_files_pass_one(&input, &inter, &symtbl, in, int_, sym) != 0) {
      exit(1);
    }
    /*try pass one*/
    if (pass_one(input, inter, symtbl) != 0) {
      err = 1;
    }
    /*close files*/
    close_files(input, inter, symtbl);
  }

  if (out) {
    printf("Running pass two: %s, %s -> %s\n", int_, sym, out);
    /*open files for pass two*/
    if (open_files_pass_two(&inter, &symtbl, &output, int_, sym, out) != 0) {
      exit(1);
    }
    /*try pass two*/
    if (pass_two(inter, symtbl, output) != 0) {
      err = 1;
    }
    /*close files*/
    close_files(inter, symtbl, output);
  }

  return err;
}

static void print_usage_and_exit() {
  /*print infos*/
  printf("Usage:\n");
  printf(
      "  Runs both passes: assembler <input file> <intermediate file> <symbol "
      "table file> <output file>\n");
  /*pass 1 usage*/
  printf(
      "  Run pass #1:      assembler -p1 <input file> <intermediate file> "
      "<symbol table file>\n");
  /*pass 2 usage*/
  printf(
      "  Run pass #2:      assembler -p2 <intermediate file> <symbol table> "
      "<output file>\n");
  /*log usage*/
  printf(
      "Append -log <file name> after any option to save log files to a text "
      "file.\n");
  /*exit*/
  exit(0);
}

int main(int argc, char **argv) {
  /*main entry*/
  int mode = 0;
  char *input, *inter, *output, *symtbl;
  int err;
  /*check args*/
  if (argc != 5 && argc != 7) {
    print_usage_and_exit();
  }
  /*pass 1*/
  if (strcmp(argv[1], "-p1") == 0) {
    mode = 1;
  } else if (strcmp(argv[1], "-p2") == 0) {
    /*pass 2*/
    mode = 2;
  }

  if (mode == 1) {
    /*mode 1*/
    input = argv[2];
    inter = argv[3];
    symtbl = argv[4];
    /*no output*/
    output = NULL;
  } else if (mode == 2) {
    /*mode 2*/
    input = NULL;
    /*no input*/
    inter = argv[2];
    symtbl = argv[3];
    output = argv[4];
  } else {
    /*others*/
    input = argv[1];
    inter = argv[2];
    /*set args*/
    symtbl = argv[3];
    output = argv[4];
  }

  if (argc == 7) {
    /*set log*/
    if (strcmp(argv[5], "-log") == 0) {
      set_log_file(argv[6]);
    } else {
      /*failed and print help*/
      print_usage_and_exit();
    }
  }
  /*try assemble*/
  err = assemble(input, output, inter, symtbl);

  if (err) {
    /*error*/
    write_to_log("One or more errors encountered during assembly operation.\n");
  } else {
    /*success*/
    write_to_log("Assembly operation completed successfully!\n");
  }
  /*set log*/
  if (is_log_file_set()) {
    printf("Results saved to %s\n", argv[5]);
  }

  return err;
}
