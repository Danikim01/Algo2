#include "abb.h"
#include <stdio.h>
#include "pa2mm.h"
#include "string.h"

#define MAX_CONTENIDO_COSA 10
#define ABB_RECORRER_INORDEN   0
#define ABB_RECORRER_PREORDEN  1
#define ABB_RECORRER_POSTORDEN 2	

typedef struct cosa{
    int clave;
    char contenido[MAX_CONTENIDO_COSA];
}cosa;

cosa* crear_cosa(int clave){
    cosa* c = (cosa*)malloc(sizeof(cosa));
    if(c)
        c->clave = clave;
    return c;
}

void destruir_cosa(cosa* c){
    free(c);
}

int comparar_cosas(void* elemento1, void* elemento2){
    if(!elemento1 || !elemento2)
        return 0;

    cosa* c1 = elemento1;
    cosa* c2 = elemento2;

    return c1->clave - c2->clave;
}

void destructor_de_cosas(void* elemento){
    if(!elemento)
        return;
    destruir_cosa((cosa*)elemento);
}


void cuandosecreaunarbol_secreaunarbolvacio(){
	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
	pa2m_afirmar(arbol != NULL,"Se pudo crear un arbol vacio");
	arbol_destruir(arbol);
}

void cuandosecreaunarbol_alcompararconunafuncioncomparadoranula_elarbolnosecrea(){
	abb_t* arbol = NULL;
	pa2m_afirmar(arbol_crear(NULL, destructor_de_cosas) == NULL,"Si la funcion comparadora es NULL no se crea el arbol");
	arbol_destruir(arbol);
}

void cuandosecreaunarbol_alcompararconunafunciondestructoranula_elarbolsecrea(){
	abb_t* arbol = arbol_crear(comparar_cosas,NULL);
	pa2m_afirmar(arbol != NULL,"Si la funcion destructora es NULL se crea el arbol");
	arbol_destruir(arbol);
}

void dadounarbolvacio_cuandoinsertounelemento_eseelementoterminasiendoraiz(){
	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
	int una_clave = 5;

	cosa* c1= crear_cosa(una_clave);

	pa2m_afirmar(arbol != NULL,"El arbol no es nulo");
	pa2m_afirmar(0 == arbol_insertar(arbol,c1),"La insercion se realizo correctamente");
	
	pa2m_afirmar(((cosa*)arbol_raiz(arbol))->clave == una_clave,"El nodo raiz tiene el elemento insertado");
	pa2m_afirmar(arbol->nodo_raiz->derecha == NULL && arbol->nodo_raiz->izquierda == NULL,"El arbol tiene un solo nodo");
	
	arbol_destruir(arbol);
}

void dadounarbolvacio_cuandoinsertounelementonulo_elarbolnoestavacio(){
	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	pa2m_afirmar(0 == arbol_insertar(arbol,NULL) && !arbol_vacio(arbol),"La insercion del elemento nulo realizo correctamente y el arbol no queda vacio");
	
	arbol_destruir(arbol);
}

void dadounarbolconulmenento_alinsertarunelementomayor_sealmacenacomohijoderecho(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
	int una_clave = 5;
	int clave_mayor = 7;

	cosa* c1 = crear_cosa(una_clave);
	cosa* c2 = crear_cosa(clave_mayor);

	arbol_insertar(arbol,c1);
	
	pa2m_afirmar(0 == arbol_insertar(arbol,c2),"La insercion del elemento mayor se realizo correctamente");
	
	pa2m_afirmar(arbol->nodo_raiz->derecha != NULL && arbol->nodo_raiz->izquierda == NULL,"El nodo raiz ahora tiene un hijo derecho");
	pa2m_afirmar(((cosa*)arbol->nodo_raiz->derecha->elemento)->clave == clave_mayor,"El nuevo elemento almacenado es el correcto");

	arbol_destruir(arbol);
}

void dadounarbolconulmenento_alinsertarunelementomenor_sealmacenacomohijoizquierdo(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
	int una_clave = 5;
	int clave_menor = 4;

	cosa* c1 = crear_cosa(una_clave);
	cosa* c2 = crear_cosa(clave_menor);

	arbol_insertar(arbol,c1);

	pa2m_afirmar(0 == arbol_insertar(arbol,c2),"La insercion del elemento menor se realizo correctamente");
	
	pa2m_afirmar(arbol->nodo_raiz->derecha == NULL && arbol->nodo_raiz->izquierda != NULL,"El nodo raiz ahora tiene un hijo izquierdo");
	pa2m_afirmar(((cosa*)arbol->nodo_raiz->izquierda->elemento)->clave == clave_menor,"El nuevo elemento almacenado es el correcto");
	
	arbol_destruir(arbol);
}

void dadounarbolconulmenento_alinsertarunelementoigual_sealmacenacomohijoizquierdo(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
	int una_clave = 5;
	int clave_igual = 5;

	cosa* c1 = crear_cosa(una_clave);
	cosa* c2 = crear_cosa(clave_igual);

	arbol_insertar(arbol,c1);

	pa2m_afirmar(0 == arbol_insertar(arbol,c2),"La insercion del elemento igual se realizo correctamente");
	
	pa2m_afirmar(arbol->nodo_raiz->derecha == NULL && arbol->nodo_raiz->izquierda != NULL,"El nodo raiz ahora tiene un hijo izquierdo");
	pa2m_afirmar(((cosa*)arbol->nodo_raiz->izquierda->elemento)->clave == clave_igual,"El nuevo elemento almacenado es el correcto");

	arbol_destruir(arbol);
}

void dadounarbolvacio_alrealizarcuatroinserciones_ladistribuciondelarbolescorrecta(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
	int una_clave = 5;
	int otra_clave = 3;
	int otra_clave_mas = 7;
	int otra_clave_mayor = 8;

	cosa* c1 = crear_cosa(una_clave);
	cosa* c2 = crear_cosa(otra_clave);
	cosa* c3 = crear_cosa(otra_clave_mas);
	cosa* c4 = crear_cosa(otra_clave_mayor);

	arbol_insertar(arbol,c1);
	arbol_insertar(arbol,c2);
	arbol_insertar(arbol,c3);
	arbol_insertar(arbol,c4);
	
	pa2m_afirmar(((cosa*)arbol_raiz(arbol))->clave == una_clave, "La raiz del arbol de cuatro elemento es la correcta");
	pa2m_afirmar(*(int*)arbol->nodo_raiz->derecha->elemento == otra_clave_mas && *(int*)arbol->nodo_raiz->izquierda->elemento == otra_clave && 
		*(int*)arbol->nodo_raiz->derecha->derecha->elemento == otra_clave_mayor, "Tras una insercion de cuatro elementos a un arbol vacio, la distribucion de la insercion es la correcta")

	arbol_destruir(arbol);
}

void dadounarbolcontreselemento_cuandobuscounelementodelarbol_encuentroeseelemento(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
	int una_clave = 5;
	int otra_clave = 3;
	int otra_clave_mas = 7;

	cosa* c1 = crear_cosa(una_clave);
	cosa* c2 = crear_cosa(otra_clave);
	cosa* c3 = crear_cosa(otra_clave_mas);

	arbol_insertar(arbol,c1);
	arbol_insertar(arbol,c2);
	arbol_insertar(arbol,c3);
	
	pa2m_afirmar(((cosa*)arbol_buscar(arbol, c2))->clave == otra_clave,"Se pudo encontrar el elemento buscado");
	
	arbol_destruir(arbol);
}

void dadounarbolcontreselementos_albuscarunelementoquenoestaenelarbol_lafunciondevuelveNULL(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
	int una_clave = 5;
	int otra_clave = 3;
	int otra_clave_mas = 7;
	int otra_clave_inexistente = 89;

	cosa* c1 = crear_cosa(una_clave);
	cosa* c2 = crear_cosa(otra_clave);
	cosa* c3 = crear_cosa(otra_clave_mas);
	cosa* c4 = crear_cosa(otra_clave_inexistente);

	arbol_insertar(arbol,c1);
	arbol_insertar(arbol,c2);
	arbol_insertar(arbol,c3);
	
	pa2m_afirmar((arbol_buscar(arbol, c4)) == NULL,"Si el elemento buscado es inexistente la busqueda sobre el elemento devuelve NULL");

	free(c4);
	arbol_destruir(arbol);
}

void dadounarbolvacio_albuscarsobreesearbol_lafunciondevuelveNULL(){
	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
	pa2m_afirmar((arbol_buscar(arbol, NULL)) == NULL,"Si el arbol sobre el cual se realiza la busqueda esta vacia, la busqueda sobre un elemento NULL devuelve NULL");
	arbol_destruir(arbol);
}

void dadounarbolnulo_albuscarsobreesearbol_lafunciondevuelveNULL(){
	abb_t* arbol = NULL;
	pa2m_afirmar((arbol_buscar(arbol, NULL)) == NULL,"Si el arbol sobre el cual se realiza la busqueda es nula, la busqueda sobre un elemento NULL devuelve NULL");
}
	
void pruebas_eliminaciones_multiples(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* c1= crear_cosa(1);
    cosa* c2= crear_cosa(2);
    cosa* c3= crear_cosa(3);
    cosa* c4= crear_cosa(4);
    cosa* c5= crear_cosa(5);
    cosa* c6= crear_cosa(6);
    cosa* c7= crear_cosa(7);
    cosa* auxiliar = crear_cosa(0);
    
    arbol_insertar(arbol, c4);
    arbol_insertar(arbol, c2);
    arbol_insertar(arbol, c6);
    arbol_insertar(arbol, c1);
    arbol_insertar(arbol, c3);
    arbol_insertar(arbol, c5);
    arbol_insertar(arbol, c7);


    pa2m_afirmar(((cosa*)arbol_raiz(arbol))->clave==4,"El nodo raiz deberia ser 4");

    auxiliar->clave = 7;
    pa2m_afirmar((arbol_borrar(arbol, auxiliar))==0,"Se borro nodo hoja cuyo elemento es 7");

    auxiliar->clave = 6;
    pa2m_afirmar((arbol_borrar(arbol, auxiliar))==0,"Se borro nodo con un hijo cuyo elemento es 6");	

    auxiliar->clave = 2;
    pa2m_afirmar((arbol_borrar(arbol, auxiliar))==0,"Se borro nodo con dos hijos cuyo elemento es 2");

    auxiliar->clave = 4;
    pa2m_afirmar((arbol_borrar(arbol, auxiliar))==0,"Se borro la raiz cuyo elemento es 4");

    auxiliar->clave = 1;
    pa2m_afirmar((arbol_borrar(arbol, auxiliar))==0,"Se borro nodo hoja cuyo elemento es 1");

    auxiliar->clave = 5;
    pa2m_afirmar((arbol_borrar(arbol, auxiliar))==0,"Se borro nodo hoja cuyo elemento es 5");

    auxiliar->clave = 3;
    pa2m_afirmar((arbol_borrar(arbol, auxiliar))==0 && arbol_vacio(arbol),"Se borro el ultimo elemento del arbol y el mismo queda vacio");

    free(auxiliar);
	arbol_destruir(arbol);

}



void dadounarbolcontreslementos_aleliminarunelementoinexistente_noseeliminaningunelementodelarbol(){
	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
	int una_clave = 5;
	int otra_clave = 3;
	int otra_clave_mas = 7;
	int otra_clave_inexistente = 890;

	cosa* c1 = crear_cosa(una_clave);
	cosa* c2 = crear_cosa(otra_clave);
	cosa* c3 = crear_cosa(otra_clave_mas);
	cosa* c4 = crear_cosa(otra_clave_inexistente);

	arbol_insertar(arbol,c1);
	arbol_insertar(arbol,c2);
	arbol_insertar(arbol,c3);
	
	pa2m_afirmar((arbol_borrar(arbol, c4)) == -1,"No se eliminan elementos inexistentes en el arbol");

	free(c4);
	arbol_destruir(arbol);
}

void dadounarbolconvacio_aleliminarunelementonulo_noseeliminaningunelementodelarbol(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	pa2m_afirmar((arbol_borrar(arbol, NULL)) == -1,"No se eliminan elementos sobre un arbol vacio");
	
	arbol_destruir(arbol);
}

void borrar_nodo_hoja(){

	abb_t* arbol = arbol_crear(comparar_cosas,destructor_de_cosas);
	
	cosa* c1= crear_cosa(1);
    cosa* c3= crear_cosa(3);
    cosa* c4= crear_cosa(4);
  	
	arbol_insertar(arbol, c3);
	arbol_insertar(arbol, c4);
	arbol_insertar(arbol, c1);

	pa2m_afirmar(arbol_raiz(arbol) == c3,"La raiz del arbol es 3");
	pa2m_afirmar(arbol_borrar(arbol, c1) == 0, "Puedo borrar un elemento sin hijos");
	pa2m_afirmar(arbol->nodo_raiz->izquierda == NULL, "La raiz ya no tiene hijo izquierda");
	pa2m_afirmar(arbol_borrar(arbol, c4) == 0, "Puedo borrar otro elemento sin hijos");
	pa2m_afirmar(arbol->nodo_raiz->derecha == NULL, "La raiz ya no tiene hijo derecho");

	arbol_destruir(arbol);
}

void borrar_nodo_raiz_con_hijo_izquierdo(){

	abb_t* arbol = arbol_crear(comparar_cosas,destructor_de_cosas);
	
	cosa* c1= crear_cosa(1);
    cosa* c3= crear_cosa(3);
  	
	arbol_insertar(arbol, c3);
	arbol_insertar(arbol, c1);

	pa2m_afirmar(arbol_raiz(arbol) == c3,"La raiz del arbol es 3");
	pa2m_afirmar(arbol_borrar(arbol, c1) == 0 && arbol->nodo_raiz->izquierda == NULL, "Puedo borrar al hijo izquierdo de la raiz");

	arbol_destruir(arbol);

}

void borrar_nodo_raiz_con_hijo_derecho(){

	abb_t* arbol = arbol_crear(comparar_cosas,destructor_de_cosas);
	
	cosa* c4= crear_cosa(4);
    cosa* c3= crear_cosa(3);
  	
	arbol_insertar(arbol, c3);
	arbol_insertar(arbol, c4);

	pa2m_afirmar(arbol_raiz(arbol) == c3,"La raiz del arbol es 3");
	pa2m_afirmar(arbol_borrar(arbol, c4) == 0 && arbol->nodo_raiz->derecha == NULL, "Puedo borrar al hijo derecho de la raiz");

	arbol_destruir(arbol);

}

void borrar_nodo_no_raiz_con_hijo_izquierdo(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

    cosa* c1= crear_cosa(1);
    cosa* c2= crear_cosa(2);
    cosa* c3= crear_cosa(3);
    cosa* c4= crear_cosa(4);
    cosa* c5= crear_cosa(5);
    cosa* c6= crear_cosa(6);
    cosa* c7= crear_cosa(7);
    cosa* c8= crear_cosa(8);
    cosa* c0= crear_cosa(0);
    
    int elemento_restante = 0;

    arbol_insertar(arbol, c4);
    arbol_insertar(arbol, c2);
    arbol_insertar(arbol, c6);
    arbol_insertar(arbol, c1);
    arbol_insertar(arbol, c3);
    arbol_insertar(arbol, c5);
    arbol_insertar(arbol, c7);
    arbol_insertar(arbol, c8);
    arbol_insertar(arbol, c0);

    pa2m_afirmar(arbol_buscar(arbol,c1) == c1 && arbol_borrar(arbol,c1) == 0,"El elemento a borrar esta en el arbol y se borra efectivamente");
    pa2m_afirmar(*(int*)arbol->nodo_raiz->izquierda->izquierda->elemento == elemento_restante,"El previo hijo izquierdo del nodo borrado reemplaza su lugar correctamente");

    arbol_destruir(arbol);
}


void borrar_nodo_no_raiz_con_hijo_derecho(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

    cosa* c1= crear_cosa(1);
    cosa* c2= crear_cosa(2);
    cosa* c3= crear_cosa(3);
    cosa* c4= crear_cosa(4);
    cosa* c5= crear_cosa(5);
    cosa* c6= crear_cosa(6);
    cosa* c7= crear_cosa(7);
    cosa* c8= crear_cosa(8);
    cosa* c0= crear_cosa(0);
    
    int elemento_restante = 8;

    arbol_insertar(arbol, c4);
    arbol_insertar(arbol, c2);
    arbol_insertar(arbol, c6);
    arbol_insertar(arbol, c1);
    arbol_insertar(arbol, c3);
    arbol_insertar(arbol, c5);
    arbol_insertar(arbol, c7);
    arbol_insertar(arbol, c8);
    arbol_insertar(arbol, c0);

    pa2m_afirmar(arbol_buscar(arbol,c7) == c7 && arbol_borrar(arbol,c7) == 0,"El otro elemento a borrar esta en el arbol y se borra efectivamente");
    pa2m_afirmar(*(int*)arbol->nodo_raiz->derecha->derecha->elemento == elemento_restante,"El previo hijo derecho del nodo borrado reemplaza su lugar correctamente");

    arbol_destruir(arbol);
}

void borrar_nodo_raiz_con_dos_hijos(){

	abb_t* arbol = arbol_crear(comparar_cosas,destructor_de_cosas);
	
	cosa* c1= crear_cosa(1);
    cosa* c3= crear_cosa(3);
    cosa* c4= crear_cosa(4);
  	
	arbol_insertar(arbol, c3);
	arbol_insertar(arbol, c4);
	arbol_insertar(arbol, c1);

	pa2m_afirmar(arbol_raiz(arbol) == c3 && arbol->nodo_raiz->izquierda != NULL && arbol->nodo_raiz->derecha != NULL,"La raiz del arbol es 3 y tiene dos hijos");
	pa2m_afirmar(arbol_borrar(arbol, c3) == 0, "Puedo borrar la raiz con dos hijos");
	pa2m_afirmar(arbol_raiz(arbol) == c1,"La raiz del arbol es 1");
	pa2m_afirmar(arbol->nodo_raiz->izquierda == NULL, "La raiz no tiene hijo izquierdo");
	pa2m_afirmar(arbol->nodo_raiz->derecha != NULL, "La raiz ya no tiene hijo derecho");

	arbol_destruir(arbol);
}

void borrar_nodo_no_raiz_con_dos_hijos(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

    cosa* c1= crear_cosa(1);
    cosa* c2= crear_cosa(2);
    cosa* c3= crear_cosa(3);
    cosa* c4= crear_cosa(4);
    cosa* c5= crear_cosa(5);
    cosa* c6= crear_cosa(6);
    cosa* c7= crear_cosa(7);
    cosa* c8= crear_cosa(8);
    cosa* c0= crear_cosa(0);
    
    arbol_insertar(arbol, c4);
    arbol_insertar(arbol, c2);
    arbol_insertar(arbol, c6);
    arbol_insertar(arbol, c1);
    arbol_insertar(arbol, c3);
    arbol_insertar(arbol, c5);
    arbol_insertar(arbol, c7);
    arbol_insertar(arbol, c8);
    arbol_insertar(arbol, c0);

    pa2m_afirmar(arbol_borrar(arbol,c6) == 0,"Se borra efectivamente el nodo no raiz con dos hijos y cuya clave es 6");
    pa2m_afirmar(*(int*)arbol->nodo_raiz->derecha->elemento == 5 && *(int*)arbol->nodo_raiz->derecha->derecha->elemento == 7 && *(int*)arbol->nodo_raiz->derecha->derecha->derecha->elemento == 8,"El predecesor inorden del nodo 6,el 5, reemplaza su lugar y sus respectivos hijos son los correctos");
    pa2m_afirmar(arbol_borrar(arbol,c2) == 0,"Se borra efectivamente el nodo no raiz con dos hijos y cuya clave es 2");
    pa2m_afirmar(*(int*)arbol->nodo_raiz->izquierda->elemento == 1 && *(int*)arbol->nodo_raiz->izquierda->izquierda->elemento == 0 && *(int*)arbol->nodo_raiz->izquierda->derecha->elemento == 3,"El predecesor inorden del nodo 2,el 1, reemplaza su lugar y sus hijos son los correctos");

    arbol_destruir(arbol);
}

void borrar_nodo_raiz_con_muchos_subarboles(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

    cosa* c1= crear_cosa(1);
    cosa* c2= crear_cosa(2);
    cosa* c3= crear_cosa(3);
    cosa* c4= crear_cosa(4);
    cosa* c5= crear_cosa(5);
    cosa* c6= crear_cosa(6);
    cosa* c7= crear_cosa(7);
    cosa* c8= crear_cosa(8);
    cosa* c0= crear_cosa(0);
    
    arbol_insertar(arbol, c4);
    arbol_insertar(arbol, c2);
    arbol_insertar(arbol, c6);
    arbol_insertar(arbol, c1);
    arbol_insertar(arbol, c3);
    arbol_insertar(arbol, c5);
    arbol_insertar(arbol, c7);
    arbol_insertar(arbol, c8);
    arbol_insertar(arbol, c0);

    pa2m_afirmar(arbol_raiz(arbol) == c4,"El nodo raiz del arbol es el 4");
    pa2m_afirmar(arbol_borrar(arbol,c4) == 0,"Se borra efectivamente el nodo raiz con dos hijos y muchos subarboles por cada lado");
    pa2m_afirmar(arbol_raiz(arbol) == c3,"La nueva raiz del arbol es el precedesor inorden del nodo raiz anterior que es el 3");
    pa2m_afirmar(*(int*)arbol->nodo_raiz->izquierda->elemento == 2 && *(int*)arbol->nodo_raiz->izquierda->izquierda->elemento == 1 && *(int*)arbol->nodo_raiz->izquierda->izquierda->izquierda->elemento == 0,"Los hijos del subarbol izquierdo del nuevo nodo raiz son los correctos");

    arbol_destruir(arbol);
}

bool comparador_enteros(size_t elemento1,size_t elemento2){
	return (elemento1==elemento2);
}

void dadounarbolvacio_alrecorrerinordenelarbol_noserecorreningunelemento(){

	int vector[2];
    size_t tamanio=2;

    abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	pa2m_afirmar(comparador_enteros(arbol_recorrido_inorden(arbol,(void**)vector,tamanio),0),"No se recorre sobre un arbol vacio");

	arbol_destruir(arbol);
}

void dadounarboldesieteelementos_alrecorrerelarbolinordeneinsertanelementosobreunvectordetopemenoralacantidaddeelementosdelarbol_noserecorrentodosloslementosdelarbol(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[5];

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));

    size_t cantidad_recorridos = arbol_recorrido_inorden(arbol, (void**)elementos, 5);

    pa2m_afirmar(cantidad_recorridos == 5,"Se recorren sobre los elementos del arbol segun un tamanio del array menor a la cantidad de elementos");

    arbol_destruir(arbol);
}

void inicializar_vector(int array[],int tope){
	for(int i = 0; i < tope;i++){
		array[i] = 0;
	}
}

void dadounarboldesieteelementos_alrecorrerelarbolinordeneinsertanelementosobreunvectordetopemenoralacantidaddelementosdelarbol_sealmacenan5elementosenelvectorenordenascendente(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[5];
	inicializar_vector((int*)elementos,5);

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));
   	
   	arbol_recorrido_inorden(arbol, (void**)elementos, 5);

	pa2m_afirmar(elementos[0]->clave == 1 && elementos[1]->clave == 2 && elementos[2]->clave == 3 && elementos[3]->clave == 4 && elementos[4]->clave == 5,"El vector almacena correctamente los elementos tras recorrido inorden segun un tamanio del array menor a la cantidad de elementos");
    
    arbol_destruir(arbol);
}

void dadounarboldesieteelementos_alrecorrerelarbolinordeneinsertanelementosobreunvectordetopeigualalacantidaddeelementosdelarbol_serecorrentodosloslementosdelarbol(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[7];

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));

    size_t cantidad_recorridos = arbol_recorrido_inorden(arbol, (void**)elementos, 7);

    pa2m_afirmar(cantidad_recorridos == 7,"Se recorren sobre los elementos del arbol segun un tamanio del array igual a la cantidad de elementos");

    arbol_destruir(arbol);
}

void dadounarboldesieteelementos_alrecorrerelarbolinordeneinsertanelementosobreunvectordetopeigualalacantidaddelementosdelarbol_sealmacenan7elementosenelvectorenordenascendente(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[7];
	inicializar_vector((int*)elementos,7);

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));
   	
   	arbol_recorrido_inorden(arbol, (void**)elementos, 7);

	pa2m_afirmar(elementos[0]->clave == 1 && elementos[1]->clave == 2 && elementos[2]->clave == 3 && elementos[3]->clave == 4 && elementos[4]->clave == 5 && elementos[5]->clave == 6 && elementos[6]->clave == 7,"El vector almacena correctamente los elementos tras recorrido inorden segun un tamanio del array igual a la cantidad de elementos");
    
    arbol_destruir(arbol);
}

void dadounarboldesieteelementos_alrecorrerelarbolinordeneinsertanelementosobreunvectordetopemayoralacantidaddeelementosdelarbol_serecorrentodosloslementosdelarbol(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[10];

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));

    size_t cantidad_recorridos = arbol_recorrido_inorden(arbol, (void**)elementos, 10);

    pa2m_afirmar(cantidad_recorridos == 7,"Se recorren sobre los elementos del arbol segun un tamanio del array mayor a la cantidad de elementos");

    arbol_destruir(arbol);
}

void dadounarboldesieteelementos_alrecorrerelarbolinordeneinsertanelementosobreunvectordetopemayoralacantidaddelementosdelarbol_sealmacenan7elementosenelvectorenordenascendente(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[10];
	inicializar_vector((int*)elementos,10);

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));
   	
   	arbol_recorrido_inorden(arbol, (void**)elementos, 10);

	pa2m_afirmar(elementos[0]->clave == 1 && elementos[1]->clave == 2 && elementos[2]->clave == 3 && elementos[3]->clave == 4 && elementos[4]->clave == 5 && elementos[5]->clave == 6 && elementos[6]->clave == 7,"El vector almacena correctamente los elementos tras recorrido inorden segun un tamanio del array mayor a la cantidad de elementos");
    
    arbol_destruir(arbol);
}

void dadounarbolvacio_alrecorrerpreordenelarbol_noserecorreningunelemento(){

	int vector[2];
    size_t tamanio=2;

    abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	pa2m_afirmar(comparador_enteros(arbol_recorrido_preorden(arbol,(void**)vector,tamanio),0),"No se recorre sobre un arbol vacio");

	arbol_destruir(arbol);
}


void dadounarboldesieteelementos_alrecorrerelarbolpreordeneinsertanelementosobreunvectordetopemenoralacantidaddeelementosdelarbol_noserecorrentodosloslementosdelarbol(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[5];

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));

    size_t cantidad_recorridos = arbol_recorrido_preorden(arbol, (void**)elementos, 5);

    pa2m_afirmar(cantidad_recorridos == 5,"Se recorren sobre los elementos del arbol segun un tamanio del array menor a la cantidad de elementos");

    arbol_destruir(arbol);
}

void dadounarboldesieteelementos_alrecorrerelarbolpreordeneinsertanelementosobreunvectordetopemenoralacantidaddelementosdelarbol_sealmacenan5elementosenelvectorenelordencorrecto(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[5];
	inicializar_vector((int*)elementos,5);

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));
   	
	arbol_recorrido_preorden(arbol, (void**)elementos, 5);

	pa2m_afirmar(elementos[0]->clave == 4 && elementos[1]->clave == 2 && elementos[2]->clave == 1 && elementos[3]->clave == 3 && elementos[4]->clave == 6,"El vector almacena correctamente los elementos tras recorrido preorden segun un tamanio del array menor a la cantidad de elementos");
    
    arbol_destruir(arbol);
}

void dadounarboldesieteelementos_alrecorrerelarbolpreordeneinsertanelementosobreunvectordetopeigualalacantidaddeelementosdelarbol_serecorrentodosloslementosdelarbol(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[7];

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));

    size_t cantidad_recorridos = arbol_recorrido_preorden(arbol, (void**)elementos, 7);

    pa2m_afirmar(cantidad_recorridos == 7,"Se recorren sobre los elementos del arbol segun un tamanio del array igual a la cantidad de elementos");

    arbol_destruir(arbol);
}


void dadounarboldesieteelementos_alrecorrerelarbolpreordeneinsertanelementosobreunvectordetopeigualalacantidaddelementosdelarbol_sealmacenan7elementosenelvectorenelordencorrecto(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[7];
	inicializar_vector((int*)elementos,7);

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));
   	
	arbol_recorrido_preorden(arbol, (void**)elementos, 7);

	pa2m_afirmar(elementos[0]->clave == 4 && elementos[1]->clave == 2 && elementos[2]->clave == 1 && elementos[3]->clave == 3 && elementos[4]->clave == 6 && elementos[5]->clave == 5 && elementos[6]->clave == 7,"El vector almacena correctamente los elementos tras recorrido preorden segun un tamanio del array igual a la cantidad de elementos");
    
    arbol_destruir(arbol);
}

void dadounarboldesieteelementos_alrecorrerelarbolpreordeneinsertanelementosobreunvectordetopemayoralacantidaddeelementosdelarbol_serecorrentodosloslementosdelarbol(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[10];

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));

    size_t cantidad_recorridos = arbol_recorrido_preorden(arbol, (void**)elementos, 10);

    pa2m_afirmar(cantidad_recorridos == 7,"Se recorren sobre los elementos del arbol segun un tamanio del array mayor a la cantidad de elementos");

    arbol_destruir(arbol);
}

void dadounarboldesieteelementos_alrecorrerelarbolpreordeneinsertanelementosobreunvectordetopemayoralacantidaddelementosdelarbol_sealmacenan7elementosenelvectorenelordencorrecto(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[10];
	inicializar_vector((int*)elementos,10);

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));
   	
	arbol_recorrido_preorden(arbol, (void**)elementos, 10);

	pa2m_afirmar(elementos[0]->clave == 4 && elementos[1]->clave == 2 && elementos[2]->clave == 1 && elementos[3]->clave == 3 && elementos[4]->clave == 6 && elementos[5]->clave == 5 && elementos[6]->clave == 7,"El vector almacena correctamente los elementos tras recorrido preorden segun un tamanio del array mayor a la cantidad de elementos");
    
    arbol_destruir(arbol);
}

void dadounarbolvacio_alrecorrerpostordenelarbol_noserecorreningunelemento(){
	int vector[2];
    size_t tamanio=2;

    abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	pa2m_afirmar(comparador_enteros(arbol_recorrido_postorden(arbol,(void**)vector,tamanio),0),"No se recorre sobre un arbol vacio");

	arbol_destruir(arbol);
}

void dadounarboldesieteelementos_alrecorrerelarbolpostordeneinsertanelementosobreunvectordetopemenoralacantidaddeelementosdelarbol_noserecorrentodosloslementosdelarbol(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[5];
	inicializar_vector((int*)elementos,5);

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));
   	
	size_t cantidad = arbol_recorrido_postorden(arbol, (void**)elementos, 5);

	pa2m_afirmar(cantidad == 5,"Se recorren sobre los elementos del arbol segun un tamanio del array menor a la cantidad de elementos");
    
    arbol_destruir(arbol);

}

void dadounarboldesieteelementos_alrecorrerelarbolpostordeneinsertanelementosobreunvectordetopemenoralacantidaddelementosdelarbol_sealmacenan5elementosenelvectorenelordencorrecto(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[5];
	inicializar_vector((int*)elementos,5);

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));
   	
   	arbol_recorrido_postorden(arbol, (void**)elementos, 5);

	pa2m_afirmar(elementos[0]->clave == 1 && elementos[1]->clave == 3 && elementos[2]->clave == 2 && elementos[3]->clave == 5 && elementos[4]->clave == 7,"El vector almacena correctamente los elementos tras recorrido postorden segun un tamanio del array menor a la cantidad de elementos");
    
    arbol_destruir(arbol);
}

void dadounarboldesieteelementos_alrecorrerelarbolpostordeneinsertanelementosobreunvectordetopeigualalacantidaddeelementosdelarbol_serecorrentodosloslementosdelarbol(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[7];
	inicializar_vector((int*)elementos,7);

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));
   	
	size_t cantidad = arbol_recorrido_postorden(arbol, (void**)elementos, 7);

	pa2m_afirmar(cantidad == 7,"Se recorren sobre los elementos del arbol segun un tamanio del array igual a la cantidad de elementos");
    
    arbol_destruir(arbol);

}

void dadounarboldesieteelementos_alrecorrerelarbolpostordeneinsertanelementosobreunvectordetopeigualalacantidaddelementosdelarbol_sealmacenan7elementosenelvectorenelordencorrecto(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[7];
	inicializar_vector((int*)elementos,7);

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));
   	
   	arbol_recorrido_postorden(arbol, (void**)elementos, 7);

	pa2m_afirmar(elementos[0]->clave == 1 && elementos[1]->clave == 3 && elementos[2]->clave == 2 && elementos[3]->clave == 5 && elementos[4]->clave == 7 && elementos[5]->clave == 6 && elementos[6]->clave == 4,"El vector almacena correctamente los elementos tras recorrido postorden segun un tamanio del array igual a la cantidad de elementos");
    
    arbol_destruir(arbol);
}

void dadounarboldesieteelementos_alrecorrerelarbolpostordeneinsertanelementosobreunvectordetopemayoralacantidaddeelementosdelarbol_serecorrentodosloslementosdelarbol(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[10];
	inicializar_vector((int*)elementos,10);

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));
   	
	size_t cantidad = arbol_recorrido_postorden(arbol, (void**)elementos, 10);

	pa2m_afirmar(cantidad == 7,"Se recorren sobre los elementos del arbol segun un tamanio del array mayor a la cantidad de elementos");
    
    arbol_destruir(arbol);
}

void dadounarboldesieteelementos_alrecorrerelarbolpostordeneinsertanelementosobreunvectordetopemayoralacantidaddelementosdelarbol_sealmacenan7elementosenelvectorenelordencorrecto(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	cosa* elementos[10];
	inicializar_vector((int*)elementos,10);

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(7));
   	
   	arbol_recorrido_postorden(arbol, (void**)elementos,10);

	pa2m_afirmar(elementos[0]->clave == 1 && elementos[1]->clave == 3 && elementos[2]->clave == 2 && elementos[3]->clave == 5 && elementos[4]->clave == 7 && elementos[5]->clave == 6 && elementos[6]->clave == 4,"El vector almacena correctamente los elementos tras recorrido postorden segun un tamanio del array mayor a la cantidad de elementos");
    
    arbol_destruir(arbol);
}

/*
Devuelve false en caso de error (en caso de que tanto elemento a recorrer del arbol como el elemento buscado sean inexistentes )
y tambien devuele false en caso de que elemento coincida con el elemento_buscado. Devuelve true mientras no pase lo anterior.
*/
bool recorrer_hasta_elemento_buscado(void* elemento, void* elemento_buscado){
	
	if (!elemento_buscado)
    	return true;

    if(elemento){
        if(((cosa*)elemento)->clave == *(int*)elemento_buscado){
            return true;
        }
    }
    return false;
}

bool recorrer_arbol(void* elemento, void* extra){
    extra=extra; 
    return false;
}

void dadounarbolvacio_alrecorrerelarbolaplicandoeliteradorinterno_noserecorreningunelementodelarbol(){
	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

   	pa2m_afirmar(abb_con_cada_elemento(arbol,ABB_RECORRER_INORDEN,recorrer_arbol,NULL) == 0 && abb_con_cada_elemento(arbol,ABB_RECORRER_PREORDEN,recorrer_arbol,NULL) == 0
   		&& abb_con_cada_elemento(arbol,ABB_RECORRER_POSTORDEN,recorrer_arbol,NULL) == 0,"Si el arbol esta vacio y uso el iterador interno, no se recorre sobre ningun elemento");
 
    arbol_destruir(arbol);
}

void dadounarbolcon3elementos_alrecorrerelarbolaplicandoeliteradorinternosininvocaraunafuncion_noserecorreningunelementodelarbol(){
	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));

   	pa2m_afirmar(abb_con_cada_elemento(arbol,ABB_RECORRER_INORDEN,NULL,NULL) == 0 && abb_con_cada_elemento(arbol,ABB_RECORRER_PREORDEN,NULL,NULL) == 0
   		&& abb_con_cada_elemento(arbol,ABB_RECORRER_POSTORDEN,NULL,NULL) == 0,"Si uso el iterador interno en un arbol de tres elementos sin haber invocado a una funcion, no se recorre sobre ningun elemento del arbol");
 
    arbol_destruir(arbol);
}


void dadounarbolcon3elementos_alrecorrerelarbolaplicandoeliteradorinterno_lacantidaddeleementosrecorridosescorrecta(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

    arbol_insertar(arbol, crear_cosa(4));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));

   	pa2m_afirmar(abb_con_cada_elemento(arbol,ABB_RECORRER_INORDEN,recorrer_arbol,NULL) == 3,"Usando el iterador interno por medio de un recorrido inorden se recorren correctamente sobre los 3 elementos del arbol");
    pa2m_afirmar(abb_con_cada_elemento(arbol,ABB_RECORRER_PREORDEN,recorrer_arbol,NULL) == 3,"Usando el iterador interno por medio de un recorrido preorden se recorren correctamente sobre los 3 elementos del arbol");
    pa2m_afirmar(abb_con_cada_elemento(arbol,ABB_RECORRER_POSTORDEN,recorrer_arbol,NULL) == 3,"Usando el iterador interno por medio de un recorrido postorden se recorren correctamente sobre los 3 elementos del arbol");

    arbol_destruir(arbol);

}

void dadounarbolcon3elementos_alrecorrerelarbolaplicandoeliteradorinterno_eliteradordejadeiterarcuandocorresponde(){

	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	int elemento_a_buscar = 5;
	int otro_elemento_a_buscar = 6;
	int elemento_restante_a_buscar = 2;

    pa2m_afirmar(arbol_insertar(arbol, crear_cosa(5)) == 0 && arbol_insertar(arbol, crear_cosa(2)) == 0 && arbol_insertar(arbol, crear_cosa(6)) == 0,"Se insertan sucesivamente al arbol los elementos 5,2 y 6");

   	pa2m_afirmar(2 == abb_con_cada_elemento(arbol,ABB_RECORRER_INORDEN,recorrer_hasta_elemento_buscado,&elemento_a_buscar),"Por medio de un recorrido inorden, se recorren 2 elementos al encontrar el 5");
    pa2m_afirmar(1 == abb_con_cada_elemento(arbol,ABB_RECORRER_PREORDEN,recorrer_hasta_elemento_buscado,&elemento_a_buscar),"Por medio de un recorrido preorden, se recorren 2 elementos al encontrar el 5");
    pa2m_afirmar(3 == abb_con_cada_elemento(arbol,ABB_RECORRER_POSTORDEN,recorrer_hasta_elemento_buscado,&elemento_a_buscar),"Por medio de un recorrido postorden, se recorren 2 elementos al encontrar el 5");
  	
  	pa2m_afirmar(3 == abb_con_cada_elemento(arbol,ABB_RECORRER_INORDEN,recorrer_hasta_elemento_buscado,&otro_elemento_a_buscar),"Por medio de un recorrido inorden, se recorren 3 elementos al encontrar el 6");
  	pa2m_afirmar(3 == abb_con_cada_elemento(arbol,ABB_RECORRER_PREORDEN,recorrer_hasta_elemento_buscado,&otro_elemento_a_buscar),"Por medio de un recorrido preorden, se recorren 3 elementos al encontrar el 6");
  	pa2m_afirmar(2 == abb_con_cada_elemento(arbol,ABB_RECORRER_POSTORDEN,recorrer_hasta_elemento_buscado,&otro_elemento_a_buscar),"Por medio de un recorrido postorden, se recorren 2 elementos al encontrar el 6");
    
  	pa2m_afirmar(1 == abb_con_cada_elemento(arbol,ABB_RECORRER_INORDEN,recorrer_hasta_elemento_buscado,&elemento_restante_a_buscar),"Por medio de un recorrido inorden, se recorren 1 elementos al encontrar el 2");
  	pa2m_afirmar(2 == abb_con_cada_elemento(arbol,ABB_RECORRER_PREORDEN,recorrer_hasta_elemento_buscado,&elemento_restante_a_buscar),"Por medio de un recorrido preorden, se recorren 2 elementos al encontrar el 2");
  	pa2m_afirmar(1 == abb_con_cada_elemento(arbol,ABB_RECORRER_POSTORDEN,recorrer_hasta_elemento_buscado,&elemento_restante_a_buscar),"Por medio de un recorrido postorden, se recorren 1 elementos al encontrar el 2");


    arbol_destruir(arbol);
}

void dadounarbolconunelementoNULL_albuscardichoelemento_elelementoseencuentraenelarbol(){
	abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

	arbol_insertar(arbol,NULL);
	pa2m_afirmar(arbol_buscar(arbol,NULL) == NULL,"La busqueda del unico elemento nulo se realizo correctamente");
	
	arbol_destruir(arbol);
}

void dadounarbolNULL_alquerersabersiestavacio_lafuncionarbolvaciodevuelvetrue(){
	pa2m_afirmar(arbol_vacio(NULL),"Un arbol NULL es vacio");
}


int main(){

	pa2m_nuevo_grupo("Pruebas de creacion del arbol");
	cuandosecreaunarbol_secreaunarbolvacio();
	cuandosecreaunarbol_alcompararconunafuncioncomparadoranula_elarbolnosecrea();
	cuandosecreaunarbol_alcompararconunafunciondestructoranula_elarbolsecrea();
	printf("\n");
	pa2m_nuevo_grupo("Pruebas de insercion");
	dadounarbolvacio_cuandoinsertounelemento_eseelementoterminasiendoraiz();
	dadounarbolvacio_cuandoinsertounelementonulo_elarbolnoestavacio();
	dadounarbolconulmenento_alinsertarunelementomayor_sealmacenacomohijoderecho();
	dadounarbolconulmenento_alinsertarunelementomenor_sealmacenacomohijoizquierdo();
	dadounarbolconulmenento_alinsertarunelementoigual_sealmacenacomohijoizquierdo();
	dadounarbolvacio_alrealizarcuatroinserciones_ladistribuciondelarbolescorrecta();
	printf("\n");
	pa2m_nuevo_grupo("Pruebas de busqueda");
	dadounarbolcontreselemento_cuandobuscounelementodelarbol_encuentroeseelemento();
	dadounarbolcontreselementos_albuscarunelementoquenoestaenelarbol_lafunciondevuelveNULL();
	dadounarbolvacio_albuscarsobreesearbol_lafunciondevuelveNULL();
	dadounarbolnulo_albuscarsobreesearbol_lafunciondevuelveNULL();
	printf("\n");
	pa2m_nuevo_grupo("Pruebas de eliminacion");
	dadounarbolconvacio_aleliminarunelementonulo_noseeliminaningunelementodelarbol();
	dadounarbolcontreslementos_aleliminarunelementoinexistente_noseeliminaningunelementodelarbol();
	pa2m_nuevo_grupo("Pruebas de eliminacion de nodos hoja");
	borrar_nodo_hoja();
	pa2m_nuevo_grupo("Pruebas de eliminacion de nodos con un hijo");
	borrar_nodo_raiz_con_hijo_izquierdo();
	borrar_nodo_raiz_con_hijo_derecho();
	borrar_nodo_no_raiz_con_hijo_izquierdo();
	borrar_nodo_no_raiz_con_hijo_derecho();
	pa2m_nuevo_grupo("Pruebas de eliminacion de nodos con dos hijos");
	borrar_nodo_raiz_con_dos_hijos();
	borrar_nodo_no_raiz_con_dos_hijos();
	borrar_nodo_raiz_con_muchos_subarboles();
	pa2m_nuevo_grupo("Pruebas de eliminaciones multiples");
	pruebas_eliminaciones_multiples();
	pa2m_nuevo_grupo("Pruebas de recorridos");
	printf("\n");
	pa2m_nuevo_grupo("Pruebas de recorridos inorden");
	dadounarbolvacio_alrecorrerinordenelarbol_noserecorreningunelemento();
	dadounarboldesieteelementos_alrecorrerelarbolinordeneinsertanelementosobreunvectordetopemenoralacantidaddeelementosdelarbol_noserecorrentodosloslementosdelarbol();
	dadounarboldesieteelementos_alrecorrerelarbolinordeneinsertanelementosobreunvectordetopemenoralacantidaddelementosdelarbol_sealmacenan5elementosenelvectorenordenascendente();
	dadounarboldesieteelementos_alrecorrerelarbolinordeneinsertanelementosobreunvectordetopeigualalacantidaddeelementosdelarbol_serecorrentodosloslementosdelarbol();
	dadounarboldesieteelementos_alrecorrerelarbolinordeneinsertanelementosobreunvectordetopeigualalacantidaddelementosdelarbol_sealmacenan7elementosenelvectorenordenascendente();
	dadounarboldesieteelementos_alrecorrerelarbolinordeneinsertanelementosobreunvectordetopemayoralacantidaddeelementosdelarbol_serecorrentodosloslementosdelarbol();
	dadounarboldesieteelementos_alrecorrerelarbolinordeneinsertanelementosobreunvectordetopemayoralacantidaddelementosdelarbol_sealmacenan7elementosenelvectorenordenascendente();
	printf("\n");
	pa2m_nuevo_grupo("Pruebas de recorridos preorden");
	dadounarbolvacio_alrecorrerpreordenelarbol_noserecorreningunelemento();
	dadounarboldesieteelementos_alrecorrerelarbolpreordeneinsertanelementosobreunvectordetopemenoralacantidaddeelementosdelarbol_noserecorrentodosloslementosdelarbol();
	dadounarboldesieteelementos_alrecorrerelarbolpreordeneinsertanelementosobreunvectordetopemenoralacantidaddelementosdelarbol_sealmacenan5elementosenelvectorenelordencorrecto();
	dadounarboldesieteelementos_alrecorrerelarbolpreordeneinsertanelementosobreunvectordetopeigualalacantidaddeelementosdelarbol_serecorrentodosloslementosdelarbol();
	dadounarboldesieteelementos_alrecorrerelarbolpreordeneinsertanelementosobreunvectordetopeigualalacantidaddelementosdelarbol_sealmacenan7elementosenelvectorenelordencorrecto();
	dadounarboldesieteelementos_alrecorrerelarbolpreordeneinsertanelementosobreunvectordetopemayoralacantidaddeelementosdelarbol_serecorrentodosloslementosdelarbol();
	dadounarboldesieteelementos_alrecorrerelarbolpreordeneinsertanelementosobreunvectordetopemayoralacantidaddelementosdelarbol_sealmacenan7elementosenelvectorenelordencorrecto();
	printf("\n");
	pa2m_nuevo_grupo("Pruebas de recorridos postorden");
	dadounarbolvacio_alrecorrerpostordenelarbol_noserecorreningunelemento();
	dadounarboldesieteelementos_alrecorrerelarbolpostordeneinsertanelementosobreunvectordetopemenoralacantidaddeelementosdelarbol_noserecorrentodosloslementosdelarbol();
	dadounarboldesieteelementos_alrecorrerelarbolpostordeneinsertanelementosobreunvectordetopemenoralacantidaddelementosdelarbol_sealmacenan5elementosenelvectorenelordencorrecto();
	dadounarboldesieteelementos_alrecorrerelarbolpostordeneinsertanelementosobreunvectordetopeigualalacantidaddeelementosdelarbol_serecorrentodosloslementosdelarbol();
	dadounarboldesieteelementos_alrecorrerelarbolpostordeneinsertanelementosobreunvectordetopeigualalacantidaddelementosdelarbol_sealmacenan7elementosenelvectorenelordencorrecto();
	dadounarboldesieteelementos_alrecorrerelarbolpostordeneinsertanelementosobreunvectordetopemayoralacantidaddeelementosdelarbol_serecorrentodosloslementosdelarbol();
	dadounarboldesieteelementos_alrecorrerelarbolpostordeneinsertanelementosobreunvectordetopemayoralacantidaddelementosdelarbol_sealmacenan7elementosenelvectorenelordencorrecto();
	printf("\n");
	pa2m_nuevo_grupo("Pruebas iteradores internos");
	dadounarbolvacio_alrecorrerelarbolaplicandoeliteradorinterno_noserecorreningunelementodelarbol();
	dadounarbolcon3elementos_alrecorrerelarbolaplicandoeliteradorinternosininvocaraunafuncion_noserecorreningunelementodelarbol();
	dadounarbolcon3elementos_alrecorrerelarbolaplicandoeliteradorinterno_lacantidaddeleementosrecorridosescorrecta();
	dadounarbolcon3elementos_alrecorrerelarbolaplicandoeliteradorinterno_eliteradordejadeiterarcuandocorresponde();
  	printf("\n");
  	pa2m_nuevo_grupo("Pruebas con elementos NULL");
  	dadounarbolconunelementoNULL_albuscardichoelemento_elelementoseencuentraenelarbol();
  	dadounarbolNULL_alquerersabersiestavacio_lafuncionarbolvaciodevuelvetrue();

  	return pa2m_mostrar_reporte();
}
