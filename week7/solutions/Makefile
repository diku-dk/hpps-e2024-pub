CC?=gcc
CFLAGS=-Wall -Wextra -pedantic -fopenmp

# A generic Makefile rule that assumes every program comes from one C file.
%: %.c util.c
	$(CC) $^ -o $@ $(CFLAGS)
