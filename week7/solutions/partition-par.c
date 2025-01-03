#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

void partition_into(int* out, int* offsets, int n, int* arr, int k) {
  int* parts[k];
  int sizes[k];

  for (int l = 0; l < k; l++) {
    parts[l] = malloc(n * sizeof(int));
    sizes[l] = 0;
  }

  for (int i = 0; i < n; i++) {
    int x = arr[i];
    int l = x % k;
    parts[l][sizes[l]] = x;
    sizes[l]++;
  }

  int o = 0;
  for (int l = 0; l < k; l++) {
    offsets[l] = o;
    memcpy(&out[o], parts[l], sizes[l] * sizeof(int));
    o += sizes[l];
    free(parts[l]);
  }
}

void partition(int n, int* arr, int k) {
  int P = omp_get_max_threads();
  int* thread_parts[P];
  int thread_parts_offsets[P][k];
  int thread_parts_size[P];

  #pragma omp parallel
  {
    int t = omp_get_thread_num();
    int start = t * n/P;
    int end = (t+1) * n/P;
    if (t == omp_get_num_threads()-1) { end = n; }
    int chunk_size = end - start;

    thread_parts[t] = malloc(chunk_size * sizeof(int));
    thread_parts_size[t] = chunk_size;

    partition_into(thread_parts[t], thread_parts_offsets[t],
                   chunk_size,
                   &arr[start],
                   k);
  }

  int offsets[k];
  offsets[0] = 0;
  for (int l = 1; l < k; l++) {
    offsets[l] = offsets[l-1];
    for (int t = 0; t < P; t++) {
      int begin = thread_parts_offsets[t][l];
      int end = l == k-1
        ? thread_parts_size[t]
        : thread_parts_offsets[t][l+1];
      offsets[l] += (end-begin);
    }
  }

  #pragma omp parallel for
  for (int l = 0; l < k; l++) {
    int o = offsets[l];
    for (int t = 0; t < P; t++) {
      int begin = thread_parts_offsets[t][l];
      int end = l == k-1
        ? thread_parts_size[t]
        : thread_parts_offsets[t][l+1];
      if (end != begin) {
        memcpy(&arr[o], &thread_parts[t][begin], (end-begin)*sizeof(int));
      }
      o += (end-begin);
    }
  }

  for (int t = 0; t < P; t++) {
    free(thread_parts[t]);
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
