#include <stdio.h>

void print_number(int x) {
  char digits[30];

  int i = 0;
  int w = 10;

  while (x != 0) {
    int digit = x % w;
    int remainder = x / w;
    x = remainder;
    int code = digit + '0';
    digits[29-i] = code;
    i = i + 1;
  }
  fwrite(&digits[29-i], 1, i+1, stdout);
  printf("\n");
}

int main() {
  print_number(123);
}
