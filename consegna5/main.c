// Quando ho scritto questo codice solo io e Dio sapevamo cosa fa. Ora lo sa solo Dio.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gestionefile.h"
#include "operazioni.h"

void a_instruction(char[], FILE *);
void c_instruction(char[], FILE *);

int main(int argc, char **argv)
{
  // Apro il file input in read
  FILE *assem;
  assem = fopen(argv[1], "r");
  int len = strlen(argv[1]);
  // E creo il file destinazione
  char hack[len + 1];
  rename_file(argv[1], hack);
  FILE *output;
  output = fopen(hack, "w");

  char riga[128];
  while(fgets(riga, 127, assem)) {    // Itero per ogni riga del file asm
    del_tab(riga);
    if (riga[0] != '/' && riga[0] != '\r' && riga[0] != '\n') {
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
  to_bin(num, bin);         // Si trova in operazioni .c
  write_a(bin, output);     // Si trova in gestionefile.c
}

void c_instruction(char riga[], FILE *output) {
  char codifica[17] = "1110000000000000";

  // Questa funzione imposta i bit d1, d2, d3 (si trova in operazioni.c)
  dest_bits(riga, codifica);
  // Questa funzione imposta i bit j1, j2, j3 (si trova in operacioni.c)
  jump_bits(riga, codifica);
  
  // Porto il mio index alla posizione dell'=
  int i = 0;
  while(riga[i] != '=') i++; 
  // Questa funzione legge cosa c'è dopo l'uguale
  // E calcola i bit a, c1, c2, c3, c4, c5, c6 (si trova in operazioni.c)
  comp_bits(riga, codifica, i);

  // Scrivo la codifica nel file di output (si trova in gestionefile.c)
  write_c(codifica, output);
}