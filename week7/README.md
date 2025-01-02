![header](header.png)

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
first, then those that modulo *k* are equal to 1, and so on. This can
also be considered a sorting problem, but to aid the eventual
parallelisation, that is not how we will handle it.

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

Then insert a call `partition(n, arr, 10);` in between the calls to
`read_ints()` and `write_ints()`. Ensure that the program still works
as expected.

Now we will actually implement `partition()`. It must reorder the
elements of `arr` based on their remainder when divided by `k` as
discussed above.

The easiest way to implement `partition()` is to create `k` arrays
each of size `n`, tracking how many elements have been written to
each, then move through `arr` and copy elements to the appropriate
array. Finally, the elements of these `k` arrays are copied in order
to `arr`.

[Solution here.](solutions/partition-seq.c)

### Timing and validation
