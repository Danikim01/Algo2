#include "hash.h"
#define FACTOR_DE_CARGA 0.75

hash_t* hash_crear(size_t capacidad_inicial){
	hash_t* hash = calloc(1,sizeof(hash_t));
	if(!hash) return NULL;
	hash->tamanio_tabla = capacidad_inicial;
	hash->tabla = calloc(capacidad_inicial,sizeof(par_t*));
	if(!hash->tabla){
		free(hash);
		return NULL;
	}
	return hash;
}

void eliminar_sub_listas_rec(par_t* lista){
	if(!lista){
		return;
	}
	par_t* sig = lista->siguiente;
	free((char*)lista->clave);
	free(lista);
	eliminar_sub_listas_rec(sig);
}
void hash_destruir(hash_t* hash){
	for(int i = 0; i < hash->tamanio_tabla;i++){
		eliminar_sub_listas_rec(hash->tabla[i]);
	}
	free(hash->tabla);
	free(hash);
}

size_t fhash(const char* k){
	size_t valor = 0;

	while(*k != '\0'){
		valor += *k;
		k++;
	}

	return valor;
}

char* strdup(const char* clave){
	char* buffer = calloc(1,(strlen(clave)+1)*sizeof(char));
	if(!buffer) return NULL;
	strcpy(buffer,clave);
	return buffer;
} 

par_t* crear_par(const char* clave,void* elemento){
	par_t* par = calloc(1,sizeof(par_t));
	if(!par) return NULL;
	par->clave = strdup(clave);
	par->valor = elemento;
	if(!par->clave){
		free(par);
		return NULL;
	}
	return par;
}

double calcular_factor_de_carga(hash_t* hash){
	return ((double)hash->cantidad/(double)hash->tamanio_tabla); 
}


void insertar(hash_t* hash,par_t* par){
	if(!par) return;
	hash_insertar(hash,par->clave,par->valor);
	insertar(hash,par->siguiente);
}


bool rehashear(hash_t* h){
	hash_t hash_aux = *h;
	h->tabla = calloc(h->tamanio_tabla*2,sizeof(par_t*));
	if(!h->tabla) return false;
	h->tamanio_tabla *= 2;
	printf("Despues del rehasheo el tamanio de la tabla es:%d\n",h->tamanio_tabla);
	h->cantidad = 0;

	for(int i = 0; i < hash_aux.tamanio_tabla;i++){
		insertar(h,hash_aux.tabla[i]);
	}
	

	for(int i = 0; i < hash_aux.tamanio_tabla;i++){
		eliminar_sub_listas_rec(hash_aux.tabla[i]);
	}
	free(hash_aux.tabla);
	return true;
}


bool hash_insertar(hash_t* hash,const char* clave,void* elemento){
	size_t hasheo = fhash(clave);
	size_t posicion = hasheo % hash->tamanio_tabla;

	if(calcular_factor_de_carga(hash)>=FACTOR_DE_CARGA){
		printf("REHASH PRIMER IF\n");
		printf("Hay que rehashear el factor de carga es %f\n",calcular_factor_de_carga(hash));
		rehashear(hash);
	}

	if(!hash->tabla[posicion]){
		par_t* par = crear_par(clave,elemento);
		hash->tabla[posicion] = par;
		hash->cantidad += 1;
		return true;
	}
	/*
	//En esta version inserto al principio
	par_t* actual = hash->tabla[posicion];
	while(actual && strcmp(actual->clave,clave) != 0){
		actual = actual->siguiente;
	}
	if(actual){
		actual->valor = elemento;
		return true;
	}
	par_t* par = crear_par(clave,elemento);
	if(!par){
		return NULL;
	}
	par->siguiente = hash->tabla[posicion];
	hash->tabla[posicion] = par;
	hash->cantidad += 1;
	return true;
	*/
	
	//En esta version inserto al final
	par_t* actual = hash->tabla[posicion];
	par_t* anterior = NULL;
	while(actual && strcmp(actual->clave,clave) != 0){
		anterior = actual;
		actual = actual->siguiente;
	}
	if(actual){
		actual->valor = elemento;
		return true;
	}
	par_t* par = crear_par(clave,elemento);
	anterior->siguiente = par;
	hash->cantidad += 1;

	if(calcular_factor_de_carga(hash)>=FACTOR_DE_CARGA){
		printf("REHASH SEGUNDO IF\n");
		printf("Hay que rehashear el factor de carga es %f\n",calcular_factor_de_carga(hash));
		rehashear(hash);
	}


	return true;
	
}

void* hash_buscar(hash_t* hash,const char* clave){
	size_t hasheo = fhash(clave);
	size_t posicion = hasheo % hash->tamanio_tabla;

	par_t* actual = hash->tabla[posicion];
	while(actual && strcmp(actual->clave,clave) != 0){
		actual = actual->siguiente;
	}
	if(actual){
		return actual->valor;
	}
	return NULL;
}

bool hash_eliminar(hash_t* hash,const char* clave){
	size_t hasheo = fhash(clave);
	size_t posicion = hasheo % hash->tamanio_tabla;

	par_t* actual = hash->tabla[posicion];
	par_t* anterior = NULL;
	while(actual && strcmp(actual->clave,clave) != 0){
		anterior = actual;
		actual = actual->siguiente;
	}

	if(!actual){
		return false;
	}

	if(anterior){
		anterior->siguiente = actual->siguiente;
		hash->cantidad -= 1;
		free((char*)actual->clave);
		free(actual);
		return true;
	}
	par_t* primero = hash->tabla[posicion];
	hash->tabla[posicion] = primero->siguiente;
	free((char*)primero->clave);
	free(primero);
	hash->cantidad -= 1;
	return true;

}

void imprimir_pares(par_t* pares,size_t posicion,void (*funcion)(void* elemento)){
	if(!pares){
		return;
	}
	printf("En la posicion %d en la tabla de hash,se encuentra la persona %s y cuyo valor es: ",posicion,pares->clave);
	funcion(pares->valor);
	imprimir_pares(pares->siguiente,posicion,funcion);
}


void hash_imprimir(hash_t* hash,void (*funcion)(void* elemento)){
	printf("Al imrpimir el tamanio de la tabla es %d\n",hash->tamanio_tabla);
	for(int i = 0; i < hash->tamanio_tabla;i++){
		if(hash->tabla[i]){
			imprimir_pares(hash->tabla[i],i,funcion);
		}
	}
}
