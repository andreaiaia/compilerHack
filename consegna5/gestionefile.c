#include "gestionefile.h"

void rename_file(char in_name[], char out_name[])
{
  int len = strlen(in_name);
  strcpy(out_name, in_name);
  int i = len - 3;            // Posiziono il "cursore" al . prima dell'estensione
  char est[5] = "hack";
  while (i < len + 1)         // Sostituisco i caratteri dell'estensione con l'est 'hack'
  {
    out_name[i] = est[i - len + 3];
    i++;
  }
  out_name[i] = '\0';         // Chiudo la stringa
}

void write_a(int riga[], FILE *output)
{
  fprintf(output, "%d", 0);           // Imposto il primo bit a 0 per segnalare una A-instruction
  for (int i = 0; i < 15; i++)
  {
    fprintf(output, "%d", riga[i]);   // Copio il numero in binario precedentemente calcolato nel file
  }
  fprintf(output, "\n");              // Vado a capo
}

void write_c(char riga[], FILE *output) {
  for (int i = 0; i < 16; i++)
  {
    fprintf(output, "%c", riga[i]);   // Copio la stringa codificata nel file
  }
  fprintf(output, "\n");              // Vado a capo
}