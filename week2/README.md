# Week 2 - floating-point numbers, pointers, and memory

## Suggested Reading

Tuesday:

* HPPS 3

Thursday:

* HPPS 4
* JG 8-9, 11

## Videos

* [Addresses](https://sid.erda.dk/share_redirect/f8RgVGzlET/addresses.mp4)
* [IO](https://sid.erda.dk/share_redirect/f8RgVGzlET/io-in-c.mp4)
* [Makefiles](https://sid.erda.dk/share_redirect/f8RgVGzlET/make.mp4)
* [Using a debugger](https://www.youtube.com/watch?v=bWH-nL7v5F4)

## Exercise: play with integers

Here it is! The sequel to the [Integer representation
game](http://topps.diku.dk/compsys/integers.html)!

Make a winning streak at the [integer arithmetic
game](http://topps.diku.dk/compsys/integer-arithmetic.html). Then push
your skills to the limit by winning the [floating point
game](http://topps.diku.dk/compsys/floating-point.html).

## Exercise: Developing a C library for reading and writing numbers

Now that we understand how integers and floating-point numbers are
represented, let us create a C library for reading and writing them to
files. Such a library will be the topic of A2. This set of exercises
is intended to prepare you for A2, by teaching you how to structure
real C libraries generally, and in particular how the library you will
develop in A2 will work.

In A1 we put all code in the header file (`bits.h`). Such [header-only
libraries](https://en.wikipedia.org/wiki/Header-only) have their
merits, but also serious downsides. In particular, they cannot be
accessed by other languages, such as Python.

Most real C libraries are structured as one or more header files and
one or more *implementation files* (`.c` files). The difference
between such implementation files and the C programs you have written
previously is that *they have no `main()` function*. Next week we will
see how this enables us to compile a library *once* and use it
*multiple times*. For now, we will simply pass multiple `.c` files to
the compiler.

The [`solution`](solution/) directory contains our solutions. It's
best to look at them only when you have tried yourself first, or are
stuck. When you do look at them, make sure you understand why they
work before moving on.

### Part 1: Setting up a working environment

* Create an empty file `numlib.h` containing the following *prototype*:

  ```C
  int get_number(void);
  ```

  In C, a prototype is a declaration that some function *exists*, but
  without giving it a definition. Think of it like an abstract
  interface.

* Create a file `numlib.c` that contains `#include "numlib.h"` and the
  following *definition*:

  ```C
  int get_number(void) {
    return 42;
  }
  ```

* Create a file `numlib_test.c`.

  This file must contain `#include "numlib.h"` and a `main()`
  function. The `main()` function should call `get_number()` and print
  the result.

* Create a `Makefile` with a target `numlib_test` that compiles
  `numlib.c` *and* `numlib_test.c` (by passing both to the C compiler)
  to produce a program called `numlib`. Look at [last week's
  exercises](../1-e-2) or A1 if you have forgotten what that looks
  like.

Now run `make numlib_test` and run `./numlib_test` to ensure
everything works. You have now created your first multi-file C
program.

#### Hints

* **Never** `#include` a `.c` file! If you ever do something like
  `#include "numlib.c"`, then you are making a mistake. Pass the `.c`
  files to the *compiler* on the command line, or in your `Makefile`.

* [Solution here](solution/part1).

### Part 2: A very crude number reader

The following function reads a single character from standard input
(the console, unless redirected). If that character corresponds to a
decimal digit, it returns an `int` of the corresponding numeric value.
Otherwise it returns -1.

```C
int get_digit(void) {
  int c = getchar();
  if (isdigit(c)) {
    return c - 48;
  } else {
    return -1;
  }
}
```

We will use this function to make `get_number()` return an integer
read from standard input. Now go through the following steps. Remember
to check after every step that everything still compiles.

* Copy `get_digit` into `numlib.c` but *not* `numlib.h`. You will need
  to add includes for the the `stdio.h` and `ctype.h` headers.

* Now modify `get_number` such that it contains a loop that repeatedly
  calls `get_digit()` and produces a number corresponding to the
  digits read. The loop should terminate (and `get_number()` return)
  when `get_digit()` returns -1.

* Modify `numlib_test.c` such that it calls `get_number()` twice (and
  prints both numbers, of course).

Now ponder:

* Can `numlib_test.c` directly use `get_digit()`? Why or why not? What
  would you modify if you wanted this to be possible?

* What happens when you run `echo "123 456" | ./numlib_test`?

* What happens when you run `echo "123  456" | ./numlib_test`?

#### Hints

* [Solution here](solution/part2).

## Exercise: a hex dumper

Before continuing work on the library, we will build a useful tool
that might come in handy. This will also give you more experience with
C, in particular with file I/O and simple use of pointers.

In the following, we colloquially use "print" as slang for writing to
standard output.

For this exercise you must Write a C program `hexabyte` that prints in
hexadecimal notation the bytes contained in a file. For example:

```
$ ./hexabyte ints.bin
01
00
00
00
01
00
00
00
02
00
00
00
03
...
```

You are given free reign to design and implement the program from
scratch.

### Hints

* Define the main function as

  ```C
  int main(int argc, char** argv) {
    ...
  }
  ```

  in order to access command-line parameters (in `argv`).

* Use `fopen(filename, "r")` to open a file for reading.

* Use the `fread()` function to actually perform the read from the
  `FILE*` object.  This function will return `0` when you have read
  the entire file.

* Use `%.2x` format specifier with `printf()` to print an integer as a
  two-digit hexadecimal number.

* Use the `assert` macro from `<assert.h>` to guard against potential
  errors.

* You *did* create a `Makefile`, right?

* [Solution here](solution/hexabyte)

## Exercise: Rebuilding the number library

Take the library you developed in the previous exercises, but delete
the code from `numlib.h` and `numlib.c` (and any test programs you may
have written) - we start from scratch.

Add the following includes to `numlib.h`:

```
#include <stdint.h>
#include <stdio.h>
```

Add the following includes to `numlib.c`:

```C
#include "numlib.h"
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
```

Also add the following function to `numlib.c` and a corresponding
prototype to `numlib.h`:

```C
void skipspaces(FILE *f) {
  while (1) {
    int c = fgetc(f);
    if (!isspace(c)) {
      if (c != EOF) {
        ungetc(c, f);
      }
      return;
    }
  }
}
```

What do you think it does?

<details>
<summary>Open this to see the answer</summary>

The `skipspaces()` function repeatedly reads characters from the given
file until it reads something that is *not* a whitespace character. It
then puts the character "back in the file" (so to speak) with
`ungetc()`, meaning that the next call to `fgetc()` will read it.
However, this is *only* done if the character is not the special `EOF`
marker.

Essentially, this function fast-forwards over any whitespace
characters, but is careful not to also consume the first
non-whitespace character encountered.

</details>

[Solution here.](solution/numlib)

### Implement `read_uint_ascii()`

Add the following prototype to `numlib.h`:

```C
// Read unsigned integer in base-10 ASCII format from the given file.
// Stores the resulting number in the location pointed to by 'out'.
// Stops at the first non-digit character, which is left unread. Returns nonzero
// on failure. Fails if no digits are read before a non-digit is encountered.
// Returns EOF if the first character read results in EOF.
int read_uint_ascii(FILE *f, uint32_t *out);
```

Now implement that function in `numlib.c`. Make sure to also write a
test program. Do *not* use `scanf()` or any other function in the
`scanf()` family.

#### Hints

* The logic is very similar to the function developed in the morning
  exercise.

* The loop has two stopping conditions: if the next character exists
  but is not a digit (e.g. a letter or whispace), or if there are no
  more characters to read (so that `fgetc()` returns EOF).

* When you find a non-digit character, use `ungetc()` to put it back.

* Leave the EOF handling for last - it is necessary so we can
  distinguish between a file containing an invalid non-digit, or if
  there merely nothing more to read from a file. We usually want to
  deal with these cases differently (one may be invalid input, the
  other not).

* You can test with e.g. `echo 123 | ./your_test_program`.

* You can also create a test file and *redirect* the input to your
  program from that file, e.g., `./your_test_program <
  your_test_file`.

* Make sure to also test what happens when the input is invalid (e.g.
  `abc`).

* Write a test program that calls `read_uint_ascii()` as many times as
  necessary to read all numbers in a file (e.g. `123 456 789`). You
  will need to call `skipspaces()` after each call to
  `read_uint_ascii()`. Stop as soon as `read_uint_ascii()` fails.
  Treat handling due to EOF differently than handling due to invalid
  input.

### Implement `write_uint_ascii()`

Add the following prototype to `numlib.h`:

```C
// Write the provided integer in base-10 ASCII format to the given
// file. Returns 0 on success.
int write_uint_ascii(FILE *f, uint32_t x);
```

Now implement that function in `numlib.c`. You are allowed to use
`fprintf()` if you wish, but you can also write the digits manually -
the lecture slides show how.

#### Hints

* When you have implemented both `read_uint_ascii` and
  `write_uint_ascii`, you can write a single test program that uses
  both these functions to check whether they work well together.

### Implement `write_uint_le()`

Add the following prototype to `numlib.h`:

```C
// Write the provided 32-bit integer in *little endian byte order*
// to the given file.  Returns 0 on success.
int write_uint_le(FILE *f, uint32_t x);
```

Now implement that function in `numlib.c`.

#### Hints

* When writing a test program for this program, make sure to redirect
  the output to a file (`./test > output`), as your terminal will now
  show the raw bytes correctly. Use `hexabytes` (or some similar
  program) to inspect the bytes of the file produced.

* To extract the most significant byte from a variable `x` of type
  `uint32_t`, use `x>>24`.

### Implement a program that converts from ASCII to binary representation

Create a file `uint_ascii_to_uint_le.c`. Add a Makefile rule that
compiles this to a program `uint_ascii_to_uint_le`.

This program must run a loop that reads all ASCII numbers from
standard input (using `read_uint_ascii()` and `skipspaces()` like the
test program above) and writes them in little endian binary format on
standard output (using `write_uint_ascii()`.

### Implement `read_uint_le()`

Add the following prototype to `numlib.h`:

```C
// Read little-endian 32-bit integer from given file.
// Returns 0 on success.
int read_uint_le(FILE *f, uint32_t *out);
```

Now implement that function in `numlib.c`.

#### Hints

* Use `fgetc(f)` to read a single byte from a file. Make sure to store
  the result in a variable of type `int`, and check whether the return
  value is `EOF`.

* If you have a byte `b` stored in a variable of type `int` or
  `uint32_t`, then `x<<8` shifts that byte 8 bit positions. I.e.,
  `0xFF << 8 == 0xFF00`.

* If you have four bytes that you want to combine into an integer, you
  can do that with a combination of shifting and bitwise disjunction:
  `b0 | (b1<<8) | (b2<<16) | (b3<<24)`.

### Implement a program that converts from binary to ASCII representation

Create a file `uint_le_to_uint_ascii.c`. Add a Makefile rule that
compiles this to a program `uint_le_to_uint_ascii`.

This program must run a loop that reads all ASCII numbers from
standard input (using `read_uint_le()`) like the test program above)
and writes them in ASCII format, separated by spaces or newlines
characters, on standard output (using `write_uint_ascii()`).

#### Hints

* The programs `uint_ascii_to_uint_le` are inverses of each other
  `uint_le_to_uint_ascii`. A command such as

        echo 123 456 789 | ./uint_ascii_to_uint_le | ./uint_le_to_uint_ascii

  ought to reproduce the original input.
