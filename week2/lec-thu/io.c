#include <stdio.h>
#include <assert.h>

int main() {
  FILE* f = fopen("output", "w");
  assert(f != NULL);

  int x = 31337;

  size_t ret =
    fwrite(&x,
         1, sizeof(int),
         f);
  assert(ret == sizeof(int));

  fclose(f);
}
