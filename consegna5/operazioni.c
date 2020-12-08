#include "operazioni.h"

// OPERAZIONI GENERICHE
// Funzione che legge una stringa e restituisce un intero
int a_to_i(char riga[]) {
  int num = 0;
  for(int i = 1; riga[i] >= '0' && riga[i] <= '9'; i++)
    num = num*10 + riga[i] - '0';

  return num;
}

void to_bin(int i, int out[]) {
  for (int k = 14; k >= 0; k--)    // Converte ogni decimale in binario con il modulo della divisione per 2
  {
    out[k] = i % 2;
    i /= 2;
  }
}

// Rimuove tutte le tabulazioni, i commenti e i tab dalla riga ca codificare
void clear(char riga[]) {
  int i = 0;
  int j = 0;
  char pulita[128];
  while((riga[i] != '\0') && (riga[i] != '/')) {  // Ciclo la riga finché non trovo un commento o la riga finisce
    if (riga[i] != ' ' && riga[i] != '\t' && riga[i] != '\r' && riga[i] != '\n') {      // Se il carattere "puntato" non è uno spazio o un tab
      pulita[j] = riga[i];                        // lo copio nella riga ripulita
      j++;
    }
    i++;
  }
  pulita[j] = '\0';            // Aggiungo il terminatore
  strcpy(riga, pulita);        // Copio il risultato nella riga che poi codificherò
}

// OPERAZINI SULLE A-INSTRUCTIONS

// Discerno se la @ è seguita da un numero, un'etichetta o una variabile
int smistatore(char riga[], pTable head, int *i) {
  int num;
  pTable p = head;
  if ((riga[1] >= 'A' && riga[1] <= 'Z') || (riga[1] >= 'a' && riga[1] <= 'z'))
  {
    char nome_label[128];
    int j;
    for (j = 1; riga[j] != '\0'; j++) nome_label[j - 1] = riga[j];
    nome_label[j-1] = '\0';
    while ((strcmp(p->label, nome_label) != 0) && (p->next)) {
      p = p->next;
    }
    if (strcmp(p->label, nome_label))  // se le due stringhe sono diverse
    {
      p->next = malloc(sizeof(table));
      p = p->next;
      p->address = *i;
      strcpy(p->label, nome_label);
      p->next = NULL;
      *i += 1;
    }
    num = p->address;
  }
  else
  {
    num = a_to_i(riga); // from operazioni.c
  }

  return num;
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

// Codifico i bit j1, j2, j3
void jump_bits(char riga[], char codifica[], int i)
{
  switch (riga[i + 2])
  {
  case 'G':
    codifica[15] = '1';
    if (riga[i + 3] == 'E')
      codifica[14] = '1';
    break;

  case 'E':
    codifica[14] = '1';
    break;

  case 'L':
    codifica[13] = '1';
    if (riga[i + 3] == 'E')
      codifica[14] = '1';
    break;

  case 'N':
    codifica[13] = '1';
    codifica[15] = '1';
    break;

  case 'M':
    codifica[13] = '1';
    codifica[14] = '1';
    codifica[15] = '1';

  default:
    break;
  }
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

// OPERAZIONI SULLA SYMBOL TABLE
void init_table(pTable head)
{
  pTable slide = head;
  // RAM0
  strcpy(slide->label, "R0");
  slide->address = 0;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM1
  strcpy(slide->label, "R1");
  slide->address = 1;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM2
  strcpy(slide->label, "R2");
  slide->address = 2;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM3
  strcpy(slide->label, "R3");
  slide->address = 3;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM4
  strcpy(slide->label, "R4");
  slide->address = 4;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM5
  strcpy(slide->label, "R5");
  slide->address = 5;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM6
  strcpy(slide->label, "R6");
  slide->address = 6;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM7
  strcpy(slide->label, "R7");
  slide->address = 7;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM8
  strcpy(slide->label, "R8");
  slide->address = 8;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM9
  strcpy(slide->label, "R9");
  slide->address = 9;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM10
  strcpy(slide->label, "R10");
  slide->address = 10;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM11
  strcpy(slide->label, "R11");
  slide->address = 11;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM12
  strcpy(slide->label, "R12");
  slide->address = 12;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM13
  strcpy(slide->label, "R13");
  slide->address = 13;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM14
  strcpy(slide->label, "R14");
  slide->address = 14;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // RAM15
  strcpy(slide->label, "R15");
  slide->address = 15;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // SCREEN
  strcpy(slide->label, "SCREEN");
  slide->address = 16384;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // KBD
  strcpy(slide->label, "KBD");
  slide->address = 24576;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // SP
  strcpy(slide->label, "SP");
  slide->address = 0;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // LCL
  strcpy(slide->label, "LCL");
  slide->address = 1;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // ARG
  strcpy(slide->label, "ARG");
  slide->address = 2;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // THIS
  strcpy(slide->label, "THIS");
  slide->address = 3;
  slide->next = malloc(sizeof(table));
  slide = slide->next;
  // THAT
  strcpy(slide->label, "THAT");
  slide->address = 4;
  slide->next = NULL;
}