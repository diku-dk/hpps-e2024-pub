#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void partition(int n, int* arr, int k) {
  int* parts[k];
  int parts_size[k];

  for (int l = 0; l < k; l++) {
    parts[l] = malloc(n * sizeof(int));
    parts_size[l] = 0;
  }

  for (int i = 0; i < n; i++) {
    int x = arr[i];
    int l = x % k;
    parts[l][parts_size[l]] = x;
    parts_size[l]++;
  }

  int o = 0;
  for (int l = 0; l < k; l++) {
    memcpy(&arr[o], parts[l], parts_size[l] * sizeof(int));
    o += parts_size[l];
    free(parts[l]);
  }
}

int main(void) {
  int n;
  int* arr = read_ints(&n);

  int runs = 10;

  int *orig = malloc(n * sizeof(int));
  memcpy(orig, arr, n * sizeof(int));

  double elapsed = 0;
  for (int i = 0; i < runs; i++) {
    memcpy(arr, orig, n * sizeof(int));
    double bef = seconds();
    partition(n, arr, 10);
    double aft = seconds();
    elapsed += aft - bef;
  }
  fprintf(stderr, "Runtime (mean of %d): %f\n", runs, elapsed/runs);

  write_ints(n, arr);

  free(arr);
  free(orig);
}
