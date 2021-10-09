CFLAGS=-g -O2 -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes

compilar:
	gcc $(CFLAGS) -o join main.c strutil.c split.c

valgrind: compilar
	valgrind $(VFLAGS) ./join
