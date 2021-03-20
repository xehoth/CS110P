/* This project is based on the MIPS Assembler of CS61C in UC Berkeley.
   The framework of this project is been modified to be suitable for RISC-V
   in CS110 course in ShanghaiTech University by Zhijie Yang in March 2019.
   Updated by Chibin Zhang and Zhe Ye in March 2021.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "tables.h"

const int SYMBOLTBL_NON_UNIQUE = 0;
const int SYMBOLTBL_UNIQUE_NAME = 1;

/*******************************
 * Helper Functions
 *******************************/

/*alloc failed*/
void allocation_failed() {
  write_to_log("Error: allocation failed\n");
  exit(1);
}

/*addr % 4 != 0*/
void addr_alignment_incorrect() {
  write_to_log("Error: address is not a multiple of 4.\n");
}

/*log name already exits*/
void name_already_exists(const char *name) {
  write_to_log("Error: name '%s' already exists in table.\n", name);
}

/*write sym to file*/
void write_sym(FILE *output, uint32_t addr, const char *name) {
  fprintf(output, "%u\t%s\n", addr, name);
}

/*read sym from file*/
int read_sym(FILE *input, uint32_t *addr, char *name) {
  return fscanf(input, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time.
   If memory allocation fails, you should call allocation_failed().
   Mode will be either SYMBOLTBL_NON_UNIQUE or SYMBOLTBL_UNIQUE_NAME. You will
   need to store this value for use during add_to_table().
 */
SymbolTable *create_table(int mode) {
  /* YOUR CODE HERE */
  SymbolTable *table = NULL;
  /*try malloc -> failed*/
  if (!(table = (SymbolTable *)malloc(sizeof(SymbolTable)))) {
    allocation_failed();
    /*prevent running after*/
    return NULL;
  }
  /*init table*/
  table->cap = INCREMENT_OF_CAP;
  table->len = 0;
  /*failed ot malloc*/
  if (!(table->table = (Symbol *)malloc(sizeof(Symbol) * table->cap))) {
    allocation_failed();
    /*prevent memory leak*/
    free(table);
    return NULL;
  }
  /*set mode*/
  table->mode = mode;
  return table;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable *table) { /* YOUR CODE HERE */
  uint32_t i = 0;
  if (!table) return; /*check if table is null*/
  /*free each elements*/
  for (; i < table->len; ++i) free(table->table[i].name);
  /*free the vector data*/
  free(table->table);
  /*!!!this might need to delete to free outside: also free table*/
  free(table);
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE.
   1. ADDR is given as the byte offset from the first instruction.
   2. The SymbolTable must be able to resize itself as more elements are added.

   3. Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.

   4. If ADDR is not word-aligned, you should call addr_alignment_incorrect()
   and return -1.

   5. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists
   in the table, you should call name_already_exists() and return -1.

   6.If memory allocation fails, you should call allocation_failed().

   Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable *table, const char *name, uint32_t addr) {
  /* YOUR CODE HERE */
  uint32_t i = 0;
  /*cur pointer to data to be added*/
  Symbol *cur = NULL;
  if (!table) return -1; /*check if table is null*/
  if (addr % 4) {
    /*check if ADDR is not word-aligned*/
    addr_alignment_incorrect();
    return -1;
  }
  if (table->mode == SYMBOLTBL_UNIQUE_NAME) {
    /*check unique name*/
    for (i = 0; i < table->len; ++i) {
      /*if equals*/
      if (strcmp(table->table[i].name, name) == 0) {
        name_already_exists(name);
        return -1;
      }
    }
  }
  /*cur points to data to be added*/
  cur = &table->table[table->len];
  if (!(cur->name = (char *)malloc(strlen(name) + 1))) {
    /*failed to malloc*/
    allocation_failed();
    return -1;
  }
  /*copy the name*/
  strcpy(cur->name, name);
  /*copy addr*/
  cur->addr = addr;
  if (++table->len >= table->cap) {
    /*table is full*/
    if (!(table->table = (Symbol *)realloc(
              table->table,
              sizeof(Symbol) * (table->cap += INCREMENT_OF_CAP)))) {
      /*failed to realloc*/
      allocation_failed();
      return -1;
    }
  }
  return 0;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable *table, const char *name) {
  /* YOUR CODE HERE */
  uint32_t i = 0;
  if (!table || !name) return -1; /*check if table is null*/
  /*search through table*/
  for (i = 0; i < table->len; ++i)
    if (strcmp(table->table[i].name, name) == 0) return table->table[i].addr;
  return -1;
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_sym() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable *table, FILE *output) { /* YOUR CODE HERE */
  uint32_t i = 0;
  if (!table || !output) return; /*check if null pointer*/
  /*loop through to write*/
  for (i = 0; i < table->len; ++i)
    write_sym(output, table->table[i].addr, table->table[i].name);
}

/* Reads the symbol table file written by the `write_table` function, and
   restore the symbol table.
 */
SymbolTable *create_table_from_file(int mode, FILE *file) {
  /* YOUR CODE HERE */
  SymbolTable *ret = NULL;
  /*buffer for scanf str*/
  char name_buf[1024];
  uint32_t addr;
  /*check invalid*/
  if (!file || (mode != SYMBOLTBL_NON_UNIQUE && mode != SYMBOLTBL_UNIQUE_NAME))
    return NULL;
  if (!(ret = create_table(mode))) return NULL; /*failed to create table*/
  while (read_sym(file, &addr, name_buf) != EOF) {
    /*read sym until EOF*/
    if (add_to_table(ret, name_buf, addr) == -1) {
      /*prevent memory leak*/
      free_table(ret);
      return NULL;
    }
  }
  /*success*/
  return ret;
}