#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gestionefile.h"

int main(int argc, char *argv[]) {
  // Apro il file input in read
  FILE *assem;
  assem = fopen(argv[1], "r");
  int len = strlen(argv[1]);
  char hack[len + 1];
  rename_file(argv[1], hack);

  FILE *output;
  output = fopen(hack, "w");


  fclose(assem);
  fclose(output);

  return 0;
}