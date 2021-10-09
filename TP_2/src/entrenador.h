#ifndef ENTRENADOR_H_
#define ENTRENADOR_H_

#include <stdlib.h>
#include "lista.h"
#include "abb.h"

typedef struct _entrenador_t entrenador_t;

/*
Pre condiciones : La funcion recibe un entrenador_t entrenador cuyo espacio en memoria dinamica fue reservado previamente (en este caso mediante un calloc lo cual llena de ceros los valores en memoria reservada)
y recibe, a su vez, el puntero a un puntero a char de elementos inicializados, los cuales corresponden al nombre y a la cantidad de victorias del entrenador a inicializar.
Post condiciones : La funcion inicializa del struct entrenador_t* el nombre del entrenador y la cantidad de victorias.
*/
void inicializar_nombre_y_victorias_entrenador(entrenador_t* entrenador, char** elementos);

/*
Reserva la memoria necesaria para crear un entrenador_t entrenador y la devuelve.
*/
entrenador_t* entrenador_crear();

/*
Devuelve la cantidad de victorias del entrenador, si existe el entrenador.
*/
int victorias_entrenador(entrenador_t* entrenador);
/*
Devuelve el nombre del entrenador, si existe el entrenador.
*/
char* nombre_entrenador(entrenador_t* entrenador);

/*
Devuelve la cantidad de pokemones del entrenador, si existe el entrenador.
*/
size_t cantidad_pokemones_entrenador(entrenador_t* entrenador);

/*
Recorre una lista de pokemones enviada por parametro usando el iterador de lista, se deja de iterar cuando algun pokemon de la lista
tenga el mismo nombre con el enviado por parametro y devuelve false (tambien en caso de error). Caso contrario devuelve true.
*/
bool entrenador_tiene_pokemon(lista_t* lista_pokemones,const char* nombre_pokemon);

/*
Devuelve la lista de pokemones del entrenador enviado por parametro.
*/
lista_t* lista_pokemones(entrenador_t* entrenador);

#endif // ENTRENADOR_H_
