#include <stdio.h>

void showbytes(char* p, int n) {
  for (int i = 0; i < n; i++) {
    unsigned char b = *(p+i);
    printf("%p: %x\n", p+i, (int)b);
  }
}

int main() {
  int x = -1;
  showbytes((char*)&x, sizeof(int));
  printf("\n");
  double y = 3.14;
  showbytes((char*)&y, sizeof(double));
}
