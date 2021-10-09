#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define CANTIDAD_INICIAL_ELEMENTOS 0
#define TIENE_SIGUIENTE 1 
#define ERROR -1
#define EJECUCION_CORRECTA 0  


lista_t* lista_crear(){
    lista_t* lista = malloc(sizeof(lista_t));
    if(!lista)
        return NULL;
    lista->nodo_inicio = NULL;
    lista->nodo_fin = NULL;
    lista->cantidad = CANTIDAD_INICIAL_ELEMENTOS;
    return lista;
}

void avanzar_lista(nodo_t** nodo, int posicion, int posicion_aux){
    if(posicion_aux == posicion && (*nodo)!=NULL)
        return;
    if(posicion_aux < posicion && (*nodo)!=NULL)
        (*nodo)=(*nodo)->siguiente;
    avanzar_lista(nodo,posicion,posicion_aux+1);
}

int lista_insertar(lista_t* lista, void* elemento){
    if(!lista){
        return ERROR;
    }
    nodo_t* nodo_a_agregar = calloc(1,sizeof(nodo_t));
    if(!nodo_a_agregar){
        return ERROR;
    }
    nodo_a_agregar->elemento = elemento;
    nodo_a_agregar->siguiente = NULL;
    if(lista_vacia(lista)){
        nodo_a_agregar->siguiente=lista->nodo_inicio;
        lista->nodo_inicio= nodo_a_agregar;
        lista->nodo_fin= nodo_a_agregar;
    } else {
       lista->nodo_fin->siguiente = nodo_a_agregar;
       lista->nodo_fin = nodo_a_agregar;
    }
    (lista->cantidad)++;
    return EJECUCION_CORRECTA;
}


int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
    if(!lista){
        return ERROR;
    }
    nodo_t* nuevo_nodo=calloc(1,sizeof(nodo_t));
    if (!nuevo_nodo){
        return ERROR;
    }
    nuevo_nodo->siguiente=NULL;
    nuevo_nodo->elemento=elemento;
    if (posicion==0){
        nuevo_nodo->siguiente=lista->nodo_inicio;
        lista->nodo_inicio=nuevo_nodo;
    }else if((int)posicion > ((lista->cantidad)-1) || (int)posicion < 0){
    	
    	nodo_t* aux=lista->nodo_inicio;
        avanzar_lista(&aux,(int)lista_elementos(lista)-1,0);
        nuevo_nodo->siguiente=aux->siguiente;
        aux->siguiente=nuevo_nodo;

    }else{
        nodo_t* aux=lista->nodo_inicio;
        avanzar_lista(&aux,(int)posicion-1,0);
        nuevo_nodo->siguiente=aux->siguiente;
        aux->siguiente=nuevo_nodo;
    }
    (lista->cantidad)++;
    return EJECUCION_CORRECTA;
}

int lista_borrar(lista_t* lista){
    if(!lista || lista_vacia(lista)){
        return ERROR;
    }
    if(lista->cantidad==1){
        lista->nodo_inicio=NULL;
        lista->nodo_fin=NULL;
        (lista->cantidad)--;
    } else {
        nodo_t* aux=lista->nodo_inicio;
        avanzar_lista(&aux,(int)(lista->cantidad)-2,0);
        lista->nodo_fin = aux;
        free(aux->siguiente);
        lista->cantidad--;
        lista->nodo_fin->siguiente = NULL;
    }
    return EJECUCION_CORRECTA;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
     if(!lista || lista_vacia(lista)){
        return ERROR;
    }
    if((int)posicion>((lista->cantidad)-1) || (int)posicion < 0){
        int eliminacion = lista_borrar(lista);
        return eliminacion;
    }
    if (posicion==0){
        nodo_t* nodo_a_eliminar = lista->nodo_inicio;
        lista->nodo_inicio = nodo_a_eliminar->siguiente;
        free(nodo_a_eliminar);
        lista->cantidad--;
        return EJECUCION_CORRECTA;
    }
    nodo_t* nodo_aux = lista->nodo_inicio;
    avanzar_lista(&(nodo_aux),(int)posicion,0);
    nodo_t* nodo_anterior = lista->nodo_inicio;
    avanzar_lista(&(nodo_anterior),(int)posicion-1,0);
    nodo_anterior->siguiente = nodo_aux->siguiente;
    free(nodo_aux);
    lista->cantidad--;
    return EJECUCION_CORRECTA;
}
void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
    if((lista==NULL)||(lista_vacia(lista))){
        return NULL;
    }
    if(posicion>((lista->cantidad)-1)){
        return NULL;
    }
    nodo_t* aux=lista->nodo_inicio;
    if (posicion==0){
        return aux->elemento;
    }else{
        avanzar_lista(&aux,(int)posicion,0);
        return aux->elemento;
    }
}

void* lista_ultimo(lista_t* lista){
    if(!lista || lista_vacia(lista)){
    	return NULL;
    }

    nodo_t* aux = lista->nodo_inicio;

    if(lista->cantidad == 1){
    	return aux->elemento;
    } else {
    	avanzar_lista(&(aux),(int)lista->cantidad-1,0);
    	return aux->elemento;
    }
}
bool lista_vacia(lista_t* lista){
    if(lista==NULL || ((lista->cantidad)==0)){
        return true; 
    }
    return false;
}
size_t lista_elementos(lista_t* lista){
	if(!lista || lista_vacia(lista)){
		return EJECUCION_CORRECTA;
	}
    return (lista->cantidad);
}

int lista_apilar(lista_t* lista, void* elemento){
	if(!lista)
		return ERROR;
    return (lista_insertar_en_posicion(lista,elemento,0));
}

int lista_desapilar(lista_t* lista){
    if(!lista)
    	return ERROR;
    return (lista_borrar_de_posicion(lista,0));
}

void* lista_tope(lista_t* lista){
    if(!lista || lista_vacia(lista))
    	return NULL;
    return(lista_primero(lista));
}

int lista_encolar(lista_t* lista, void* elemento){
	
	if(!lista)
		return ERROR;
	return(lista_insertar(lista,elemento));
}

int lista_desencolar(lista_t* lista){
    if(!lista)
    	return ERROR;
    size_t primera_posicion = 0;
    return (lista_borrar_de_posicion(lista,primera_posicion));
}

void* lista_primero(lista_t* lista){
    if(!lista || lista_vacia(lista))
    	return NULL;
    size_t primer_posicion = 0;
    return(lista_elemento_en_posicion(lista,primer_posicion));
}

void lista_destruir(lista_t* lista){
	
	if(!lista)
		return;
	while(!lista_vacia(lista))
		lista_borrar_de_posicion(lista,0);
	free(lista);

}
lista_iterador_t* lista_iterador_crear(lista_t* lista){

	if(lista_vacia(lista))
		return NULL;

	lista_iterador_t* iterador = malloc(sizeof(lista_iterador_t));
	
	iterador->corriente = lista->nodo_inicio;
	iterador->lista = lista;

    return iterador;
}
bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
	
	if((iterador->corriente) && (iterador)){
		return true;
	}
	return false;
}
/*
 * Avanza el iterador al siguiente elemento.
 * Devuelve TIENE_SIGUIENTE si pudo avanzar el iterador o ERROR en caso de
 * que no queden elementos o en caso de error.
*/
int iterador_lista_siguiente(lista_iterador_t* iterador){

	if(!iterador || (iterador->corriente == NULL) || lista_vacia(iterador->lista))
		return ERROR;
	iterador->corriente = iterador->corriente->siguiente;
    return TIENE_SIGUIENTE;

}
bool lista_iterador_avanzar(lista_iterador_t* iterador){
    return (iterador_lista_siguiente(iterador) == TIENE_SIGUIENTE);
}
void* lista_iterador_elemento_actual(lista_iterador_t* iterador){
	if(!iterador || (iterador->corriente == NULL)){
		return NULL;
	}
	void* elemento = iterador->corriente->elemento;
	return elemento;
}
void lista_iterador_destruir(lista_iterador_t* iterador){
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto){

	if((!funcion && !contexto) || !lista || lista_vacia(lista)){
		return 0;
	}

	size_t elementos_recorridos = CANTIDAD_INICIAL_ELEMENTOS;
	lista_iterador_t* it = NULL;
	it = lista_iterador_crear(lista);
	if(!it)
		return 0;
	void* elemento_actual = lista_iterador_elemento_actual(it);

	while (funcion(elemento_actual,contexto) && it && elemento_actual){
    	elementos_recorridos++;
    	lista_iterador_avanzar(it);
    	elemento_actual = lista_iterador_elemento_actual(it);
    }

	lista_iterador_destruir(it);

    return elementos_recorridos;

}
