#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define CANTIDAD_INICIAL_ELEMENTOS 0
#define TIENE_SIGUIENTE 1 
#define ERROR -1
#define EJECUCION_CORRECTA 0  

lista_t* lista_crear(lista_liberar_elemento destructor){
    lista_t* lista = malloc(sizeof(lista_t));
    if(!lista)
        return NULL;
    lista->nodo_inicio = NULL;
    lista->nodo_fin = NULL;
    lista->cantidad = CANTIDAD_INICIAL_ELEMENTOS;
    lista->destructor = destructor;
    return lista;
}

void avanzar_lista(nodo_t** nodo, int posicion, int posicion_aux){
    /*
    if(posicion_aux == posicion && (*nodo)!=NULL)
        return;
    if(posicion_aux < posicion && (*nodo)!=NULL)
        (*nodo)=(*nodo)->siguiente;
    avanzar_lista(nodo,posicion,posicion_aux+1);
    */
      for (int i = 0; i < posicion; ++i){
        if ((*nodo)!=NULL){
            (*nodo)=(*nodo)->siguiente;
        }
    }
}

int lista_insertar(lista_t* lista, void* elemento){
    if(!lista){
        return ERROR;
    }
    nodo_t* nodo_a_agregar = malloc(sizeof(nodo_t));
    if(!nodo_a_agregar){
        return ERROR;
    }
    nodo_a_agregar->elemento = elemento;
    nodo_a_agregar->siguiente = NULL;

    if(lista_vacia(lista)){
        lista->nodo_inicio= nodo_a_agregar;
        lista->nodo_fin= nodo_a_agregar;
    } else {
        
        lista->nodo_fin->siguiente = nodo_a_agregar;
        lista->nodo_fin = nodo_a_agregar;

    }
    (lista->cantidad)++;
    return EJECUCION_CORRECTA;
}

/*
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

    }else if( (int)posicion > lista_elementos(lista)-1 || (int)posicion < 0 || !posicion){
        
        nodo_t* aux_2=lista->nodo_inicio;
        avanzar_lista(&aux_2,(int)lista_elementos(lista)-1,0);
        aux_2->siguiente = nuevo_nodo;       
        
    }else{
        
        nodo_t* aux=lista->nodo_inicio;
        avanzar_lista(&aux,(int)posicion,0);
        nuevo_nodo->siguiente=aux;

        nodo_t* aux_2 = lista->nodo_inicio;
        avanzar_lista(&aux_2,(int)posicion-1,0);
        aux_2->siguiente=nuevo_nodo;
        
    }

    (lista->cantidad)++;
    return EJECUCION_CORRECTA;
}
*/

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
    if(!lista){
        return ERROR;
    }
    if((int)posicion >= ((lista->cantidad)) || lista->cantidad == 0){
        int insercion=lista_insertar(lista,elemento);
        return insercion;
    }
    nodo_t* nuevo_nodo=malloc(sizeof(nodo_t));
    if (!nuevo_nodo){
        return ERROR;
    }
    nuevo_nodo->siguiente=NULL;
    //nuevo_nodo->elemento=elemento;
    if (posicion==0){
        nuevo_nodo->siguiente=lista->nodo_inicio;
        lista->nodo_inicio=nuevo_nodo;
    }else{
        nodo_t* aux=lista->nodo_inicio;
        avanzar_lista(&aux,(int)posicion-1,0);
        nuevo_nodo->siguiente=aux->siguiente;
        aux->siguiente=nuevo_nodo;
    }
    (lista->cantidad)++;

    nuevo_nodo->elemento=elemento;
    return EJECUCION_CORRECTA;
}


int lista_borrar(lista_t* lista){
    if((lista==NULL)||(lista_vacia(lista))){
        return ERROR;
    }
    nodo_t* aux=lista->nodo_inicio;
    avanzar_lista(&aux,(int)(lista->cantidad)-2,0);
    if(lista->cantidad==1){
        if(lista->destructor){
            lista->destructor(aux->elemento);
        }
        free(aux);
        lista->nodo_inicio=NULL;
        lista->nodo_fin=NULL;
        (lista->cantidad)--;
        return EJECUCION_CORRECTA;
    }
    lista->nodo_fin=aux;
    if(lista->destructor){
        lista->destructor(aux->siguiente->elemento);
    }   
    free(aux->siguiente);
    lista->nodo_fin->siguiente=NULL;
    (lista->cantidad)--;
    return EJECUCION_CORRECTA;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
     if(!lista || lista_vacia(lista)){
        return ERROR;
    }

    if (posicion==0){
        nodo_t* nodo_a_eliminar = lista->nodo_inicio;
        lista->nodo_inicio = nodo_a_eliminar->siguiente;
        if(lista->destructor){
            lista->destructor(nodo_a_eliminar->elemento);
        }
        free(nodo_a_eliminar);
        lista->cantidad--;
        return EJECUCION_CORRECTA;
    }else if((int)posicion>((lista->cantidad)-1) || (int)posicion < 0){
        int eliminacion = lista_borrar(lista);
        return eliminacion;
    } else {
        nodo_t* nodo_aux = lista->nodo_inicio;
        avanzar_lista(&(nodo_aux),(int)posicion,0);
        nodo_t* nodo_anterior = lista->nodo_inicio;
        avanzar_lista(&(nodo_anterior),(int)posicion-1,0);
        nodo_anterior->siguiente = nodo_aux->siguiente;
        if(lista->destructor){
            lista->destructor(nodo_aux->elemento);
        }
        free(nodo_aux);
        lista->cantidad--;
    }
   
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
        return (aux->elemento);
    } else {
        avanzar_lista(&(aux),(int)lista->cantidad-1,0);
        return (aux->elemento);
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
/*
void lista_destruir(lista_t* lista){
    
    if(!lista)
        return;
    while(!lista_vacia(lista))
        lista_borrar(lista);
    free(lista);
}
*/

void nodo_destruir(lista_t* lista,nodo_t* nodo){
    if(nodo != NULL){
        nodo_destruir(lista,nodo->siguiente);
        if(lista->destructor){
            lista->destructor(nodo->elemento);
        }
        free(nodo);
    }else{
        return;
    }
}
void lista_destruir(lista_t* lista){
    if(lista != NULL){
        nodo_destruir(lista,lista->nodo_inicio);
        free(lista);
    }else{
        return;
    }
}



lista_iterador_t* lista_iterador_crear(lista_t* lista){

    if(!lista)
        return NULL;

    lista_iterador_t* iterador = malloc(sizeof(lista_iterador_t));
    
    iterador->corriente = lista->nodo_inicio;
    iterador->lista = lista;

    return iterador;
}
bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
    if(!iterador)
        return false;
    return (iterador->corriente != NULL);
}

/*
 * Avanza el iterador al siguiente elemento.
 * Devuelve TIENE_SIGUIENTE si pudo avanzar el iterador o ERROR en caso de
 * que no queden elementos o en caso de error.
*/
/*
int iterador_lista_siguiente(lista_iterador_t* iterador){

    if(!iterador || (iterador->corriente == NULL) || lista_vacia(iterador->lista) || !lista_iterador_tiene_siguiente(iterador)){
        return ERROR;
    }
    iterador->corriente = iterador->corriente->siguiente;
    return TIENE_SIGUIENTE;

}
*/
bool lista_iterador_avanzar(lista_iterador_t* iterador){
    if(!lista_iterador_tiene_siguiente(iterador))
        return false;
    iterador->corriente = iterador->corriente->siguiente;
    return (iterador->corriente != NULL);
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
size_t lista_con_cada_elemento(lista_t* lista,bool (*funcion)(void*,void*),void* contexto){
    if(!lista || !funcion){
        return 0;
    }
    
    size_t elementos_recorridos = CANTIDAD_INICIAL_ELEMENTOS;

    lista_iterador_t* it = NULL;

    for(it = lista_iterador_crear(lista); lista_iterador_tiene_siguiente(it); lista_iterador_avanzar(it)){

        if(it){
            elementos_recorridos++;
            if(!funcion(lista_iterador_elemento_actual(it),contexto)){
                lista_iterador_destruir(it);
                return elementos_recorridos;
            }
        } else {
            return 0;
        }
            
    }

    lista_iterador_destruir(it);
    return elementos_recorridos;
        
}





