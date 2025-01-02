#pragma once

// Return the number of seconds since some unspecified time. Timing
// can be done by calling this function multiple times and subtracting
// the return values.
double seconds(void);

// Read ASCII integers from standard input. Returns an array of
// integers and writes the size of the array to *n.
int* read_ints(int *n);

// Write ASCII integers to standard output.
void write_ints(int n, int *arr);
