#include <stdio.h>
#include <stdlib.h>
#include "mostrar_abb.h"

typedef struct heap{
	//podria ser un void** vector
	int* vector;
	int tope;
}heap_t;

heap_t* heap_crear(){
	//Al crear le podriamos pasar un comparador para que el usuario lo ordene como quiera.
	heap_t* heap = calloc(1,sizeof(heap_t));
	heap->vector = calloc(1,100);
	return heap;
} 

heap_t* crear_heap_desde_vector(int* vector,int n){
	heap_t* heap = calloc(1,sizeof(heap_t));
	heap->vector = vector;
	heap->tope = n;
	return heap;
}

int posicion_padre(int n){
	return (n-1)/2;
}

void swap(int* vector,int n,int padre){
	int aux = vector[padre];
	vector[padre] = vector[n];
	vector[n] = aux;
}

void sift_up(heap_t* heap,int n){
	if(n == 0){
		return;
	}
	int padre = posicion_padre(n);

	if(heap->vector[padre] < heap->vector[n]){
		swap(heap->vector,n,padre);
		sift_up(heap,padre);
	}
}

int posicion_derecho(int n){
	return 2*n+2;
}

int posicion_izquierdo(int n){
	return 2*n+1;
}

void sift_down(heap_t* heap,int n){
	//mostrar_arbol(heap->vector,heap->tope);
	int hijo_der = posicion_derecho(n);
	int hijo_izq = posicion_izquierdo(n);

	if(heap->tope-1 < hijo_izq){//no tiene hijos
		return;
	}

	if(heap->tope-1 < hijo_der){//tiene 1 hijo izq
		if(heap->vector[n] < heap->vector[hijo_izq]){
			swap(heap->vector,n,hijo_izq);
			sift_down(heap,hijo_izq);
		}
	}

	if(heap->tope-1 >= hijo_der){//tiene dos hijos
		if (heap->vector[n] < heap->vector[hijo_izq] && heap->vector[hijo_izq] > heap->vector[hijo_der]){
			swap(heap->vector,n,hijo_izq);
			sift_down(heap,hijo_izq);
		}else if(heap->vector[n] < heap->vector[hijo_der] && heap->vector[hijo_der] > heap->vector[hijo_izq]){
			swap(heap->vector,n,hijo_der);
			sift_down(heap,hijo_der);
		}
	}
}

int heap_insertar(heap_t* heap,int elemento){
	if (!heap) return -1;
	
	heap->vector[heap->tope] = elemento;
	heap->tope += 1;

	sift_up(heap,heap->tope-1);
	return 0;
}

int heap_extraer_raiz(heap_t* heap){
	if(!heap){
		return -1;
	}
	int aux = heap->vector[0];
	//heap->vector[0] = heap->vector[heap->tope-1];
	swap(heap->vector,0,heap->tope-1);
	//heap->vector[heap->tope-1] = 0;
	heap->tope -= 1;

	sift_down(heap,0);

	return aux;

}

void heap_destruir(heap_t* heap){
	free(heap->vector);
	free(heap);
}

heap_t* heapify(int vector[],int n){
	heap_t* heap = crear_heap_desde_vector(vector,n);

	for(int i = posicion_padre(n); i>=0;i--){
		sift_down(heap,i);
	}
	return heap;
}

void heap_sort(int* vector,int n){
	heap_t* heap = heapify(vector,n);
	while(heap->tope > 0){
		heap_extraer_raiz(heap);
	}
	free(heap);
}


int main(){
	/*
	heap_t* heap = heap_crear();

	heap_insertar(heap,1);
	heap_insertar(heap,10);
	heap_insertar(heap,25);
	heap_insertar(heap,40);
	heap_insertar(heap,23);
	heap_insertar(heap,5);
	heap_insertar(heap,7);
	heap_insertar(heap,12);
	heap_insertar(heap,17);
	heap_insertar(heap,21);
	*/
	//mostrar_arbol(heap->vector,heap->tope);
	//heap_destruir(heap);

	heap_t* heap_2 = heap_crear();
	printf("\n");

	int vector[8];
	vector[0] = 6;
	vector[1] = 1;
	vector[2] = 3;
	vector[3] = 2;
	vector[4] = 9;
	vector[5] = 7;
	vector[6] = 15;
	vector[7] = 24;
	/*
	printf("Realizo Heapify\n");
	heapify(vector,sizeof(vector)/sizeof(int));

	for(int i = 0; i < sizeof(vector)/sizeof(int);i++){
		printf(" %i \n",vector[i]);
	}

	//Construyo heap
	for(int i = 0; i < sizeof(vector)/sizeof(int);i++){
		heap_insertar(heap_2,vector[i]);
	}

	mostrar_arbol(heap_2->vector,heap_2->tope);
	printf("\n");
	*/
	printf("Despues del Heapsort\n");
	heap_sort(vector,sizeof(vector)/sizeof(int));
	for(int i = 0; i < sizeof(vector)/sizeof(int);i++){
		printf(" %i \n",vector[i]);
	}

	heap_destruir(heap_2);

	//printf("La raiz es %d\n",heap_extraer_raiz(heap));
	//mostrar_arbol(heap->vector,heap->tope);
	
	return 0;
}