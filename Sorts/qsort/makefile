VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2
CC = gcc
CFLAGS =-g -O0 -Wall -Werror -std=c99 -I cabeceras

all: clean valgrind

qsort: quicksort.c
	$(CC) $(CFLAGS) quicksort.c -o qsort 2>&1

valgrind: qsort
	valgrind $(VALGRIND_FLAGS) ./qsort 2>&1

clean:
	rm -f qsort 2>&1