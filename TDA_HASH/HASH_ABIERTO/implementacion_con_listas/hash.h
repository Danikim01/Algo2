#ifndef HASH_H_
#define HASH_H_

#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct hash{
	size_t cantidad;
	size_t tamanio_tabla;
	lista_t** tabla;
}hash_t;

hash_t* hash_crear(size_t capacidad);

void hash_destruir(hash_t* hash);

bool hash_insertar(hash_t* hash,const char* clave,void* elemento);

void* hash_buscar(hash_t* hash,const char* clave);

bool hash_eliminar(hash_t* hash,const char* clave);

void hash_imprimir(hash_t* hash,void (*funcion)(void* elemento));

#endif //HASH_H_