#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gestionefile.h"

void a_instruction(char [], FILE *);
void to_bin(int, int []);
void write(int [], FILE *);

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
    if (riga[0] != '/') {
      if (riga[0] == '@') a_instruction(riga, output);
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
  write(bin, output);
}
void to_bin(int i, int out[]) {
  for (int k = 14; k > 0; k--) {
    out[k] = i%2;
    i /= 2;
  }
}
void write(int riga[], FILE *output) {
  fprintf(output, "%d", 0);
  for(int i = 0; i < 15; i++) {
    fprintf(output, "%d", riga[i]);
  }
  fprintf(output, "\n");
}