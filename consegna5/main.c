#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gestionefile.h"
#include "operazioni.h"

void a_instruction(char [], FILE *);
void c_instruction(char [], FILE *);

int main(int argc, char **argv) {
  // Apro il file input in read e creo il file destinazione
  FILE *assem;
  assem = fopen(argv[1], "r");
  int len = strlen(argv[1]);
  char hack[len + 1];
  rename_file(argv[1], hack);
  FILE *output;
  output = fopen(hack, "w");

  char riga[128];
  while(fgets(riga, 127, assem)) {
    del_tab(riga);
    if (riga[0] != '/') {
      if (riga[0] == '@') a_instruction(riga, output);
      else if (riga[0] != '(') c_instruction(riga, output);
    }
  }

  fclose(assem);
  fclose(output);

  return 0;
}

void a_instruction(char riga[], FILE *output) {
  int num = atoi(riga + 1);
  int bin[15] = {0};
  to_bin(num, bin);
  write_a(bin, output);
}

void c_instruction(char riga[], FILE *output) {
  char codifica[14] = "0000000000000";

  dest_bits(riga, codifica);

  switch (expression)
  {
  case /* constant-expression */:
    /* code */
    break;
  
  default:
    break;
  }

  write_c(codifica, output);
}