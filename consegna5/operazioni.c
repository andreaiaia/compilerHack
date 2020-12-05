#include "operazioni.h"

// OPERAZIONI GENERICHE
void to_bin(int i, int out[])
{
  for (int k = 14; k > 0; k--)
  {
    out[k] = i % 2;
    i /= 2;
  }
}

void del_tab(char riga[])
{
  int i = 0;
  while (riga[i] == ' ' || riga[i] == '\t')
    i++;
  int j = 0;
  char pulita[128];
  while (riga[i] != '\0')
  {
    pulita[j] = riga[i];
    j++, i++;
  }
  strcpy(riga, pulita);
}

// OPERAZIONI SULLE C INSTRUCTIONS
void dest_bits(char riga[], char codifica[])
{
  switch (riga[0])
  {
  case 'M':
    codifica[12] = '1';
    if (riga[1] == 'D')
      codifica[11] = '1';
    break;

  case 'D':
    codifica[11] = '1';
    break;

  case 'A':
    codifica[10] = '1';
    if (riga[1] == 'M')
    {
      codifica[12] = '1';
      if (riga[2] == 'D')
        codifica[11] = '1';
    }
    else if (riga[1] == 'D')
      codifica[11] = '1';

  default:
    break;
  }
}

void jump_bits(char riga[], char codifica[]) {
  int i = 0;
  i = 1;
}