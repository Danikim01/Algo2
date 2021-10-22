VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2
CC = gcc
CFLAGS =-g -O0 -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror  -I cabeceras

all: clean correr-interactivo

interactivo: heap.c mostrar_abb.c 
	$(CC) $(CFLAGS) heap.c mostrar_abb.c -o interactivo 2>&1

correr-interactivo: interactivo
	valgrind $(VALGRIND_FLAGS) ./interactivo 2>&1

clean:
	rm -f interactivo 2>&1
