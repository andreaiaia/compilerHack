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
  to_bin(num, bin);
  write_a(bin, output);
}

void c_instruction(char riga[], FILE *output) {
  char codifica[17] = "1110000000000000";

  dest_bits(riga, codifica);
  jump_bits(riga, codifica);
  
  int i = 0;
  while(riga[i] != '=') i++; // Da grande sarò una funzione

  switch (riga[i+1])
  {
  case '0':
    codifica[3] = '1';
    codifica[4] = '1';
    codifica[6] = '1'; 
    codifica[8] = '1';
    break;

  case 'D':
    if (riga[i+3] == '1') {
      codifica[6] = '1';
      codifica[7] = '1';
      codifica[8] = '1';
      if (riga[i+2] == '+') {
        codifica[5] = '1';
        codifica[9] = '1';
      }
    }
    else if (riga[i + 3] == 'A' || riga[i + 3] == 'M')
    {
      if (riga[i + 3] == 'M') codifica[3] = '1';
      if (riga[i+2] == '+') codifica[8] = '1';
      else if (riga[i+2] == '-') {
        codifica[5] = '1';
        codifica[8] = '1';
        codifica[9] = '1';
      } else if (riga[i+2] == '|') {
        codifica[5] = '1';
        codifica[7] = '1';
        codifica[9] = '1';
      }
    }
    else {
      codifica[3] = '1';
      codifica[6] = '1';
      codifica[7] = '1';
    }
    break;
  
  case '-':
    // RICOMINCIARE DA QUI
    break;

  default:
    break;
  }

  write_c(codifica, output);
}