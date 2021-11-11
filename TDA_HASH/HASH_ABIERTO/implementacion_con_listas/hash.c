#include "hash.h"
#define FACTOR_DE_CARGA 0.75
#define ERROR -1

typedef struct par{
	const char* clave;
	void* elemento;
}par_t;

void destruir_par(par_t* par){
	free((char*)par->clave);
	free(par);
}

void hash_destruir(hash_t* hash){
	for(size_t i = 0; i < hash->tamanio_tabla;i++){
		lista_destruir(hash->tabla[i]);
	}
	free(hash->tabla);
	free(hash);
}

hash_t* hash_crear(size_t capacidad_inicial){

	if(capacidad_inicial <= 0) return NULL;
	hash_t* hash = calloc(1,sizeof(hash_t));
	if(!hash) return NULL;
	hash->tamanio_tabla = capacidad_inicial;
	hash->tabla = calloc(hash->tamanio_tabla,sizeof(lista_t*));
	bool hubo_error = false;
	if(!hash->tabla){
	  free(hash);
	  return NULL;
	}

	for(size_t i = 0; i < hash->tamanio_tabla;i++){
	  hash->tabla[i] = lista_crear(destruir_par);
	  if(!hash->tabla[i]){
	    hubo_error = true;
	  }
	}

	if(hubo_error){
	    hash_destruir(hash);
	    return NULL;
	}
	return hash;
}

size_t fhash(const char* k){
	size_t valor = 0;
	while(*k != '\0'){
		valor += *k++;
	}
	return valor;
}

char* strdup(const char* clave){
	char* buffer = calloc(1,(strlen(clave)+1)*sizeof(char));
	if(!buffer){
		return NULL;
	}
	strcpy(buffer,clave);
	return buffer;
}

par_t* crear_par(const char* clave,void* elemento){
	par_t* par = calloc(1,sizeof(par_t));
	if(!par){
		return NULL;
	}
	par->clave = strdup(clave);
	if(!par->clave){
		free(par);
		return NULL;
	}
	par->elemento = elemento;
	return par;
}

void insertar_rec_aux(hash_t* hash,nodo_t* nodo){
	if(!nodo){
		return;
	}
	hash_insertar(hash,((par_t*)nodo->elemento)->clave,((par_t*)nodo->elemento)->elemento);
	insertar_rec_aux(hash,nodo->siguiente);
}

void insertar_rec(hash_t* hash,lista_t* lista){
	if(!lista){
		return;
	}
	insertar_rec_aux(hash,lista->nodo_inicio);
}

bool inicializar_nueva_tabla(hash_t* h){
	h->tamanio_tabla *= 2;
	h->cantidad = 0;
	h->tabla = calloc(h->tamanio_tabla,sizeof(lista_t*));
	if(!h->tabla){
		return false;
	}
	bool hubo_error = false;
	for(size_t i = 0;i < h->tamanio_tabla;i++){
		h->tabla[i] = lista_crear(destruir_par);
		if(!h->tabla[i]){
			hubo_error = true;
		}
	}

	if(hubo_error){
		for(size_t i = 0; i < h->tamanio_tabla;i++){
			lista_destruir(h->tabla[i]);
		}
		free(h->tabla);
		return false;
	}
	return true;
}

bool rehasear(hash_t* h){
	hash_t hash_aux = *h;
	if(!inicializar_nueva_tabla(h)){
		return false;
	}
	printf("Despues del rehasheo el tamanio de la tabla es:%d\n",h->tamanio_tabla);
	for(int i = 0; i < hash_aux.tamanio_tabla;i++){
		insertar_rec(h,hash_aux.tabla[i]);
	}
	
	for(int i = 0; i < hash_aux.tamanio_tabla;i++){
		lista_destruir(hash_aux.tabla[i]);
	}
	free(hash_aux.tabla);
	return true;
}

double calcular_factor_de_carga(hash_t* hash){
	return ((double)hash->cantidad/(double)hash->tamanio_tabla); 
}

bool hash_insertar(hash_t* hash,const char* clave,void* elemento){
	if(!hash || !clave) return false;
	size_t vhash = fhash(clave);
	size_t posicion = vhash % hash->tamanio_tabla;

	if(calcular_factor_de_carga(hash) >= FACTOR_DE_CARGA){
		rehasear(hash);
	}

	par_t* par = crear_par(clave,elemento);
	if(lista_insertar(hash->tabla[posicion],par)==ERROR){
		destruir_par(par);
		return false;
	}
	hash->cantidad += 1;

	if(calcular_factor_de_carga(hash) >= FACTOR_DE_CARGA){
		rehasear(hash);
	}
	return true;
}

typedef struct ctx{
	void** elemento;
	int* pos_aux;
	const char* clave;
}ctx_t;

bool hallar_clave(void* elemento,void* extra){
	par_t* iterado = elemento;
	ctx_t* contexto = extra;
	if(strcmp(iterado->clave,contexto->clave) == 0){
		*(contexto->elemento) = iterado->elemento;
		return false;
	}
	return true;
}

bool hallar_posicion(void* elemento,void* extra){
	par_t* iterado = elemento;
	ctx_t* contexto = extra;
	if(strcmp(iterado->clave,contexto->clave) == 0){
		*(contexto->elemento) = iterado->elemento;
		return false;
	}
	(*contexto->pos_aux) += 1;
	return true;
}

void* hash_buscar(hash_t* hash,const char* clave){
	if(!hash) return NULL;
	size_t vhash = fhash(clave);
	size_t posicion = vhash % hash->tamanio_tabla;
	void* elemento_encontrado = NULL;
	ctx_t contexto = {&elemento_encontrado,0,clave};
	lista_con_cada_elemento(hash->tabla[posicion],hallar_clave,&contexto);
	return elemento_encontrado;
}

bool hash_eliminar(hash_t* hash,const char* clave){
	if(!hash || !clave) return NULL;
	size_t vhash = fhash(clave);
	size_t posicion = vhash % hash->tamanio_tabla;
	int pos_encontrada = 0;
	void* elemento_encontrado = NULL;
	ctx_t contexto = {&elemento_encontrado,&pos_encontrada,clave};
	lista_con_cada_elemento(hash->tabla[posicion],hallar_posicion,&contexto);
	if(elemento_encontrado){
		lista_borrar_de_posicion(hash->tabla[posicion],*(contexto.pos_aux));
		hash->cantidad -= 1;
		return true;
	}
	return false;
}

void imprimir_nodo_rec(nodo_t* nodo,int pos,void (*funcion)(void* elemento)){
	if(!nodo){
		return;
	}
	printf("El valor asociado a la clave %s en la posicion %d de la tabla de hash es: ",((par_t*)nodo->elemento)->clave,pos);
	funcion(((par_t*)nodo->elemento)->elemento);
	imprimir_nodo_rec(nodo->siguiente,pos,funcion);
}


void imprimir_por_lista(lista_t* lista,int pos,void (*funcion)(void* elemento)){
	if(!lista){
		return;
	}
	imprimir_nodo_rec(lista->nodo_inicio,pos,funcion);
}

void hash_imprimir(hash_t* hash,void (*funcion)(void* elemento)){
	if(!hash){
		return;
	}
	for(int i = 0; i < hash->tamanio_tabla;i++){
		imprimir_por_lista(hash->tabla[i],i,funcion);
	}
}