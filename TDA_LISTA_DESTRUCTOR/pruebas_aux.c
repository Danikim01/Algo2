#include "lista.h"
#include "pa2mm.h"
#include <string.h>
#include <stdlib.h>

typedef struct cosa{
    int clave;
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

void destructor_de_cosas(void* elemento){
    if(!elemento)
        return;
    destruir_cosa((cosa*)elemento);
}

/*
* Devuelve true si ambos caracteres (validos) recibidos son iguales, en caso contrario devuelve false.
*/
bool comparador_int(int elemento1, int elemento2){
	return (elemento1==elemento2);
}

void Cuandocreounalista_SeCreaUnalistaVacia(){
	lista_t* lista = NULL;
	lista = lista_crear(destructor_de_cosas);
	pa2m_afirmar(lista != NULL,"Se pudo crear una lista");
	pa2m_afirmar(lista_vacia(lista) == true, "Se creo una lista vacia ");
	lista_destruir(lista);
}

void pruebas_insercion(){
	int primero = 1;
	int segundo = 2;

	cosa* c1= crear_cosa(primero);
	cosa* c2= crear_cosa(segundo);

	lista_t* lista = NULL;
	lista = lista_crear(destructor_de_cosas);

	pa2m_afirmar(lista_insertar(lista, c1) == 0 , "Se pudo insertar algo en la lista");
	pa2m_afirmar((lista_elementos(lista)) == 1,"La lista tiene un elemento");
	pa2m_afirmar(lista_insertar(lista, c2) == 0, "Pudo insertar otro elemento a la lista");
	pa2m_afirmar((lista_elementos(lista)) == 2,"La lista tiene dos elementos"); 

	lista_insertar(lista,NULL);
	pa2m_afirmar((lista_elementos(lista)) == 3,"Al insertar vacio la lista se agranda la lista");
	pa2m_afirmar(lista_elemento_en_posicion(lista,0) == c1, "El primer elemento es el correcto");
	pa2m_afirmar(lista_elemento_en_posicion(lista,1) == c2, "El segundo elemento es el correcto");
	
	lista_destruir(lista);
}

void dadounalistavacio_cuandoquieraeliminarelultimoelemento_noseeliminanada(){
	lista_t* lista = NULL;
	lista_borrar(lista);
	pa2m_afirmar(lista_vacia(lista),"Una lista vacia no es eliminada");
}

void dadaunalistacontreselementos_aleliminarsegunposicion_seeliminaelementoenlaposicionadecuada(){
	lista_t* lista = NULL;
	lista = lista_crear(destructor_de_cosas);
	int primer_elemento = 1, segundo_elemento = 2, tercer_elemento = 2;
	cosa* cosa_1 = crear_cosa(primer_elemento);
	cosa* cosa_2 = crear_cosa(segundo_elemento);
	cosa* cosa_3 = crear_cosa(tercer_elemento);
	if(lista != NULL){
		lista_insertar(lista, cosa_1);
		lista_insertar(lista, cosa_2);
		lista_insertar(lista, cosa_3);
		size_t posicion = 1;
		lista_borrar_de_posicion(lista,posicion);
		pa2m_afirmar((lista_elementos(lista)) == 2, "Se elimino un elemento correctamente tras eliminar segun posicion");
		pa2m_afirmar(comparador_int(*(int*)lista_elemento_en_posicion(lista,0),primer_elemento),"Se elimina correctamente segun posicion");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

void dadaunalistadetreselementos_alquerereliminarenunaposicioninvalida_seeliminaelultimoelemento(){
	lista_t* lista = NULL;
	lista = lista_crear(destructor_de_cosas);
	int primer_elemento = 1, segundo_elemento = 2, tercer_elemento = 2;
	cosa* cosa_1 = crear_cosa(primer_elemento);
	cosa* cosa_2 = crear_cosa(segundo_elemento);
	cosa* cosa_3 = crear_cosa(tercer_elemento);
	if(lista != NULL){
		lista_insertar(lista, cosa_1);
		lista_insertar(lista, cosa_2);
		lista_insertar(lista, cosa_3);
		size_t posicion = 563;
		lista_borrar_de_posicion(lista,posicion);
		pa2m_afirmar((lista_elementos(lista)) == 2, "Se elimino un elemento correctamente tras eliminar segun posicion invalida");
		pa2m_afirmar(comparador_int(*(int*)lista_elemento_en_posicion(lista,(lista_elementos(lista))-1),segundo_elemento),"El elemento eliminado es el correcto tras la eliminacion segun posicion invalida");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

void dadaunalistacontreslementos_alquererborrarloselementosdesdeelinicio_seborrancorrectamente(){

	lista_t* lista = NULL;
	lista = lista_crear(destructor_de_cosas);
	int primer_elemento = 1, segundo_elemento = 2, tercer_elemento = 2;
	cosa* cosa_1 = crear_cosa(primer_elemento);
	cosa* cosa_2 = crear_cosa(segundo_elemento);
	cosa* cosa_3 = crear_cosa(tercer_elemento);
	if(lista != NULL){
		lista_insertar(lista, cosa_1);
		lista_insertar(lista, cosa_2);
		lista_insertar(lista, cosa_3);

		lista_borrar_de_posicion(lista,0);
		lista_borrar_de_posicion(lista,0);
		lista_borrar_de_posicion(lista,0);

	    
		pa2m_afirmar((lista_vacia(lista)), "Pude eliminar todos los elementos eliminandolos desde la primera posicion");
		
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

int main() {

  pa2m_nuevo_grupo("Pruebas de creacion");
  Cuandocreounalista_SeCreaUnalistaVacia();
  printf("\n");	
  pa2m_nuevo_grupo("Pruebas de insercion");
  pruebas_insercion();	
  
  pa2m_nuevo_grupo("Pruebas de eliminacion");
  dadounalistavacio_cuandoquieraeliminarelultimoelemento_noseeliminanada();
  
  dadaunalistacontreselementos_aleliminarsegunposicion_seeliminaelementoenlaposicionadecuada();
  
  dadaunalistadetreselementos_alquerereliminarenunaposicioninvalida_seeliminaelultimoelemento();
  dadaunalistacontreslementos_alquererborrarloselementosdesdeelinicio_seborrancorrectamente();
  
  return pa2m_mostrar_reporte();
}