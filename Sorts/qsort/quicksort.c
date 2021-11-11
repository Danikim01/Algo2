#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprimir(int* numeros,int tope){
	for(int i = 0; i < tope;i++){
		printf(" %i ",numeros[i]);
	}
	printf("\n");
}

void imprimir_char(char* buffer,int tope){
	for(int i = 0; i < tope;i++){
		printf(" %c ",buffer[i]);
	}
	printf("\n");
}

void swap(int* numeros,int a, int b){
	int aux = numeros[a];
	numeros[a] = numeros[b];
	numeros[b] = aux;
}

int particion(int* numeros,int tope){
	int pivote_actual = numeros[tope-1];
	int pivote_final = 0;
	for(int i = pivote_final; i < tope-1;i++){
		if(numeros[i] > pivote_actual){
			swap(numeros,i,pivote_final);
			pivote_final += 1;
		}
	}
	swap(numeros,pivote_final,tope-1);
	return pivote_final;
}

void quicksort(int* numeros, int tope){
	if(tope <= 1){
		return;
	}
	int pivote = particion(numeros,tope);
	quicksort(numeros,pivote);
	quicksort(numeros+pivote+1,tope-pivote-1);
}

void swap_generico(void* vector,int a, int b,int tamanio_un_elemento){
	char aux[tamanio_un_elemento];
	memcpy(aux,vector+(a*tamanio_un_elemento),tamanio_un_elemento);
	memcpy(vector+(a*tamanio_un_elemento),vector+(b*tamanio_un_elemento),tamanio_un_elemento);
	memcpy(vector+(b*tamanio_un_elemento),aux,tamanio_un_elemento);

}

int pivotear_generico(void* _vector,int cant_elementos,int tamanio_un_elemento,int (*comparador)(void*,void*)){
	int pivote_final = 0;
	int pos_pivote = cant_elementos-1;
	char* vector = _vector;
	for(int i = pivote_final; i < cant_elementos-1;i++){
		if(comparador(vector+i*tamanio_un_elemento,vector+pos_pivote*tamanio_un_elemento) <= 0){
			swap_generico(vector,i,pivote_final,tamanio_un_elemento);
			pivote_final += 1;
		}
	}
	swap_generico(vector,pos_pivote,pivote_final,tamanio_un_elemento);
	return pivote_final;
}

void qsort_generico(void *vector, int cant_elementos, int tamanio_un_elemento, int (*comparador)(void *,void*)){
	if(cant_elementos <= 1){
		return;
	}
	int pivote = pivotear_generico(vector,cant_elementos,tamanio_un_elemento,comparador);
	imprimir((int*)vector,cant_elementos);
	qsort_generico((char*)vector,pivote,tamanio_un_elemento,comparador);
	imprimir((int*)vector,cant_elementos);
	qsort_generico((char*)vector+((pivote+1)*tamanio_un_elemento),cant_elementos-pivote-1,tamanio_un_elemento,comparador);
	imprimir((int*)vector,cant_elementos);
}

int comparar_ints(void* i1,void* i2){
	int* _i1 = i1;
	int* _i2 = i2;
	return(*_i1-*_i2);
}

int main(){
	int vector[] = {2,124,23,5,89,-1,44,643,34};
	int tope = sizeof(vector)/sizeof(int);
	printf("Antes de ser ordenated\n");
	imprimir(vector,tope);
	//quicksort(vector,tope);
	//imprimir(vector,tope);
	qsort_generico(vector,tope,sizeof(int),comparar_ints);
	printf("Despues de ser ordenated\n");
	imprimir(vector,tope);

	return 0;
}