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
    clear(riga);
    if (riga[0] != '/' && riga[0] != '\r' && riga[0] != '\n' && riga[0] != '\0') {
      if (riga[0] == '@') a_instruction(riga, output);
      else if (riga[0] != '(') c_instruction(riga, output);
    }
  }

  fclose(assem);
  fclose(output);
  return 0;
}

void a_instruction(char riga[], FILE *output) {
  int num = a_to_i(riga);     // from operazioni.c
  int bin[15] = {0};
  to_bin(num, bin);           // from operazioni .c
  write_a(bin, output);       // from gestionefile.c
}

void c_instruction(char riga[], FILE *output) {
  char codifica[17] = "1110000000000000";
  
  // Porto il mio index alla posizione dell'=
  int eq = 0;
  while((riga[eq] != '=') && (riga[eq] != '\0')) eq++;
  // Porto un altro index alla positione del ;
  int semicolon = 0;
  while((riga[semicolon] != ';') && (riga[semicolon] != '\0')) semicolon++;

  // Questa funzione imposta i bit d1, d2, d3 (from operazioni.c)
  if (riga[eq] != '\0')
    dest_bits(riga, codifica);

  // Se c'è un =, calcola il comp dopo l'uguale, 
  // altrimenti calcola il comp a partire da inizio riga
  // E calcola i bit a, c1, c2, c3, c4, c5, c6 (from operazioni.c)
  if(riga[eq] != '\0')
    comp_bits(riga, codifica, eq);        // from operazioni.c
  else comp_bits(riga, codifica, -1);     // from operazioni.c

  // Questa funzione imposta i bit j1, j2, j3 (from operacioni.c)
  jump_bits(riga, codifica, semicolon);

  // Scrivo la codifica nel file di output (from gestionefile.c)
  write_c(codifica, output);
}