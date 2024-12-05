CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g -fsanitize=address

all: test_list

list.o: list.c
	$(CC) list.c -c $(CFLAGS)

test_list.o: test_list.c
	$(CC) test_list.c -c $(CFLAGS)

test_list: list.o test_list.o
	$(CC) list.o test_list.o -o test_list $(CFLAGS)

clean:
	rm -f list.o test_list.o test_list
