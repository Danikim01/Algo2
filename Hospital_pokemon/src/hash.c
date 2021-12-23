#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "hash.h"

const int MINIMA_CAPACIDAD_HASH = 3;
#define FACTOR_DE_CARGA 0.75
#define ERROR -1
#define EJECUCUCION_CORRECTA 0
#define CANTIDAD_INICIAL_ELEMENTOS 0

typedef enum{VACIO, BORRADO, OCUPADO} estado_t; //Los 3 posibles estados para los elementos dentro de una tabla hash

typedef struct tabla tabla_t;

typedef struct hash_iter hash_iter_t;

struct tabla{
  char* clave;
  void* dato;
  estado_t estado;
};

struct hash_iter{
	tabla_t* corriente;
	const hash_t* hash;
	size_t posicion;
};

struct hash{
	hash_destruir_dato_t destructor;
	size_t capacidad; //tamaño del arreglo.
	size_t cantidad_ocupados; //cantidad de lugares ocupados.
	size_t cantidad_borrados; //cantidad de lugares borrados.
	tabla_t* tabla;//arreglo de nodos.
};

//recibe la capacidad de la tabla de hash y la incializa.
tabla_t* crear_e_inicializar_tabla_hash(size_t capacidad_inicial){

	tabla_t* nodo = malloc(capacidad_inicial*sizeof(tabla_t));
	if(!nodo)
		return NULL;
	for(int i=0; i < capacidad_inicial;i++){
		nodo[i].estado = VACIO;
		nodo[i].dato = NULL;
		nodo[i].clave = NULL;
	}
	return nodo;
}

hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad_inicial){
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash){
		return NULL;
	}
	if(capacidad_inicial >= MINIMA_CAPACIDAD_HASH){
		hash->capacidad = capacidad_inicial;
	}else{
		hash->capacidad = (size_t)MINIMA_CAPACIDAD_HASH;
	}
	
	hash->tabla = crear_e_inicializar_tabla_hash(hash->capacidad);
	if(!hash->tabla){
		free(hash);
		return NULL;
	}
	hash->cantidad_ocupados = CANTIDAD_INICIAL_ELEMENTOS;
	hash->cantidad_borrados = CANTIDAD_INICIAL_ELEMENTOS;
	hash->destructor = destruir_elemento;
	return hash;
}

//Devuelve una posicion dentro de un rango dado.
size_t funcion_hash(const char* clave, size_t tam){
    char* suma = NULL;
    size_t codigo = 0;

    for(size_t i=0;clave[i]; i++){
       // codigo = clave[i];
        suma+=clave[i];
    }
    codigo = (size_t)suma;

    return codigo%tam;
}
/*
size_t fhash(const char* clave, size_t tam)
{
   unsigned long number = 5381;
   int c;
   while ((c = *clave++))
	   number = ((number << 5) + number) + c;
   return number % tam;
}
*/

/*
* Crea una copia reservada mediante malloc del string recibido y la devuelve (extraida del minidemo.c)
*/
char* strdup(const char* s){
	if(!s)
    return NULL;
	char* p = malloc(strlen(s)+1);
	strcpy(p,s);
	return p;
}

//Devuelve true si la clave recibida por parametro es la misma que la clave de un elemento determinado de la tabla de hash.
bool misma_clave(const hash_t* hash, const char* clave, size_t posicion){
	return (hash->tabla[posicion].estado == OCUPADO && strcmp(hash->tabla[posicion].clave, clave) == 0);
}

//Pre : Tiene que haber espacio disponible en el hash (para lo cual me aseguro de rehashear)
//Realiza un probing lineal a traves de toda la tabla de hash buscando la proxima posicion libre inmedianta.
size_t nueva_posicion(const hash_t* hash,size_t tope,size_t posicion,const char* clave){
	if(posicion>=tope)
		posicion = 0;

	while(hash->tabla[posicion].estado == OCUPADO && !misma_clave(hash,clave,posicion)){
		posicion++;
		if(posicion>=tope)
			posicion = 0;	
	}

	return posicion;
}

//Obtiene una posicion valida y libre dentro de la tabla de hash dado un hash con espacio disponible.
size_t obtener_posicion_valida(const hash_t* hash, const char *clave){
  size_t posicion = funcion_hash(clave,hash->capacidad);
  if(hash->tabla[posicion].estado == VACIO || hash->tabla[posicion].estado == BORRADO || misma_clave(hash, clave, posicion)){
  	return posicion;
  }
  return (nueva_posicion(hash, hash->capacidad, posicion,clave));
}

//Recibe un hash, el elemento a insertar y una posicion valida de la tabla de hash.
//Inserta el elemento a la tabla de hash a su respectiva posicion.
void almacenar_valor(tabla_t* tabla_hash, void* elemento, size_t posicion){
	tabla_hash[posicion].dato = elemento;
	tabla_hash[posicion].estado = OCUPADO;
}

//Recibe un hash y retorna el factor de carga del mismo (cantidad elementos / capacidad)
double calcular_factor_de_carga(hash_t* hash){
	return ((double)hash->cantidad_ocupados/(double)hash->capacidad); 
}

//rehashe el hash, es decir, reposiciona todos los elementos de la misma en un hash mas grande.
bool rehashear_nueva_tabla(hash_t* hash,tabla_t* tabla_anterior,size_t capacidad_original){
	
	size_t posicion;
	for(int i = 0; i < capacidad_original; i++){
		if(tabla_anterior[i].estado == OCUPADO){
			posicion = obtener_posicion_valida(hash, tabla_anterior[i].clave);
		    if(posicion > hash->capacidad){
		    	return false;
		    }
		  	hash->tabla[posicion].clave = tabla_anterior[i].clave;
		    almacenar_valor(hash->tabla, tabla_anterior[i].dato, posicion);
		}
	}
	return true;
	
}
/*Dada una tabla de hash previamente inicializada agranda la capacidad de la tabla al doble y rehashea los elementos la misma
* si se pudo hacer la operacion devuelve true, y si no pudo restaura todo a la normalidad y devuelve false.
*/
bool rehashear(hash_t* hash){
	tabla_t* tabla_original = hash->tabla;
	hash->tabla = crear_e_inicializar_tabla_hash(hash->capacidad*2);
	if(!hash->tabla){
		hash->tabla = tabla_original;
		return false;
	}
	size_t capacidad_original = hash->capacidad;
	size_t nueva_capacidad = hash->capacidad*2;
	hash->capacidad = nueva_capacidad;
	if(!rehashear_nueva_tabla(hash,tabla_original,capacidad_original)){
		printf("No se pudo realizar el rehasheo\n");	
		hash->tabla = tabla_original;
		hash->capacidad = capacidad_original;
		free(tabla_original);
		return false;
	}

	free(tabla_original);
	return true;

}


int hash_insertar(hash_t* hash, const char* clave, void* elemento){
	if(!hash || !clave) return ERROR;
	//Antes de todo chequeo si tengo que rehashear
	
	if(calcular_factor_de_carga(hash) >= FACTOR_DE_CARGA){
		if(!rehashear(hash)){
			return ERROR;
		}
	}

	size_t posicion = obtener_posicion_valida(hash,clave);

	//caso en que la clave sea la misma: reemplazo
	
	if(misma_clave(hash,clave,posicion)){
		if(hash->destructor != NULL){
			hash->destructor(hash->tabla[posicion].dato);
		}
	}else{
		//inserto la clave
		hash->tabla[posicion].clave = strdup(clave);
		hash->cantidad_ocupados++;
	}
	//inserto valor
	almacenar_valor(hash->tabla,elemento,posicion);

	//me aseguro de que haya espacio disponible en todo momento.
	if(calcular_factor_de_carga(hash) >= FACTOR_DE_CARGA){
		if(!rehashear(hash)){
			return ERROR;
		}
	}

  	return EJECUCUCION_CORRECTA;
}

//Devuelve true si el estado (flag) de cierto lugar dentro de una tabla de hash esta ocupado o hubo un elemento previamente y fue borrado.
bool hay_siguiente(tabla_t* tabla_hash){
	return (tabla_hash->estado == OCUPADO || tabla_hash->estado == BORRADO);
}

//Borra un elemento determinado de la tabla de hash (incializada) aplicando el destructor.
void borrar_elemento(hash_t* hash,size_t posicion){
	hash->cantidad_borrados += 1;
	hash->tabla[posicion].estado = BORRADO;
	if(hash->destructor != NULL){
		hash->destructor(hash->tabla[posicion].dato);
	}
	free(hash->tabla[posicion].clave);	
}

int hash_quitar(hash_t* hash, const char* clave){

	 if(!hash || !clave)
    	return ERROR;
    size_t posicion = obtener_posicion_valida(hash,clave);
    if(misma_clave(hash,clave,posicion)){
    	borrar_elemento(hash,posicion);
    	return EJECUCUCION_CORRECTA;
    }
    return ERROR;
   
}

void* hash_obtener(hash_t* hash, const char* clave){
	if(!hash || !clave)
		return NULL;
	
	size_t posicion = obtener_posicion_valida(hash,clave);
	if(misma_clave(hash,clave,posicion)){
    	return hash->tabla[posicion].dato;
  	}
  	return NULL;
  
}

size_t hash_cantidad(hash_t* hash){
	if(!hash) return CANTIDAD_INICIAL_ELEMENTOS;
  	return (hash->cantidad_ocupados - hash->cantidad_borrados);
}

bool hash_contiene(hash_t* hash, const char* clave){

	if(!hash || !clave)
		return false;

	size_t posicion = obtener_posicion_valida(hash,clave);
	if(misma_clave(hash,clave,posicion)){
		return true;
	}
	return false;

}

void hash_destruir(hash_t* hash){
	if(!hash) return;
	for(int i = 0; i < hash->capacidad; i++){
    	if(hash->tabla != NULL && hash->tabla[i].estado == OCUPADO){
      		if(hash->destructor){
        		hash->destructor(hash->tabla[i].dato);
      		}
      	free(hash->tabla[i].clave);
    	}
  	}
  	
	free(hash->tabla);
	free(hash);
}

//Devuelve true si el estado (flag) de cierto lugar dentro de una tabla de hash esta desocupado.
bool siguiente_no_esta_ocupado(tabla_t* tabla_aux){
	return (tabla_aux->estado != OCUPADO);
}

//Devuelve un iterador que apunta al primer elemento válido de la
void hash_iter_avanzar_aux(hash_iter_t* iterador){
	
	while(iterador->posicion < iterador->hash->capacidad && iterador->hash->tabla[iterador->posicion].estado != OCUPADO){
		iterador->posicion += 1;
	}
	if (iterador->posicion == iterador->hash->capacidad){
	    iterador->corriente = NULL;
	    return;
  	}
  	iterador->corriente = &(iterador->hash->tabla[iterador->posicion]);

}

//Crea un iterador sobre la tabla de hash
//Pre: La tabla de hash fue creada
//Post: Devuelve un iterador que apunta al primer elemento válido de la
//corriente
hash_iter_t* hash_iter_crear(const hash_t *hash){
	if(!hash || hash->cantidad_ocupados == 0){
		return NULL;
	}
	hash_iter_t* iter= malloc(sizeof(hash_iter_t));
	if(!iter)
		return NULL;
	iter->hash = hash;
	iter->posicion = 0;
	hash_iter_avanzar_aux(iter);
  	
	return iter;
}

//Determina si el iterador está al final de la tabla de hash
//Pre: el iterador fue creado
//Post: Devuelve true si el iterador está al final, false en caso contrario.
bool hash_iter_al_final(const hash_iter_t* iter){
	//el iterador esta en el final si el nodo corriente apunta a NULL.
	if(iter->corriente != NULL)
		return false;
	return true;
}


//Muestra la clave asociada a la posición en la que se encuentra el iterador
//Pre: El iterador fue creado
//Post: Devuelve la clave de la posición actual del iterador.

const char* hash_iter_ver_actual(const hash_iter_t* iter){
	if(hash_iter_al_final(iter) || !iter){
		return NULL;
	}
	//return (iter->hash->tabla[iter->posicion].clave);
	return (iter->corriente->clave);
}

//Avanza sobre la tabla
//Pre: El iterador fue creado
//Post: Devuelve true si logra avanzar al siguiente elemento de la tabla
//false si no hay más elementos para iterar

bool hash_iter_avanzar(hash_iter_t *iter){
	if(hash_iter_al_final(iter) || !iter)
		return false;
	iter->posicion += 1;
	hash_iter_avanzar_aux(iter);
	return true;
}

//Destruye el iterador de hash
//Pre: El iterador fue creado
//Post: El iterador es destruido
void hash_iter_destruir(hash_iter_t* iter){
	free(iter);
}

size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
	if(!hash || !funcion)
		return 0;
	size_t cantidad_recorridos = 0;
	hash_iter_t* it = NULL;
	
	for(it = hash_iter_crear(hash);!hash_iter_al_final(it);hash_iter_avanzar(it)){
		if(it){
			cantidad_recorridos++;
			if(funcion(hash,hash_iter_ver_actual(it),aux)){
				hash_iter_destruir(it);
				return cantidad_recorridos;
			}
		}else{
			printf("No se pudo crear el iterador\n");
			return 0;
		}
	}
	
	hash_iter_destruir(it);
	return cantidad_recorridos;
}

