// Quando ho scritto questo codice solo io e Dio sapevamo cosa fa. Ora lo sa solo Dio.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gestionefile.h"
#include "operazioni.h"

void prima_passata(char[], pTable, int);
void a_instruction(char[], pTable, int*, FILE *);
void c_instruction(char[], FILE *);

// Struct della symbol table definita in operazioni.h

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

  // Prima passata, creazione della symbol table
  pTable head = malloc(sizeof(table));
  init_table(head);                     // from operazioni.c

  int indirizzo = 0;
  while(fgets(riga, 127, assem)) {
    clear(riga);
    if (riga[0] != '/' && riga[0] != '\r' && riga[0] != '\n' && riga[0] != '\0') {
      prima_passata(riga, head, indirizzo);
      if (riga[0] != '(') indirizzo++;
    }
  }

  rewind(assem);

  // Seconda passata, traduzione delle istruzioni
  int i = 16;
  while(fgets(riga, 127, assem)) {    // Itero per ogni riga del file asm
    clear(riga);
    if (riga[0] != '/' && riga[0] != '\r' && riga[0] != '\n' && riga[0] != '\0') {
      if (riga[0] == '@') a_instruction(riga, head, &i, output);
      else if (riga[0] != '(') c_instruction(riga, output);
    }
  }

  fclose(assem);
  fclose(output);
  return 0;
}

void prima_passata(char riga[], pTable head, int indirizzo) {
  pTable p = head;
  while(p->next) p = p->next;
  if (riga[0] == '(') {
    p->next = malloc(sizeof(table));
    p = p->next;
    for(int i = 1; riga[i] != ')'; i++)
      p->label[i-1] = riga[i];
    p->address = indirizzo;
    p->next = NULL;
  }
}

void a_instruction(char riga[], pTable head, int *i, FILE *output) {
  int num = 0;
  // Discerno se la @ è seguita da un numero, un'etichetta o una variabile
  num = smistatore(riga, head, i);  // from operazioni.c
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
  //printf("%d\n", semicolon);
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
  if(riga[semicolon] != '\0') jump_bits(riga, codifica, semicolon);

  // Scrivo la codifica nel file di output (from gestionefile.c)
  write_c(codifica, output);
}