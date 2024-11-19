#include <stdio.h>

int main(int argc, char* argv[]) {
  for (int i = 0; i < argc; i++) {
    int x;
    if (sscanf(argv[i], "%d", &x) == 1) {
      printf("%d\n", x);
    } else {
      printf("not a number: %s\n", argv[i]);
    }
  }
}
