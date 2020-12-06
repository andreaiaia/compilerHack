#include "operazioni.h"

// OPERAZIONI GENERICHE
void to_bin(int i, int out[])
{
  for (int k = 14; k > 0; k--)    // Converte ogni decimale in binario con il modulo della divisione per 2
  {
    out[k] = i % 2;
    i /= 2;
  }
}

void clear(char riga[])     // Rimuove tutte le tabulazioni, i commenti e i tab dalla riga ca codificare
{
  int i = 0;
  int j = 0;
  char pulita[128];
  while((riga[i] != '\0') && (riga[i] != '/')) {  // Ciclo la riga finché non trovo un commento o la riga finisce
    if (riga[i] != ' ' || riga[i] != '\t') {      // Se il carattere "puntato" non è uno spazio o un tab
      pulita[j] = riga[i];                        // lo copio nella riga ripulita
      j++;
    }
    i++;
  }
  j++;
  pulita[j] = '\0';            // Aggiungo il terminatore

  strcpy(riga, pulita);       // Copio il risultato nella riga che poi codificherò
}

// OPERAZIONI SULLE C INSTRUCTIONS

// Codifico i bit d1, d2, d3
void dest_bits(char riga[], char codifica[])
{
  switch (riga[0])
  {
  case 'M':                 // Se il registro di destinazione è M
    codifica[12] = '1';     // il bit d3 è a 1
    if (riga[1] == 'D')     // in questo caso M può essere seguito solo da D o nulla
      codifica[11] = '1';   // se è seguito da D, il bit d2 è 1
    break;

  case 'D':                 // Se il registro di dest è D
    codifica[11] = '1';     // il bit d2 è 1
    break;                  // D non può essere seguito da nulla

  default:                  // Se non vale nessuno dei prec, il reg di dest è A
    codifica[10] = '1';     // il bit d1 è 1
    if (riga[1] == 'M')     // Se A è seguito da M
    {
      codifica[12] = '1';   // il bit d3 è 1
      if (riga[2] == 'D')   // M può essere seguito da D
        codifica[11] = '1'; // in tal caso d2 è 1
    }
    else if (riga[1] == 'D')// se A è seguito da D
      codifica[11] = '1';   // il dib d2 è 1, D non può essere seguito da alcun reg
    break;
  }
}

void jump_bits(char riga[], char codifica[]) {    // DA SCRIVERE
  int i = 0;
  i = 1;
}

// Codifico i bit a, c1, c2, c3, c4, c5, c6
void comp_bits(char riga[], char codifica[], int i)
{
  switch (riga[i + 1])    // Analizzo i casi possibili del primo char dopo l'=
  {
  case '0':               // primo char = 0
    codifica[4] = '1';
    codifica[6] = '1';
    codifica[8] = '1';
    break;

  case '+':               // primo char = +
    for (int j = 4; j < 10; j++)
      codifica[j] = '1';
    break;

  case '1':               // primo char = 1
    for (int j = 4; j < 10; j++)
      codifica[j] = '1';
    break;

  case 'D':               // primo char = D
    if (riga[i + 3] == '1')   // Nel caso di una somma/sott con 1
    {
      codifica[6] = '1';      // imposto i bit comuni
      codifica[7] = '1';
      codifica[8] = '1';
      if (riga[i + 2] == '+') // e poi nel caso di una somma,
      {                       // imposto i bit non in comune
        codifica[5] = '1';
        codifica[9] = '1';
      }
    }
    else if (riga[i + 3] == 'A' || riga[i + 3] == 'M') // Nel caso di un'operazione con A/M (*)
    {
      if (riga[i + 3] == 'M')         // Se M imposto il bit a
        codifica[3] = '1';
      if (riga[i + 2] == '+')         // Se è D+*
        codifica[8] = '1';
      else if (riga[i + 2] == '-')    // Se + D-*
      {
        codifica[5] = '1';
        codifica[8] = '1';
        codifica[9] = '1';
      }
      else if (riga[i + 2] == '|')    // Se è D|*
      {                               // NB se non è nessuno di questi, è il caso D&*
        codifica[5] = '1';            // Essendo tutti i bit del caso a 0, non devo fare nulla
        codifica[7] = '1';
        codifica[9] = '1';
      }
    }
    else                        // Infine, caso base, D e basta
    {
      codifica[6] = '1';
      codifica[7] = '1';
    }
    break;

  case '-':               // primo char = -
    codifica[8] = '1';
    if (riga[i + 2] == '1') // Se -1
    {
      codifica[4] = '1';
      codifica[5] = '1';
      codifica[6] = '1';
    }
    else if (riga[i + 2] == 'D') // Se -D
    {
      codifica[6] = '1';
      codifica[7] = '1';
      codifica[9] = '1';
    }
    else                        // Se -A/M
    {
      if (riga[i + 2] == 'M')   // Se M allora imposto a
        codifica[3] = '1';
      codifica[4] = '1';
      codifica[5] = '1';
      codifica[9] = '1';
    }
    break;

  case '!':               // primo char = !
    codifica[9] = '1';
    if (riga[i + 2] == 'D')     // Se !D
    {
      codifica[6] = '1';
      codifica[7] = '1';
    }
    else                        // Se !A/!M
    {
      if (riga[i + 2] == 'M')   // Se M imposto a
        codifica[3] = '1';
      codifica[4] = '1';
      codifica[5] = '1';
    }
    break;

  default:                  // primo char = A/M (*)
    if (riga[i + 1] == 'M') // Se M imposto a
      codifica[3] = '1';
    if (riga[i + 3] == '1') // Se sommo/sottraggo * a 1
    {
      codifica[4] = '1';    // imposto i bit comuni alle due operazioni
      codifica[5] = '1';
      codifica[8] = '1';
      if (riga[i + 2] == '+') // imposto i bit del caso somma
      {
        codifica[7] = '1';
        codifica[9] = '1';
      }
    }
    else if (riga[i + 3] == 'D') // Se faccio A-D
    {
      codifica[7] = '1';
      codifica[8] = '1';
      codifica[9] = '1';
    }
    else                  // Altrimenti, caso base, A (o M)
    {
      codifica[4] = '1';
      codifica[5] = '1';
    }
    break;
  }
}