# More Concurrency and Parallelism

The following exercises are intended to further improve your skills
with parallel programming in C.

## Reading

Nothing new.

## Exercise: Creating a baseline

In this exercise you will develop a sequential program that partitions
an array of integers according to some predicate. Specifically, given
an integer *k* indicating the number of partitions, the array will be
reordered such that all elements that modulo *k* are equal to 0 come
first, then those that modulo *k* are equal to 1, and so on. Because
we are going to discard the sizes of the partitions, this can also be
considered a sorting problem, but to aid the eventual parallelisation,
that is not how we will handle it.

Our goal is to develop a "known good" sequential implementation,
including facilities for timing and reading and writing input, which
we can then reuse in increasingly elaborate parallel implementations.

To begin with, create a file `util.h` with [these
contents](solutions/util.h) and a `Makefile` with [these
contents](solutions/Makefile). These two files have reached their
final form and will not need further modification.

Now create a file `util.c` with these contents:

```C
#include "util.h"
```

And a file `partition-seq.c` with these contents:

```C
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void) {
}
```

You can run `make partition-seq` to compile this program. Make sure it
works before proceeding. The rest of this exercise is about modifying
`util.c` and `partition-seq.c`.

### I/O

In this subtask you must implement the functions `read_ints()` and
`write_ints()`. They go in `util.c`. First you should modify the
`main()` function in `partition-seq.c` as follows:

```C
int main(void) {
  int n;
  int* arr = read_ints(&n);

  write_ints(n, arr);
  free(arr);
}
```

Once you have implemented the I/O functions, `partitions-seq` will
read integers on standard input and produce them on standard output.
Example:

```
$ echo 1 2 3 | ./partition-seq
1
2
3
```

If we put some numbers in a file `input` we can also write the
partitioned numbers to an array `output` as follows:

```
$ ./partition-seq < input > output
```

#### Implementing `read_ints()`

The `read_ints()` function reads whitespace-separated ASCII text
integers from standard input (e.g., the console).

You may implement `read_ints()` by repeatedly calling `scanf("%d",
&x)` to read an integer into some variable `x`, which you then copy to
the array you are assembling. You should do this in a loop for as long
as the `scanf()` call returns 1 (indicating that it successfully read
an integer).

The most tricky thing about `read_ints()` is the fact that we do not
know in advance how many integers will be provided. The simplest
solution (which you should use so you can move on) is to allocate an
array of some larged fixed size (say, 1000 integers) and keep track of
how much of it you actually use. A good solution involves using
`realloc()` to grow an initial (small) allocation as necessary.

<details>
<summary>Open this to see the solution</summary>

```C
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
```

</details>

#### Implementing `write_ints()`

This one is easy - just print each integer followed by a newline.

<details>
<summary>Open this to see the solution</summary>

```C
void write_ints(int n, int *arr) {
  for (int i = 0; i < n; i++) {
    printf("%d\n", arr[i]);
  }
}
```

</details>

### Sequential partitioning

In this task you will work exclusively in `partition-seq.c`. Start by
defining a dummy `partition` function that does nothing:

```C
void partition(int n, int* arr, int k) {
}
```

* Insert a call `partition(n, arr, 10);` in between the calls to
  `read_ints()` and `write_ints()`. Ensure that the program still
  works as expected.

* Implement `partition()`. It must reorder the elements of `arr` based
  on their remainder when divided by `k` as discussed above.

The easiest way to implement `partition()` is to create `k` arrays
each of size `n`, tracking how many elements have been written to
each, then move through `arr` and copy elements to the appropriate
array. Finally, the elements of these `k` arrays are copied in order
to `arr`.

[Solution here.](solutions/partition-seq.c)

### Timing

We can use command line tools like `time` and `hyperfine` to measure
the performance of our programs. However, for something like
partitioning, reading and writing the input and output from files is
likely more expensive than actually performing the computation. This
makes it difficult to measure the impact of optimisations. In real
applications, the input would likely already be in memory. To simulate
this usage, we add timing instrumentation to `partition-seq.c` itself.

* Modify the program to call `seconds()` before and after
  `partition()` and printing (to stderr) the runtime in seconds.

<details>
<summary>Open this to see the solution</summary>

```C
  double bef = seconds();
  partition(n, arr, 10);
  double aft = seconds();
  fprintf(stderr, "Runtime: %f\n", aft-bef);
```
</details>

To obtain more robust measurements, it is common to run the function
of interest several times, and printing the average runtime.

* Modify `partition-seq.c` to run `partition()` several times (say,
  10) and report the average runtime. Note that since `partition()`
  modifies its input, you will have to store the original input
  somewhere so you can reconstruct it before each application of
  `partition()`. The time taken for this copying should not be
  measured.

<details>
<summary>Open this to see the solution</summary>

```C
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
```

</details>

## Exercise: Parallel partitioning

Now we will implement a (partially) partitioning in a file
`partition-par.c`. The basic approach will be the usual one:

1. Split the input into *chunks*, one per thread. In this case, the
   chunks are subsequence of the original input array.

2. Have each thread sequentially process its chunk and produce a
   partial result. Each partial result will be a partitioning of a
   subsequence of the original array (plus some auxiliary information,
   as we will see).

3. Combine the partial results.

It turns out that step 3 is the most difficult one. We will start with
step 2.

### Sequential partitioning with auxiliary information

In order to be able to easily combine several partitioned arrays, we
need more than just the elements they contain - we would also like
information about where each of the *k* parts of the array begins.
Further, it will turn out to be useful to have a sequential variant of
partitioning that is *not* in-place. In this task you must implement a
function with the following prototype:

```C
void partition_into(int* out, int* offsets, int n, int* arr, int k);
```

The `arr` array must not be modified; instead the result must be
written to `out`. Further, `offsets` must be a pointer to a
*k*-element array, which you must update such that `offsets[i]` is the
starting index in `out` of those elements `x` where `x%k==i`, i.e.,
the start offset of the `i`th part. Note that `offsets[0]==0`, and
that `offsets[i]==offsets[i+1]` when part `i` is empty.

You can largely adapt the sequential `partition` function to implement
`partition_into()`. The computation of `offsets` can be done by
storing the intermediate values of `o`.

<details>
<summary>Open this to see the solution</summary>

```C
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
```

</details>

### Splitting the work

Now implement a `partition()` function that splits the work into `P`
chunks, where `P` is the number of threads, and stores the per-thread
results in arrays of size `P`. Don't worry about combining the results
yet - we'll deal with that in the next subtask. For checking
correctness, just print the per-thread results.

**Hint:** construct these arrays with per-thread results:

```C
  int P = omp_get_max_threads();
  int* thread_parts[P]; // Partial results for each thread.
  int thread_parts_offsets[P][k]; // Offsets inside each partial result.
  int thread_parts_size[P]; // Size of each partial result.
```

Then when you are done, you can print the per-thread results like so:

```C
  for (int t = 0; t < P; t++) {
    printf("thread %d\n", P);
    write_ints(thread_parts_size[t], thread_parts[t]);
  }
```

<details>
<summary>Open this to see the solution</summary>

```C
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

  for (int t = 0; t < P; t++) {
    printf("thread %d\n", P);
    write_ints(thread_parts_size[t], thread_parts[t]);
  }

  for (int t = 0; t < P; t++) {
    free(thread_parts[t]);
  }
}
```

</details>

### Combining the partial results sequentially

Now implement logic the that copies the partial results into `arr`.
You should do this by first copying all the `0` parts, then the `1`
parts, etc. Do not worry about parallelising this yet.

**Hints:**

* Use an outer loop `for (int l = 0; l < k; l++)` that moves through
  the parts.
* The end offset (exclusive) for part `i` of thread `t` is
  `thread_parts_offsets[i+1]`, *except* when `i==k-1`, where the end
  offset is `thread_parts_size[t]`.

<details>
<summary>Open this to see the solution</summary>

```C
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

  int o = 0;
  for (int l = 0; l < k; l++) {
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
```

</details>

### Combining the partial results in parallel

In this task you will be parallelising the combination of the partial
results. This will be done in two parts.

First, you must compute an array `int offsets[k]` where `offsets[l]`
stores the offset of part `l` in the *final* result array. This can be
done with two *sequential* nested loops that iterates over all parts,
and for each part, over all threads, tallying the size of each part.
The total number of iterations is `k*P`, which is likely much smaller
than `n`, so it is hopefully not a big performance impediment that
this loop is sequential. It is a common technique to have short
sequential loops that build supporting information that allows
parallelisation of larger loops.

<details>
<summary>Open this to see a solution</summary>

```C
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
```

</details>

Based on the computed `offsets`, construct a parallel loop that copies
each part into place. Do this with two nested loops: an outer one that
iterates across all parts, and an inner one that iterates across all
threads, copying the relevant part into place. The outermost loop can
be parallel.

<details>
<summary>Open this to see the solution</summary>

```C
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

```

</details>

## Exercise: Benchmarking

Construct a reasonably-sized file of numbers and compare the
performance of `partition-seq`, `partition-par` with one thread, and
`partition-par` with all threads. Make sure to also check that
parallel execution produces the same result as sequential execution.

<details>
<summary>Open this to see a solution</summary>

```
$ seq 1000000 > many_numbers
$ ./partition-seq < many_numbers > many_numbers_seq
Runtime (mean of 10): 0.012177
$ OMP_NUM_THREADS=1 ./partition-par < many_numbers > many_numbers_par
Runtime (mean of 10): 0.013269
$ cmp many_numbers_seq many_numbers_par
$ ./partition-par < many_numbers > many_numbers_par
Runtime (mean of 10): 0.005652
$ cmp many_numbers_seq many_numbers_par
```

</details>
