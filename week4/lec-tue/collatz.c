#include <stdio.h>
#include <stdlib.h>

int collatz(int n) {
  int i = 0;
  while (n != 1) {
    if (n % 2 == 0) {
      n = n / 2;
    } else {
      n = n * 3 + 1;
    }
    i = i + 1;
  }
  return i;
}

int main(int argc, char* argv[]) {
  int k = atoi(argv[1]);
  int sum = 0;
  for (int i = 1; i < k; i++) {
    sum += collatz(i);
  }
  printf("%d\n", sum);
}
