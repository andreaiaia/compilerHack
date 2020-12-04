#include "gestionefile.h"

void rename_file(char in_name[], char out_name[])
{
  int len = strlen(in_name);
  strcpy(out_name, in_name);
  int i = len - 3;
  char est[5] = "hack";
  while (i < len + 1)
  {
    out_name[i] = est[i - len + 3];
    i++;
  }
  out_name[i] = '\0';
}

void write_a(int riga[], FILE *output)
{
  fprintf(output, "%d", 0);
  for (int i = 0; i < 15; i++)
  {
    fprintf(output, "%d", riga[i]);
  }
  fprintf(output, "\n");
}

void write_c(char riga[], FILE *output) {
  fprintf(output, "%s", "111");

  
  fprintf(output, "\n");
}