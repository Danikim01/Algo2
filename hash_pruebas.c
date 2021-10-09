#include "hash.h"
#include <stdio.h>
#include <string.h>
#include "pa2mm.h"


void pruebas_creacion_hash_vacio(){

	hash_t* hash = hash_crear(NULL,0);

    pa2m_afirmar(hash,"Se pudo crear un hash vacio");
    pa2m_afirmar(hash_cantidad(hash) == 0,"Prueba hash la cantidad de elementos es 0");
    pa2m_afirmar(!hash_obtener(hash, "A"),"Prueba hash obtener clave A, es NULL, no existe");
    pa2m_afirmar(!hash_contiene(hash, "A"),"Prueba hash pertenece clave A, es false, no existe");
    pa2m_afirmar(hash_quitar(hash, "A") == -1,"Prueba hash borrar clave A, es NULL, no existe");

    hash_destruir(hash);
}

void prueba_hash_insertar()
{
    hash_t* hash = hash_crear(NULL,10);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    pa2m_afirmar(hash_insertar(hash, clave1, valor1) == 0,"Prueba hash insertar clave1");
    pa2m_afirmar(hash_cantidad(hash) == 1,"Prueba hash la cantidad de elementos es 1");

    pa2m_afirmar(hash_obtener(hash, clave1) == valor1,"Prueba hash obtener clave1 es valor1");
    pa2m_afirmar(hash_contiene(hash, clave1),"Prueba hash pertenece clave1, es true");

    pa2m_afirmar(hash_insertar(hash, clave2, valor2) == 0,"Prueba hash insertar clave2");
    pa2m_afirmar(hash_cantidad(hash) == 2,"Prueba hash la cantidad de elementos es 2");

    pa2m_afirmar(hash_obtener(hash, clave2) == valor2,"Prueba hash obtener clave2 es valor2");
    pa2m_afirmar(hash_contiene(hash, clave2),"Prueba hash pertenece clave2, es true");

    pa2m_afirmar(hash_insertar(hash, clave3, valor3) == 0,"Prueba hash insertar clave3");
    pa2m_afirmar(hash_cantidad(hash) == 3,"Prueba hash la cantidad de elementos es 3");
    pa2m_afirmar(hash_obtener(hash, clave3) == valor3,"Prueba hash obtener clave3 es valor3");
    pa2m_afirmar(hash_contiene(hash, clave3),"Prueba hash pertenece clave3, es true");

    hash_destruir(hash);
}


void prueba_hash_reemplazar()
{
    hash_t* hash = hash_crear(NULL,3);
    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";
    /* Inserta 2 valores y luego los reemplaza */
    pa2m_afirmar(hash_insertar(hash, clave1, valor1a) == 0,"Prueba hash insertar clave1");
    pa2m_afirmar(hash_obtener(hash, clave1) == valor1a,"Prueba hash obtener clave1 es valor1a");
    pa2m_afirmar(hash_obtener(hash, clave1) == valor1a,"Prueba hash obtener clave1 es valor1a");
    pa2m_afirmar(hash_insertar(hash, clave2, valor2a) == 0,"Prueba hash insertar clave2");
    pa2m_afirmar(hash_obtener(hash, clave2) == valor2a,"Prueba hash obtener clave2 es valor2a");
    pa2m_afirmar(hash_obtener(hash, clave2) == valor2a,"Prueba hash obtener clave2 es valor2a");
    pa2m_afirmar(hash_cantidad(hash) == 2,"Prueba hash la cantidad de elementos es 2");
    pa2m_afirmar(hash_insertar(hash, clave1, valor1b) == 0,"Prueba hash insertar clave1 con otro valor");
    pa2m_afirmar(hash_obtener(hash, clave1) == valor1b,"Prueba hash obtener clave1 es valor1b");
    pa2m_afirmar(hash_obtener(hash, clave1) == valor1b,"Prueba hash obtener clave1 es valor1b");
    pa2m_afirmar(hash_insertar(hash, clave2, valor2b) == 0,"Prueba hash insertar clave2 con otro valor");
    pa2m_afirmar(hash_obtener(hash, clave2) == valor2b,"Prueba hash obtener clave2 es valor2b");
    pa2m_afirmar(hash_obtener(hash, clave2) == valor2b,"Prueba hash obtener clave2 es valor2b");
    pa2m_afirmar(hash_cantidad(hash) == 2,"Prueba hash la cantidad de elementos es 2");
    hash_destruir(hash);
}
void prueba_hash_reemplazar_con_destruir()
{
    hash_t* hash = hash_crear(free,3);
    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;
    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));
    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    pa2m_afirmar(hash_insertar(hash, clave1, valor1a) == 0,"Prueba hash insertar clave1");
    pa2m_afirmar(hash_obtener(hash, clave1) == valor1a,"Prueba hash obtener clave1 es valor1a");
    pa2m_afirmar(hash_obtener(hash, clave1) == valor1a,"Prueba hash obtener clave1 es valor1a");
    pa2m_afirmar(hash_insertar(hash, clave2, valor2a) == 0,"Prueba hash insertar clave2");
    pa2m_afirmar(hash_obtener(hash, clave2) == valor2a,"Prueba hash obtener clave2 es valor2a");
    pa2m_afirmar(hash_obtener(hash, clave2) == valor2a,"Prueba hash obtener clave2 es valor2a");
    pa2m_afirmar(hash_cantidad(hash) == 2,"Prueba hash la cantidad de elementos es 2");
    pa2m_afirmar(hash_insertar(hash, clave1, valor1b) == 0,"Prueba hash insertar clave1 con otro valor");
    pa2m_afirmar(hash_obtener(hash, clave1) == valor1b,"Prueba hash obtener clave1 es valor1b");
    pa2m_afirmar(hash_obtener(hash, clave1) == valor1b,"Prueba hash obtener clave1 es valor1b");
    pa2m_afirmar(hash_insertar(hash, clave2, valor2b) == 0,"Prueba hash insertar clave2 con otro valor");
    pa2m_afirmar(hash_obtener(hash, clave2) == valor2b,"Prueba hash obtener clave2 es valor2b");
    pa2m_afirmar(hash_obtener(hash, clave2) == valor2b,"Prueba hash obtener clave2 es valor2b");
    pa2m_afirmar(hash_cantidad(hash) == 2,"Prueba hash la cantidad de elementos es 2");
    /* Se destruye el hash (se debe liberar lo que quedó dentro) */
    hash_destruir(hash);
}


void prueba_hash_quitar()
{
    hash_t* hash = hash_crear(NULL,3);
    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";
    /* Inserta 3 valores y luego los borra */
    pa2m_afirmar(hash_insertar(hash, clave1, valor1) == 0,"Prueba hash insertar clave1");
    pa2m_afirmar(hash_insertar(hash, clave2, valor2) == 0,"Prueba hash insertar clave2");
    pa2m_afirmar(hash_insertar(hash, clave3, valor3) == 0,"Prueba hash insertar clave3");
    /* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
    pa2m_afirmar(hash_contiene(hash, clave3),"Prueba hash pertenece clave3, es verdadero");
    pa2m_afirmar(hash_quitar(hash, clave3) == 0,"Prueba hash borrar clave3, se pudo");
    pa2m_afirmar(hash_quitar(hash, clave3) == -1,"Prueba hash borrar clave3, no se puede");
    pa2m_afirmar(!hash_contiene(hash, clave3),"Prueba hash pertenece clave3, es falso");
    pa2m_afirmar(!hash_obtener(hash, clave3),"Prueba hash obtener clave3, es NULL");
    pa2m_afirmar(hash_cantidad(hash) == 2,"Prueba hash la cantidad de elementos es 2");
    pa2m_afirmar(hash_contiene(hash, clave1),"Prueba hash pertenece clave1, es verdadero");
    pa2m_afirmar(hash_quitar(hash, clave1) == 0,"Prueba hash borrar clave1, se pudo");
    pa2m_afirmar(hash_quitar(hash, clave3) == -1,"Prueba hash borrar clave1, no se puede");
    pa2m_afirmar(!hash_contiene(hash, clave1),"Prueba hash pertenece clave1, es falso");
    pa2m_afirmar(!hash_obtener(hash, clave1),"Prueba hash obtener clave1, es NULL");
    pa2m_afirmar(hash_cantidad(hash) == 1,"Prueba hash la cantidad de elementos es 1");
    pa2m_afirmar(hash_contiene(hash, clave2),"Prueba hash pertenece clave2, es verdadero");
    pa2m_afirmar(hash_quitar(hash, clave2) == 0,"Prueba hash borrar clave2, se pudo");
    pa2m_afirmar(hash_quitar(hash, clave3) == -1,"Prueba hash borrar clave2, no se puede");
    pa2m_afirmar(!hash_contiene(hash, clave2),"Prueba hash pertenece clave2, es falso");
    pa2m_afirmar(!hash_obtener(hash, clave2),"Prueba hash obtener clave2, es NULL");
    pa2m_afirmar(hash_cantidad(hash) == 0,"Prueba hash la cantidad de elementos es 0");
    hash_destruir(hash);
}

void prueba_hash_clave_vacia()
{
    hash_t* hash = hash_crear(NULL,1);
    char *clave = "", *valor = "";
    pa2m_afirmar(hash_insertar(hash, clave, valor) == 0,"Prueba hash insertar clave vacia");
    pa2m_afirmar(hash_cantidad(hash) == 1,"Prueba hash la cantidad de elementos es 1");
    pa2m_afirmar(hash_obtener(hash, clave) == valor,"Prueba hash obtener clave vacia es valor");
    pa2m_afirmar(hash_contiene(hash, clave),"Prueba hash pertenece clave vacia, es true");
    pa2m_afirmar(hash_quitar(hash, clave) == 0,"Prueba hash borrar clave vacia, es valor");
    pa2m_afirmar(hash_cantidad(hash) == 0,"Prueba hash la cantidad de elementos es 0");
    hash_destruir(hash);
}
void prueba_hash_valor_null()
{
    hash_t* hash = hash_crear(NULL,1);
    char *clave = "", *valor = NULL;
    /* Inserta 1 valor y luego lo borra */
    pa2m_afirmar(hash_insertar(hash, clave, valor) == 0,"Prueba hash insertar clave vacia valor NULL");
    pa2m_afirmar(hash_cantidad(hash) == 1,"Prueba hash la cantidad de elementos es 1");
    pa2m_afirmar(hash_obtener(hash, clave) == valor,"Prueba hash obtener clave vacia es valor NULL");
    pa2m_afirmar(hash_contiene(hash, clave),"Prueba hash pertenece clave vacia, es true");
    pa2m_afirmar(hash_quitar(hash, clave) == 0,"Prueba hash borrar clave vacia, se puede");
    pa2m_afirmar(hash_cantidad(hash) == 0,"Prueba hash la cantidad de elementos es 0");
    hash_destruir(hash);
}

int buscar(const char* clave, char* claves[], size_t largo) 
{
	for (size_t i = 0; i < largo; i++) {
		if (strcmp(clave, claves[i]) == 0) return i;
	}
	return -1;
}

/*
void prueba_hash_iterar()
{
	hash_t* hash = hash_crear(NULL,3);
	
	char *claves[] = {"perro", "gato", "vaca"};
	char *valores[] = {"guau", "miau", "mu"};


	pa2m_afirmar(hash_insertar(hash, claves[0], valores[0]) == 0,"Prueba hash insertar clave1");
	pa2m_afirmar(hash_insertar(hash, claves[1], valores[1]) == 0,"Prueba hash insertar clave2");
	pa2m_afirmar(hash_insertar(hash, claves[2], valores[2]) == 0,"Prueba hash insertar clave3");


	hash_iter_t* iter = hash_iter_crear(hash);
	const char *clave;
	int indice;
	
	pa2m_afirmar(!hash_iter_al_final(iter),"Prueba hash iterador esta al final, es false");


	clave = hash_iter_ver_actual(iter);
	indice = buscar(clave, claves, sizeof(claves));
	pa2m_afirmar(indice != -1,"Prueba hash iterador ver actual, es una clave valida");
	pa2m_afirmar(clave != claves[indice],"Prueba hash iterador ver actual, no es el mismo puntero");
	pa2m_afirmar(hash_iter_avanzar(iter),"Prueba hash iterador avanzar es true");
	pa2m_afirmar(!hash_iter_al_final(iter),"Prueba hash iterador esta al final, es false");


	clave = hash_iter_ver_actual(iter);
	indice = buscar(clave, claves, sizeof(claves));
	pa2m_afirmar(indice != -1,"Prueba hash iterador ver actual, es una clave valida");
	pa2m_afirmar(clave != claves[indice],"Prueba hash iterador ver actual, no es el mismo puntero");
	pa2m_afirmar(hash_iter_avanzar(iter),"Prueba hash iterador avanzar es true");
	pa2m_afirmar(!hash_iter_al_final(iter),"Prueba hash iterador esta al final, es false");

	
	clave = hash_iter_ver_actual(iter);
	indice = buscar(clave, claves, sizeof(claves));
	pa2m_afirmar(indice != -1,"Prueba hash iterador ver actual, es una clave valida");
	pa2m_afirmar(clave != claves[indice],"Prueba hash iterador ver actual, no es el mismo puntero");
	
	hash_iter_avanzar(iter);
	pa2m_afirmar(hash_iter_al_final(iter),"Prueba hash iterador esta al final, es true");

	
	pa2m_afirmar(!hash_iter_ver_actual(iter),"Prueba hash iterador ver actual, es NULL");
	pa2m_afirmar(!hash_iter_avanzar(iter),"Prueba hash iterador avanzar es false");
	pa2m_afirmar(hash_iter_al_final(iter),"Prueba hash iterador esta al final, es true");

	hash_iter_destruir(iter);
	hash_destruir(hash);
	
}
*/

bool buscar_clave(hash_t* hash, const char* clave_buscada,void* extra)
{
	extra=extra;
	if (!hash|| !clave_buscada)
    	return true;

	bool recorrer = false;
	if(strcmp(extra,clave_buscada) == 0){
		recorrer = true;
	}
    

	return recorrer;
}


bool recorrer_hash(hash_t* hash,const char* clave_buscada, void* extra){
    extra=extra; 
    return false;
}


void pruebas_iterador_interno(){
	hash_t* hash = hash_crear(NULL,3);
	char *claves[] = {"perro", "gato", "vaca"};
	char *valores[] = {"guau", "miau", "mu"};

	pa2m_afirmar(hash_insertar(hash, claves[0], valores[0]) == 0,"Prueba hash insertar clave1");
	pa2m_afirmar(hash_insertar(hash, claves[1], valores[1]) == 0,"Prueba hash insertar clave2");
	pa2m_afirmar(hash_insertar(hash, claves[2], valores[2]) == 0,"Prueba hash insertar clave3");

	pa2m_afirmar(3 == hash_con_cada_clave(hash,recorrer_hash,NULL),"La cantidad de elementos recorridos corresponde a la cantidad de claves");
	pa2m_afirmar(3 == hash_con_cada_clave(hash,buscar_clave,"gato") && 2 == hash_con_cada_clave(hash,buscar_clave,"perro") && 1 == hash_con_cada_clave(hash,buscar_clave,"vaca"),"El iterador deja de iterar cuando corresponde");
	hash_destruir(hash);
}


void pruebas_eliminacion_2(){
    hash_t* hash = hash_crear(NULL,5);
    pa2m_afirmar(hash_quitar(hash,(char*)"hola")==-1,"No puedo borrar una clave inexistente");
    pa2m_afirmar(hash_insertar(hash,(char*)"1",(char*)"A") == 0,"Inserto un elemento");
    pa2m_afirmar(hash_insertar(hash,(char*)"1",(char*)"A") == 0,"Inserto otro elemento");
    pa2m_afirmar(hash_quitar(hash,(char*)"1")==0,"Borro elemento");
    pa2m_afirmar(hash_quitar(hash,(char*)"1")==-1,"No puedo borrar elemento eliminado");
    pa2m_afirmar(hash_cantidad(hash)==0,"El tamaño del hash es 0");
    pa2m_afirmar(hash_insertar(hash,(char*)"1",(char*)"B") == 0,"Inserto otro elemento con clave 1");
    pa2m_afirmar(hash_obtener(hash,(char*)"1")==(char*)"B","Obtengo clave 1 devuelve B");
    pa2m_afirmar(hash_insertar(hash,(char*)"4",(char*)"A") == 0,"Inserto otro elemento con clave 4");
    pa2m_afirmar(hash_quitar(hash,(char*)"1")==0,"Borro elemento con clave 1");
    pa2m_afirmar(hash_quitar(hash,(char*)"4")==0,"Borro elemento con clave 4");
    pa2m_afirmar(hash_cantidad(hash)==0,"El tamaño del hash es 0");
    hash_destruir(hash);
}
/*
bool son_iguales(hash_t* a, hash_t* b) {
    
    hash_iter_t* iter_a = hash_iter_crear(a);
    hash_iter_t* iter_b = hash_iter_crear(b);
    
    while(!hash_iter_al_final(iter_a) && !hash_iter_al_final(iter_b)){
        if(!hash_pertenece(b,hash_iter_ver_actual(iter_a)) || !hash_pertenece(a,hash_iter_ver_actual(iter_b)) ){
            return false;
        }
        void* elemento_a = hash_obtener(a,hash_iter_ver_actual(iter_a));
        void* elemento_a_con_b = hash_obtener(a,hash_iter_ver_actual(iter_b));
        void* elemento_b = hash_obtener(b,hash_iter_ver_actual(iter_b));
        void* elemento_b_con_a = hash_obtener(b,hash_iter_ver_actual(iter_a));
        if(elemento_a != elemento_a_con_b || elemento_b != elemento_b_con_a){
            return false;
        }
        hash_iter_avanzar(iter_a);
        hash_iter_avanzar(iter_b);
    }

    return true;

}
*/

bool son_iguales(hash_t* a,hash_t* b){
	
	hash_iter_t* iter_a = hash_iter_crear(a);
    hash_iter_t* iter_b = hash_iter_crear(b);
    
    while(!hash_iter_al_final(iter_a) && !hash_iter_al_final(iter_b)){
        if(!hash_contiene(b,hash_iter_ver_actual(iter_a)) || !hash_contiene(a,hash_iter_ver_actual(iter_b)) ){
        	hash_iter_destruir(iter_a);
        	hash_iter_destruir(iter_b);
            return false;
        }
        void* elemento_a = hash_obtener(a,hash_iter_ver_actual(iter_a));
        void* elemento_a_con_b = hash_obtener(a,hash_iter_ver_actual(iter_b));
        void* elemento_b = hash_obtener(b,hash_iter_ver_actual(iter_b));
        void* elemento_b_con_a = hash_obtener(b,hash_iter_ver_actual(iter_a));
        if(elemento_a != elemento_b_con_a || elemento_b != elemento_a_con_b){
        	hash_iter_destruir(iter_a);
        	hash_iter_destruir(iter_b);
            return false;
        }
        hash_iter_avanzar(iter_a);
        hash_iter_avanzar(iter_b);
    }

    hash_iter_destruir(iter_a);
    hash_iter_destruir(iter_b);

    int cant_a = 0,cant_b = 0;
    hash_iter_t* iter_a_aux = hash_iter_crear(a);
    hash_iter_t* iter_b_aux = hash_iter_crear(b);
    while(!hash_iter_al_final(iter_a_aux)){
    	cant_a += 1;
    	hash_iter_avanzar(iter_a_aux);
    }
    while(!hash_iter_al_final(iter_b_aux)){
    	cant_b += 1;
    	hash_iter_avanzar(iter_b_aux);
    }
    if(cant_a == cant_b){
    	hash_iter_destruir(iter_a_aux);
    	hash_iter_destruir(iter_b_aux);
    	return true;
    }
    hash_iter_destruir(iter_a_aux);
    hash_iter_destruir(iter_b_aux);
    return false;
}

void dado_dos_hashes_iguales_se_verifica_que_son_iguales(){
	hash_t* hash_a = hash_crear(NULL,3);
    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";
    /* Inserta 3 valores y luego los borra */
    pa2m_afirmar(hash_insertar(hash_a, clave2, valor2) == 0,"Prueba hash insertar clave2");
    pa2m_afirmar(hash_insertar(hash_a, clave1, valor1) == 0,"Prueba hash insertar clave1");
    pa2m_afirmar(hash_insertar(hash_a, clave3, valor3) == 0,"Prueba hash insertar clave3");
    hash_t* hash_b = hash_crear(NULL,3);
    char *clave1_b = "perro", *valor1_b = "guau";
    char *clave2_b = "gato", *valor2_b = "miau";
    char *clave3_b = "vaca", *valor3_b = "mu";
    //char *clave4_b = "puto",*valor4_b = "soyputo";
    //char *clave5_b = "trolo",*valor5_b = "soytrolo";
    /* Inserta 3 valores y luego los borra */
    pa2m_afirmar(hash_insertar(hash_b, clave1_b, valor1_b) == 0,"Prueba hash insertar clave1");
    pa2m_afirmar(hash_insertar(hash_b, clave2_b, valor2_b) == 0,"Prueba hash insertar clave2");
    pa2m_afirmar(hash_insertar(hash_b, clave3_b, valor3_b) == 0,"Prueba hash insertar clave3");
    //pa2m_afirmar(hash_insertar(hash_b, clave4_b, valor4_b) == 0,"Prueba hash insertar clave4");
    //pa2m_afirmar(hash_insertar(hash_b, clave5_b, valor5_b) == 0,"Prueba hash insertar clave5");

    pa2m_afirmar(son_iguales(hash_a,hash_b),"Los hash son iguales");

    hash_destruir(hash_a);
    hash_destruir(hash_b);
}

bool mas_de_mitad(void* arreglo[], size_t n) {
	if(n==1) return true;
 	hash_t* hash = hash_crear(NULL,10);
    int contador = 1;
    for (size_t i = 0; i < n;i++){
        char clave[20];
        int* numero = arreglo[i];
        sprintf(clave,"%d",(*numero));
        if(hash_contiene(hash,clave)){
            int valor_viejo = *(int*)hash_obtener(hash,clave);
            valor_viejo += 1;
            if (valor_viejo >= n/2){
            	hash_destruir(hash);
                return true;
            }
            hash_insertar(hash,clave,&valor_viejo);
        }else{
            hash_insertar(hash,clave,&contador);
        }
    }
    hash_destruir(hash);
    return false;
}

void dadounvectordeenteros_alversihayalgunnumerorepetidomasdelamitaddeveces_devuelvetrue(){
	int** vector = malloc(6*sizeof(int*));
	if(!vector) return;
	int uno = 1;
	int dos = 2;
	int tres = 3;
	vector[0] = &uno;
	vector[1] = &dos;
	vector[2] = &tres;
	vector[3] = &uno;
	vector[4] = &uno;
	vector[5] = &uno;
	pa2m_afirmar(mas_de_mitad((void**)vector,6)==true,"Pruebas mas de la mitad");
	free(vector);
}

int main(){
	/*
	pa2m_nuevo_grupo("Pruebas de Creacion");
	pruebas_creacion_hash_vacio();
	pa2m_nuevo_grupo("Pruebas de Insercion");
	prueba_hash_insertar();
	prueba_hash_reemplazar();
	prueba_hash_reemplazar_con_destruir();
	pa2m_nuevo_grupo("Pruebas de eliminacion");
	prueba_hash_quitar();
    pruebas_eliminacion_2();
	pa2m_nuevo_grupo("Pruebas con NULL o vacio");
	prueba_hash_clave_vacia();
	prueba_hash_valor_null();
	pa2m_nuevo_grupo("Pruebas insercion muchas");
	pa2m_nuevo_grupo("Pruebas iterador");
	pruebas_iterador_interno();
	*/
	//pa2m_nuevo_grupo("Pruebas verificar si dos hashes son iguales");
	//dado_dos_hashes_iguales_se_verifica_que_son_iguales();
	//pa2m_nuevo_grupo("Pruebas de repetidos mas de mitad veces");
	//dadounvectordeenteros_alversihayalgunnumerorepetidomasdelamitaddeveces_devuelvetrue();

 	return pa2m_mostrar_reporte();
    

}