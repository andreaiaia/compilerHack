#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symbol
{
  char label[128];
  int address;
  struct symbol *next;
};
typedef struct symbol table;
typedef table *pTable;

int a_to_i(char[]);
void to_bin(int, int[]);
void clear(char[]);
void dest_bits(char[], char[]);
void jump_bits(char[], char[], int);
void comp_bits(char[], char[], int);
void init_table(pTable);
int smistatore(char[], pTable, int*);