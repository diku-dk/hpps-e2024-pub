CC=gcc
CFLAGS=-Wall -Wextra -pedantic

all: test_fibs

fibs.o: fibs.c
	$(CC) fibs.c -c $(CFLAGS)

test_fibs.o: test_fibs.c
	$(CC) test_fibs.c -c $(CFLAGS)

test_fibs: fibs.o test_fibs.o
	$(CC) fibs.o test_fibs.o -o test_fibs

clean:
	rm -f fibs.o test_fibs.o test_fibs
