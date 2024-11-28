#include <stdio.h>

// if x has type T
// then &x has type T*

// if x has T*
// then *x has type T

int main() {
  int x = 123;
  printf("x = %d\n", x);
  int* px = &x;
  printf("px = %p\n", px);
  int** ppx = &px;
  printf("&px = %p\n", ppx);

  printf("*px = %d\n", *px);

  *px = 456;
  printf("x = %d\n", x);
  printf("px = %p\n", px);
}
