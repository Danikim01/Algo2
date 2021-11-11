VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2
CC = gcc
CFLAGS =-g -O0 -Wall -Werror -std=c99 -I cabeceras

all: valgrind

main: hash.c main.c lista.c
	$(CC) $(CFLAGS) hash.c main.c lista.c -o main 2>&1

valgrind: main
	valgrind $(VALGRIND_FLAGS) ./main 2>&1

.PHONY: clean
clean:
	rm -f main
