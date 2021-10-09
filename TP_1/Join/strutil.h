#ifndef STRUTIL_H
#define STRUTIL_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

size_t vtrlen(void* ptr);
/*
 * Devuelve una cadena, allocada dinámicamente, resultado de unir todas las
 * cadenas del arreglo terminado en NULL ‘strv’.
 *
 * Quien llama a la función toma responsabilidad de la memoria dinámica de la
 * cadena devuelta. La función devuelve NULL si no se pudo allocar memoria.
 */
char* join(char** strv, char sep);

/*
 * Libera un arreglo dinámico de cadenas, y todas las cadenas que contiene.
 */
void free_strv(char** cadena);

#endif  // STRUTIL_H
