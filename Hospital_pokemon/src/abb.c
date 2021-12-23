#include "abb.h"
#include <stdio.h>

#define EJECUCION_CORRECTA 0
#define ERROR -1
#define CORTAR_ITERACION 0
#define RETORNAR 1
#define ELEMENTOS_IGUALES 0
#define CANTIDAD_ELEMENTOS_INICIALES 0

/*
 * Crea el arbol y reserva la memoria necesaria de la estructura.
 * Comparador se utiliza para comparar dos elementos.
 * Destructor es invocado sobre cada elemento que sale del arbol,
 * puede ser NULL indicando que no se debe utilizar un destructor.
 *
 * Devuelve un puntero al arbol creado o NULL en caso de error.
 */
abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){

    abb_t* arbol = malloc(sizeof(abb_t));
    if(!arbol)
        return NULL;
    arbol->nodo_raiz = NULL;
    if(comparador){
        arbol->comparador = comparador;
    } else {
        free(arbol);
        return NULL;
    }
    arbol->destructor = destructor;
    return arbol;
}
/*
Esta funcion recibe un nodo_abb_t* nodo_actual a partir del cual se van a insertar los nodos nodo_abb_t* nodo_a_agregar 
recursivamente segun el criterio que rigue al abb_comparador comparador.
Si el valor de la comparacion de los dos elementos a comparar del nodo_actual y del nodo a agregar es negativo, quiere decir
que el elemento se inserta a la derecha del nodo_actual y en el caso de que el valor de la comparacion sea menor o igual, se inserta 
a la izquierda. Finalmente se devuelve el nodo_actual.
*/
void* insercion_recursiva(nodo_abb_t* nodo_actual,nodo_abb_t* nodo_a_agregar, abb_comparador comparador){

    if(!nodo_actual){
        return nodo_a_agregar;
    }

    int comparacion = comparador(nodo_actual->elemento,nodo_a_agregar->elemento);

    if(comparacion < ELEMENTOS_IGUALES){
        //inserto a la derecha
        nodo_actual->derecha = insercion_recursiva(nodo_actual->derecha,nodo_a_agregar,comparador);

    }else if (comparacion >= ELEMENTOS_IGUALES){
        //inserto a la izquierda
        nodo_actual->izquierda = insercion_recursiva(nodo_actual->izquierda,nodo_a_agregar,comparador);
    }

    return nodo_actual;

}

int arbol_insertar(abb_t* arbol, void* elemento){

    if(!arbol){
        return ERROR;
    }

    nodo_abb_t* nodo_a_insertar = malloc(sizeof(nodo_abb_t));
    if(!nodo_a_insertar){
        return ERROR;
    }

    nodo_a_insertar->elemento = elemento;
    nodo_a_insertar->derecha = NULL;
    nodo_a_insertar->izquierda = NULL;
    
    if(arbol_vacio(arbol)){
        arbol->nodo_raiz = nodo_a_insertar;
    }else{
        insercion_recursiva(arbol->nodo_raiz,nodo_a_insertar,arbol->comparador);
    }
    return EJECUCION_CORRECTA;
}

/*
* Retorno true, si el nodo_actual es hoja. Es decir que no tiene ni hijo derecho ni hijo izquierdo. 
* En caso contrario devuelve false.
*/
bool es_nodo_hoja(nodo_abb_t* nodo_actual){
    return(nodo_actual->izquierda == NULL && nodo_actual->derecha == NULL);
}
/*
* Retorno true, si el nodo_actual es tiene dos hijos. Es decir que tiene hijo derecho e hijo izquierdo. 
* En caso contrario devuelve false.
*/

bool tiene_dos_hijos(nodo_abb_t* nodo_actual){
    return((nodo_actual->izquierda != NULL && nodo_actual->derecha != NULL));
}
/*
* Retorno true, si el nodo_actual tiene hijo izquierdo.  
* En caso contrario devuelve false.
*/
bool tiene_hijo_izquierdo(nodo_abb_t* nodo_actual){
    return((nodo_actual->izquierda != NULL && nodo_actual->derecha == NULL));
}

/*
 * La funcion se encarga de encontrar el mayor de los menores y 
 * asigna ese nodo al nodo nodo_abb_t** nodo_aux enviado por parametro a la funcion.
 * A su vez, la funcion devuelve el nodo hijo del predecesor inorden. En caso de que no lo tenga simplemente
 * devolver NULL.
*/
void* extraer_nodo_mas_derecho(nodo_abb_t* nodo_actual,nodo_abb_t** nodo_aux){
    
    if(!nodo_actual->derecha){
        *nodo_aux = nodo_actual;
        return nodo_actual->izquierda;
    }
    nodo_actual->derecha = extraer_nodo_mas_derecho(nodo_actual->derecha,nodo_aux);
    return nodo_actual;
}

/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación) y si lo encuentra lo quita del arbol.
 * Adicionalmente, si encuentra el elemento, invoca el destructor con
 * dicho elemento.
 */
void* eliminacion_recursiva(nodo_abb_t* nodo_actual,void* elemento_a_eliminar, abb_comparador comparador,abb_liberar_elemento destructor){

    if(!nodo_actual)
        return NULL;

    int comparacion = comparador(elemento_a_eliminar,nodo_actual->elemento);

    if(comparacion == ELEMENTOS_IGUALES){

        if(es_nodo_hoja(nodo_actual)){
            
            if(destructor){
                destructor(nodo_actual->elemento);
            }
            free(nodo_actual);
            return NULL;

        }else if(tiene_dos_hijos(nodo_actual)){

            nodo_abb_t* sucesor = NULL;
            nodo_abb_t* izquierdo = extraer_nodo_mas_derecho(nodo_actual->izquierda,&sucesor);

            sucesor->izquierda = izquierdo;
            sucesor->derecha = nodo_actual->derecha;

            if(destructor){
                destructor(nodo_actual->elemento);
            }
            
            free(nodo_actual);
            return sucesor;
        } else if(tiene_hijo_izquierdo(nodo_actual)){
            nodo_abb_t* nodo_aux=NULL;
            nodo_aux=nodo_actual->izquierda;
            if (destructor!=NULL){
                destructor(nodo_actual->elemento);
            }
            free(nodo_actual);
            return nodo_aux;
        }else{
            nodo_abb_t* nodo_aux=NULL;
            nodo_aux=nodo_actual->derecha;
            if (destructor!=NULL){
                destructor(nodo_actual->elemento);
            }
            free(nodo_actual);
            return nodo_aux;
        }
    }

    if(comparacion > ELEMENTOS_IGUALES){
        nodo_actual->derecha = eliminacion_recursiva(nodo_actual->derecha,elemento_a_eliminar,comparador,destructor);
        return nodo_actual;
    } 

    if(comparacion < ELEMENTOS_IGUALES){
        nodo_actual->izquierda = eliminacion_recursiva(nodo_actual->izquierda,elemento_a_eliminar,comparador,destructor);
        return nodo_actual;
    }

    return nodo_actual;
}

int arbol_borrar(abb_t* arbol, void* elemento){
    
    if(!arbol || arbol_vacio(arbol) || arbol_buscar(arbol,elemento) == NULL){
        return ERROR;
    }

    arbol->nodo_raiz = eliminacion_recursiva(arbol->nodo_raiz,elemento,arbol->comparador,arbol->destructor);
    return EJECUCION_CORRECTA;
}

/*
* Busca recursivamente por medio del comparador por todo el arbol el elemento_a_buscar. En caso de encontrar
* al elemento buscado, lo devuelve, y en caso contrario, devuelve NULL.
*/
void* busqueda_recursiva(nodo_abb_t* nodo_actual,void* elemento_a_buscar, abb_comparador comparador){

    if(!nodo_actual)
        return NULL;

    int comparacion = comparador(elemento_a_buscar,nodo_actual->elemento);

    if(comparacion == ELEMENTOS_IGUALES){
        return nodo_actual->elemento;
    }else if(comparacion > ELEMENTOS_IGUALES){
        return (busqueda_recursiva(nodo_actual->derecha,elemento_a_buscar,comparador));
    }else {
        return (busqueda_recursiva(nodo_actual->izquierda,elemento_a_buscar,comparador));
    }
}

void* arbol_buscar(abb_t* arbol, void* elemento){

    if(!arbol || arbol_vacio(arbol))
        return NULL;

    void *elemento_buscado = busqueda_recursiva(arbol->nodo_raiz,elemento,arbol->comparador);

    if(elemento_buscado){
        return elemento_buscado;
    } else {
        return NULL;
    }
}

void* arbol_raiz(abb_t* arbol){
    if ((arbol==NULL)||(arbol_vacio(arbol))){
        return NULL;
    }
    return (arbol->nodo_raiz->elemento);
}
bool arbol_vacio(abb_t* arbol){
    if(!arbol || !arbol->nodo_raiz){
        return true;
    }
    return false;
}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia inorden.
 * Almacena (en la variable cantidad_recorridos) la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y almacena la cantidad de elementos que
 * pudo poner).
 */
void recorrido_inorden(nodo_abb_t* nodo_actual, void** array, size_t tamanio_array,size_t* cantidad_recorridos){

    if(!nodo_actual)
        return;

    if(nodo_actual->izquierda){
        recorrido_inorden(nodo_actual->izquierda,array,tamanio_array,cantidad_recorridos);
    }
   
    if(nodo_actual){

        if(*cantidad_recorridos < tamanio_array){
            array[*cantidad_recorridos] = nodo_actual->elemento;
            *cantidad_recorridos += 1;
        }else{
            return;
        } 
    }  
        
    if(nodo_actual->derecha){
        recorrido_inorden(nodo_actual->derecha,array,tamanio_array,cantidad_recorridos);
    }
 
}


size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array){

    if(!arbol || arbol_vacio(arbol)){
        return CANTIDAD_ELEMENTOS_INICIALES;
    }

    size_t cantidad_recorridos = CANTIDAD_ELEMENTOS_INICIALES;

    recorrido_inorden(arbol->nodo_raiz,array,tamanio_array,&cantidad_recorridos);

    return cantidad_recorridos;
}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia preorden.
 * Almacena (en la variable cantidad_recorridos) la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y almacena la cantidad de elementos que
 * pudo poner).
 */
void recorrido_preorden(nodo_abb_t* nodo_actual, void** array, size_t tamanio_array,size_t* cantidad_recorridos){
    
    if(!nodo_actual)
        return;

    if(nodo_actual){

        if(*cantidad_recorridos < tamanio_array){
            array[*cantidad_recorridos] = nodo_actual->elemento;
            *cantidad_recorridos += 1;
        }else{
            return;
        } 
    }  

    if(nodo_actual->izquierda){
        recorrido_preorden(nodo_actual->izquierda,array,tamanio_array,cantidad_recorridos);
    }
   
    if(nodo_actual->derecha){
        recorrido_preorden(nodo_actual->derecha,array,tamanio_array,cantidad_recorridos);
    }
}

size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array){

     if(!arbol || arbol_vacio(arbol)){
        return CANTIDAD_ELEMENTOS_INICIALES;
    }
    
    size_t cantidad_recorridos = CANTIDAD_ELEMENTOS_INICIALES;

    recorrido_preorden(arbol->nodo_raiz,array,tamanio_array,&cantidad_recorridos);

    return cantidad_recorridos;

}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia postorden.
 * Almacena (en la variable cantidad_recorridos) la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y almacena la cantidad de elementos que
 * pudo poner).
 */
void recorrido_postorden(nodo_abb_t* nodo_actual, void** array, size_t tamanio_array,size_t* cantidad_recorridos){
    
    if(!nodo_actual)
        return;

    if(nodo_actual->izquierda){
        recorrido_postorden(nodo_actual->izquierda,array,tamanio_array,cantidad_recorridos);
    }
   
    if(nodo_actual->derecha){
        recorrido_postorden(nodo_actual->derecha,array,tamanio_array,cantidad_recorridos);
    }

    if(nodo_actual){

        if(*cantidad_recorridos < tamanio_array){
            array[*cantidad_recorridos] = nodo_actual->elemento;
            *cantidad_recorridos += 1;
        }else{
            return;
        } 
    }  

}
size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array){
    
    if(!arbol || arbol_vacio(arbol))
        return CANTIDAD_ELEMENTOS_INICIALES;

    size_t cantidad_recorridos = CANTIDAD_ELEMENTOS_INICIALES;

    recorrido_postorden(arbol->nodo_raiz,array,tamanio_array,&cantidad_recorridos);

    return cantidad_recorridos;

}
/*
* Destruye la memoria almacenada para los nodos, primero destruyendo por medio del destructor el contenido del nodo
* (si el destructor no es nulo) y luego liberando la memoria que se haya reservado para el nodo.
*/
void nodo_destruir(nodo_abb_t* nodo,abb_liberar_elemento destructor){
    if(!nodo){
        return;
    }
    nodo_destruir(nodo->derecha,destructor);
    nodo_destruir(nodo->izquierda,destructor);
    if (destructor!=NULL){
        destructor(nodo->elemento);
    }
    free(nodo);
}
void arbol_destruir(abb_t* arbol){
    if(arbol){
        if(!arbol_vacio(arbol)){
            nodo_destruir(arbol->nodo_raiz,arbol->destructor);
        }
        free(arbol);
    } 
}

/*
 * Recorre el arbol en forma inorden e invoca la funcion con cada
 * elemento del mismo. Si la función devuelve true, se finaliza el
 * recorrido aun si quedan elementos por recorrer. Si devuelve false
 * se sigue recorriendo mientras queden elementos. 
 * Ademas almacena en la variable pasada por referencia "cantidad_recorridos"
 * la cantidad de elementos que se pudieron recorrer mientras la funcion que se haya
 * invocado devuelve false, es decir, mientras este permitido recorrer el arbol.
*/
int iterador_recorrido_inorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra,size_t* cantidad_recorridos){
    if (nodo==NULL){
        return RETORNAR;
    }
    if (!iterador_recorrido_inorden(nodo->izquierda,funcion,extra,cantidad_recorridos)){
        return CORTAR_ITERACION;
    }
    (*cantidad_recorridos) += 1;
    if((*funcion)(nodo->elemento,extra)){
        return CORTAR_ITERACION;
    }
    if (!iterador_recorrido_inorden(nodo->derecha,funcion,extra,cantidad_recorridos)){
        return CORTAR_ITERACION;
    }           
    return RETORNAR;
}

/*
 * Recorre el arbol en forma preorden e invoca la funcion con cada
 * elemento del mismo. Si la función devuelve true, se finaliza el
 * recorrido aun si quedan elementos por recorrer. Si devuelve false
 * se sigue recorriendo mientras queden elementos. 
 * Ademas almacena en la variable pasada por referencia "cantidad_recorridos"
 * la cantidad de elementos que se pudieron recorrer mientras la funcion que se haya
 * invocado devuelve false, es decir, mientras este permitido recorrer el arbol.
*/
int iterador_recorrido_preorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra,size_t* cantidad_recorridos){
    if (nodo==NULL){
        return RETORNAR;
    }
    (*cantidad_recorridos) += 1;
    if((*funcion)(nodo->elemento,extra)){
        return CORTAR_ITERACION;
    }
    if (!iterador_recorrido_preorden(nodo->izquierda,funcion,extra,cantidad_recorridos)){
        return CORTAR_ITERACION;
    }
    if (!iterador_recorrido_preorden(nodo->derecha,funcion,extra,cantidad_recorridos)){
        return CORTAR_ITERACION;
    }
    return RETORNAR;
}
/*
 * Recorre el arbol en forma postorden e invoca la funcion con cada
 * elemento del mismo. Si la función devuelve true, se finaliza el
 * recorrido aun si quedan elementos por recorrer. Si devuelve false
 * se sigue recorriendo mientras queden elementos. 
 * Ademas almacena en la variable pasada por referencia "cantidad_recorridos"
 * la cantidad de elementos que se pudieron recorrer mientras la funcion que se haya
 * invocado devuelve false, es decir, mientras este permitido recorrer el arbol.
*/
int iterador_recorrido_postorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra,size_t* cantidad_recorridos){
    if (nodo==NULL){
        return RETORNAR;
    }
    if (!iterador_recorrido_postorden(nodo->izquierda,funcion,extra,cantidad_recorridos)){
        return CORTAR_ITERACION;
    }
    if (!iterador_recorrido_postorden(nodo->derecha,funcion,extra,cantidad_recorridos)){
        return CORTAR_ITERACION;
    }         
    (*cantidad_recorridos) += 1;  
    if((*funcion)(nodo->elemento,extra)){
        return CORTAR_ITERACION;
    }
    return RETORNAR;
}

size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
    if ((!arbol)||(arbol_vacio(arbol)) || (!funcion)){
        return CANTIDAD_ELEMENTOS_INICIALES;
    }
    size_t cantidad_recorridos = CANTIDAD_ELEMENTOS_INICIALES;

    if(recorrido == ABB_RECORRER_INORDEN){
        iterador_recorrido_inorden(arbol->nodo_raiz,(*funcion),extra,&cantidad_recorridos);
    }else if(recorrido == ABB_RECORRER_PREORDEN){
        iterador_recorrido_preorden(arbol->nodo_raiz,(*funcion),extra,&cantidad_recorridos);
    }else{
        iterador_recorrido_postorden(arbol->nodo_raiz,(*funcion),extra,&cantidad_recorridos);
    }

    return cantidad_recorridos;
} 

