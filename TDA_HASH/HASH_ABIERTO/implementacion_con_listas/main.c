#include "hash.h"

void imprimir_valor(void* valor){
	int* n = valor;
	printf("%d\n",*n);
}

int main(){
	
	hash_t* hash = hash_crear(15);

	int padrones[] = {3224,243,423,314345,1324,32231,2423,564,2,132,89};

	hash_insertar(hash,"Lucas",padrones+0);
	hash_insertar(hash,"Mauro",padrones+1);
	hash_insertar(hash,"Eloy",padrones+2);
	hash_insertar(hash,"Julian",padrones+3);
	hash_insertar(hash,"Gabriel",padrones+4);
	hash_insertar(hash,"Momo",padrones+5);
	hash_insertar(hash,"tete",padrones+6);
	hash_insertar(hash,"Koko",padrones+7);
	hash_insertar(hash,"Huhu",padrones+8);
	hash_insertar(hash,"ryuba",padrones+9);

	printf("El padron de Mauro es: %d\n",*(int*)hash_buscar(hash,"Mauro"));
	printf("La cantidad de elementos del hash es %d\n",hash->cantidad);

	printf("Busco un padron inexistente y devuelve:%p(deberia ser NULL)\n",hash_buscar(hash,"HOLAQUEONDA"));
	if(!hash_eliminar(hash,"JOJO")){
		printf("Elminar un padron inexistente devuelve False\n"); 
	}

	hash_imprimir(hash,imprimir_valor);

	hash_eliminar(hash,"Mauro");

	if(!hash_buscar(hash,"Mauro")){
		printf("Al eliminar a Mauro y buscarlo devuelve NULL\n");
		printf("Ahora la cantidad de elementos es %d\n",hash->cantidad);
	}

	hash_imprimir(hash,imprimir_valor);

	printf("Pruebo borrar el primero elemento a ver que onda\n");
	hash_eliminar(hash,"Lucas");

	hash_imprimir(hash,imprimir_valor);

	hash_destruir(hash);
	


	return 0;
}