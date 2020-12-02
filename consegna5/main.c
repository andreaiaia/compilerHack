#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Apro il file input in read
  FILE *assem;
  assem = fopen(argv[1], "r");
  int len = strlen(argv[1]);
  char outfile[len];
  strcpy(outfile, argv[1]);
  strcat(outfile, "hack");
  FILE *output;
  output = fopen(outfile, "w");


  fclose(assem);
  fclose(output);

  return 0;
}