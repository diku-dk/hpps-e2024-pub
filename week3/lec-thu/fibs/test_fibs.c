// test_fibs.c

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "fibs.h"

int main(int argc, char* argv[]) {
  assert(argc == 2);
  int n = atoi(argv[1]);

  {
    int* fibs = mkfibs(n);
    printf("%p\n", (void*)fibs);
    for (int i = 0; i < n; i++) {
      printf("fib(%d) = %d\n", i, fibs[i]);
    }
    free(fibs);
  }
}
