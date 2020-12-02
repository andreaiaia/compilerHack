#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Apro il file input in read
  FILE *assem;
  assem = fopen(argv[1], 'r');

  


  fclose(assem);

  return 0;
}