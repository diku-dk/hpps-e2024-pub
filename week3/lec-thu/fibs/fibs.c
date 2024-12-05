// fibs.c

#include <stdlib.h>

#include "fibs.h"

int* mkfibs(int n) {
  int* fibs = malloc(n * sizeof(int));

  fibs[0] = 1;
  fibs[1] = 1;

  for (int i = 2; i < n; i++) {
    fibs[i] = fibs[i-1] + fibs[i-2];
  }

  return fibs;
}
