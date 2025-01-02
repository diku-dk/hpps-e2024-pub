#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/time.h>

double seconds(void) {
  struct timeval tv;
  gettimeofday(&tv, NULL); // The NULL is for timezone information.
  return tv.tv_sec + tv.tv_usec/1000000.0;
}

int* read_ints(int *n) {
  int capacity = 10;
  int used = 0;

  int *arr = malloc(capacity * sizeof(int));

  while (scanf("%d", &arr[used]) == 1) {
    used++;
    if (used == capacity) {
      capacity *= 2;
      arr = realloc(arr, capacity * sizeof(int));
    }
  }

  *n = used;
  return arr;
}

void write_ints(int n, int *arr) {
  for (int i = 0; i < n; i++) {
    printf("%d\n", arr[i]);
  }
}
