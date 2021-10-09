#include <stdlib.h>
#include "lista.h"
#include <stdio.h>


bool mostrar(void* elemento,void* extra){
	extra=extra;
	if(!elemento)
		return false;
	printf("%i",*(int*)elemento);
	return true;
}

void reverse_list(nodo_t** nodo){
	nodo_t* prev   = NULL;
    nodo_t* current = (*nodo);
    nodo_t* next;
    while (current != NULL)
    {
        next  = current->siguiente;  
        current->siguiente = prev;   
        prev = current;
        current = next;
    }
    (*nodo) = prev;
    return;
}

int main(){

	lista_t* lista = lista_crear();
	int elemento1 = 1,elemento2 = 2,elemento3 = 3;

	lista_insertar(lista,&elemento1);
	lista_insertar(lista,&elemento2);
	lista_insertar(lista,&elemento3);

	//lista_con_cada_elemento(lista,mostrar,NULL);

	reverse_list(&(lista->nodo_inicio));
	printf("\n");
	//lista_con_cada_elemento(lista,mostrar,NULL);
	printf("\n");
	lista_destruir(lista);
}