#include "lista.h"
#include "pa2mm.h"
#include <string.h>

#define ELEMENTOS_INSERTADOS 3

typedef struct elemento_a_insertar{
	void* campo1;
	void* campo2;
}elemento_a_insertar_t;
/*
* Devuelve true si ambos caracteres (validos) recibidos son iguales, en caso contrario devuelve false.
*/
bool comparador_char(char elemento1, char elemento2){
	return (elemento1==elemento2);
}

void Cuandocreounalista_SeCreaUnalistaVacia(){
	lista_t* lista = NULL;
	lista = lista_crear();
	pa2m_afirmar(lista != NULL,"Se pudo crear una lista");
	pa2m_afirmar(lista_vacia(lista) == true, "Se creo una lista vacia ");
	lista_destruir(lista);
}
void pruebas_insercion(){
	char letra_a_insertar = 'a';
	int segunda_elemento_a_insertar = 1;
	elemento_a_insertar_t otro_elemento_a_insertar; 

	lista_t* lista = NULL;
	lista = lista_crear();
	if(lista != NULL){
		pa2m_afirmar(lista_insertar(lista, &(letra_a_insertar)) == 0 , "Se pudo insertar algo en la lista");
		pa2m_afirmar((lista_elementos(lista)) == 1,"La lista tiene un elemento");
		pa2m_afirmar(lista_insertar(lista, &(segunda_elemento_a_insertar)) == 0, "Pudo insertar otro elemento a la lista");
		pa2m_afirmar((lista_elementos(lista)) == 2,"La lista tiene dos elementos"); 
		pa2m_afirmar(lista_insertar(lista, &(otro_elemento_a_insertar)) == 0, "Pudo insertar un tercer elemento"); 
		pa2m_afirmar((lista_elementos(lista)) == 3,"La lista tiene tres elementos");

		lista_insertar(lista,NULL);
		pa2m_afirmar((lista_elementos(lista)) == 4,"Al insertar vacio la lista se agranda la lista");
		pa2m_afirmar(lista_elemento_en_posicion(lista,0) == &(letra_a_insertar), "El primer elemento es el correcto");
		pa2m_afirmar(lista_elemento_en_posicion(lista,1) == &(segunda_elemento_a_insertar), "El segundo elemento es el correcto");
		pa2m_afirmar(lista_elemento_en_posicion(lista,2) == &(otro_elemento_a_insertar), "El tercer elemento es el correcto");
 
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

bool mostrar_elemento(void* elemento, void* contador){
    if(elemento && contador)
        printf("Elemento %i: %c \n", (*(int*)contador)++, *(char*)elemento);
    return true;
}

void dadaunalistavacia_alinsertarmultipleslementosdesdeeliniciodelalista_seinsertancorrectamenteloselementos(){

	char a='a', b='b',c='c', d='d',e='e',f='f';

	lista_t* lista = NULL;
	lista = lista_crear();

	if(lista != NULL){

		pa2m_afirmar(lista_insertar_en_posicion(lista,&a,0) == 0, "Se pudo insertar un elemento en la primera posicion de la lista");
		lista_insertar_en_posicion(lista,&b,0);
		lista_insertar_en_posicion(lista,&c,0);
		lista_insertar_en_posicion(lista,&d,0);
		lista_insertar_en_posicion(lista,&e,0);
		lista_insertar_en_posicion(lista,&f,0);
		pa2m_afirmar(comparador_char(*(char*)lista_elemento_en_posicion(lista,0),f) && comparador_char(*(char*)lista_elemento_en_posicion(lista,1),e)
			&& comparador_char(*(char*)lista_elemento_en_posicion(lista,2),d) && comparador_char(*(char*)lista_elemento_en_posicion(lista,3),c) && 
			comparador_char(*(char*)lista_elemento_en_posicion(lista,4),b) && comparador_char(*(char*)lista_elemento_en_posicion(lista,5),a),"Pude insertar 6 elementos desde el inicio de la lista");
		
	

	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);


}

void dadaunalistavacia_alinsertarmultipleselementos_seinsertantodosloslementosenlalista(){

	char a='a', b='b', c='c', d='d',e='e',f='f';

	lista_t* lista = NULL;
	lista = lista_crear();
	if(lista != NULL){
		pa2m_afirmar(lista_insertar(lista, &(a)) == 0 && lista_insertar(lista, &(b)) == 0 && lista_insertar(lista, &(c)) == 0
		&& lista_insertar(lista, &(d)) == 0 && lista_insertar(lista, &(e)) == 0 && lista_insertar(lista, &(f)) == 0, "Se pudieron insertar multiples elementos en una lista");
		pa2m_afirmar(lista_elementos(lista) == 6,"La lista tiene 6 elementos");
 
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

void dadaunalistacontreselementos_siagregounelementoenlasegundaposicion_elelementoseinsertaenlaposicioncorrecta(){
	lista_t* lista = NULL;
	char primer_elemento = 'a', segundo_elemento = 'b', tercer_elemento = 'c', elemento_a_insertar = 'd';
	lista = lista_crear();
	if(lista != NULL){
		lista_insertar(lista, &(primer_elemento));
		lista_insertar(lista, &(segundo_elemento));
		lista_insertar(lista, &(tercer_elemento));
		lista_insertar_en_posicion(lista, &(elemento_a_insertar), 1);
		pa2m_afirmar(comparador_char(*(char*)lista_elemento_en_posicion(lista,1),elemento_a_insertar), "El elemento se inserta en la posicion correcta");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}
void dadaunalistadetreselementos_alquererinsertarodenadamenteunelementoenunaposicioninexsitente_seinsertaenlaultimaposicion(){
	lista_t* lista = NULL;
	lista = lista_crear();
	char primer_elemento = 'a', segundo_elemento = 'b', tercer_elemento = 'c', elemento_a_insertar = 'd';
	if(lista != NULL){
		lista_insertar(lista, &(primer_elemento));
		lista_insertar(lista, &(segundo_elemento));
		lista_insertar(lista, &(tercer_elemento));
		lista_insertar_en_posicion(lista, &(elemento_a_insertar), 10);
		pa2m_afirmar(comparador_char(*(char*)lista_elemento_en_posicion(lista,((lista_elementos(lista))-1)),elemento_a_insertar),"El elemento se inserta en la ultima posicion al pasar una posicion mayor a la cantidad de elementos de la lista");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}
void dadounalistavacio_cuandoquieraeliminarelultimoelemento_noseeliminanada(){
	lista_t* lista = NULL;
	lista_borrar(lista);
	pa2m_afirmar(lista_vacia(lista),"Una lista vacia no es eliminada");
}

void dadaunalistacontreslementos_alquererborrarloselementosdesdeelinicio_seborrancorrectamente(){

	lista_t* lista = NULL;
	lista = lista_crear();
	char primer_elemento = 'a', segundo_elemento = 'b', tercer_elemento = 'c';
	if(lista != NULL){
		lista_insertar(lista, &(primer_elemento));
		lista_insertar(lista, &(segundo_elemento));
		lista_insertar(lista, &(tercer_elemento));

		lista_borrar_de_posicion(lista,0);
		lista_borrar_de_posicion(lista,0);
		lista_borrar_de_posicion(lista,0);

	    
		pa2m_afirmar((lista_vacia(lista)), "Pude eliminar todos los elementos eliminandolos desde la primera posicion");
		
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

void dadaunalistacontreselementos_aleliminarsegunposicion_seeliminaelementoenlaposicionadecuada(){
	lista_t* lista = NULL;
	lista = lista_crear();
	char primer_elemento = 'a', segundo_elemento = 'b', tercer_elemento = 'c';
	if(lista != NULL){
		lista_insertar(lista, &(primer_elemento));
		lista_insertar(lista, &(segundo_elemento));
		lista_insertar(lista, &(tercer_elemento));
		size_t posicion = 1;
		lista_borrar_de_posicion(lista,posicion);
		pa2m_afirmar((lista_elementos(lista)) == 2, "Se elimino un elemento correctamente tras eliminar segun posicion");
		pa2m_afirmar(comparador_char(*(char*)lista_elemento_en_posicion(lista,0),primer_elemento),"Se elimina correctamente segun posicion");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

void dadaunalistadetreselementos_alquerereliminarenunaposicioninvalida_seeliminaelultimoelemento(){
	lista_t* lista = NULL;
	lista = lista_crear();
	char primer_elemento = 'a', segundo_elemento = 'b', tercer_elemento = 'c';
	if(lista != NULL){
		lista_insertar(lista, &(primer_elemento));
		lista_insertar(lista, &(segundo_elemento));
		lista_insertar(lista, &(tercer_elemento));
		size_t posicion = 563;
		lista_borrar_de_posicion(lista,posicion);
		pa2m_afirmar((lista_elementos(lista)) == 2, "Se elimino un elemento correctamente tras eliminar segun posicion invalida");
		pa2m_afirmar(comparador_char(*(char*)lista_elemento_en_posicion(lista,(lista_elementos(lista))-1),segundo_elemento),"El elemento eliminado es el correcto tras la eliminacion segun posicion invalida");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

void dadaunalistavacia_alquererapilarunelemento_lalistacontienealelementoapilado(){

	lista_t* lista = NULL;
	lista = lista_crear();
	char elemento_a_apilar = 'd';
	if(lista != NULL){
		lista_apilar(lista,&(elemento_a_apilar));
		pa2m_afirmar(comparador_char(*(char*)lista_ultimo(lista),elemento_a_apilar),"Apila correctamente sobre una lista vacia");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

void dadaunalistadetreselementos_alapilarunelemento_seapilacorretamenteenlaprimeraposiciondelalista(){

	lista_t* lista = NULL;
	lista = lista_crear();
	char primer_elemento = 'a', segundo_elemento = 'b', tercer_elemento = 'c', elemento_a_insertar = 'd';
	if(lista != NULL){
		lista_insertar(lista, &(primer_elemento));
		lista_insertar(lista, &(segundo_elemento));
		lista_insertar(lista, &(tercer_elemento));
		lista_apilar(lista,&(elemento_a_insertar));
		
		pa2m_afirmar(comparador_char(*(char*)lista_primero(lista),elemento_a_insertar),"Apila correctamente el elemento a apilar");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);

}

void dadaunalistavacia_alquererdesapilar_lalistapermanecesincambios(){
	lista_t* lista = NULL;
	lista = lista_crear();

	if(lista != NULL){
		pa2m_afirmar(lista_desapilar(lista) == -1,"No desapila elementos en una lista vacia");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

void dadaunalistadetreselementos_aldesapilar_sedesapilaelprimerelementoapilado(){

	lista_t* lista = NULL;
	lista = lista_crear();
	char primer_elemento = 'a', segundo_elemento = 'b', tercer_elemento = 'c';
	if(lista != NULL){
		lista_insertar(lista, &(primer_elemento));
		lista_insertar(lista, &(segundo_elemento));
		lista_insertar(lista, &(tercer_elemento));

		lista_desapilar(lista);
		pa2m_afirmar(comparador_char(*(char*)lista_primero(lista),segundo_elemento),"Desapila correctamente el primer elemento apilado");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

void dadaunalistadetreselementosapilados_alquerersabercualeselultimoelementoapilado_devuelvecorrectamenteelelementobuscado(){
	lista_t* lista = NULL;
	lista = lista_crear();
	char primer_elemento = 'a', segundo_elemento = 'b', tercer_elemento = 'c';
	if(lista != NULL){
		lista_apilar(lista, &(primer_elemento));
		lista_apilar(lista, &(segundo_elemento));
		lista_apilar(lista, &(tercer_elemento));
		
		pa2m_afirmar(comparador_char(*(char*)lista_tope(lista),tercer_elemento),"El elemento buscado corresponde ultimo elemento insertado");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}
void dadaunalistavacia_alquererencolarunelemento_lalistacontienealelementoencolado(){

	lista_t* lista = NULL;
	lista = lista_crear();
	char elemento_a_encolar = 'd';
	if(lista != NULL){
		lista_encolar(lista,&(elemento_a_encolar));
		pa2m_afirmar(comparador_char(*(char*)lista_ultimo(lista),elemento_a_encolar),"Encola correctamente sobre una lista vacia");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

void dadaunalistavacia_alquererdesencolarunelemento_lalistapermanecesincambios(){

	lista_t* lista = NULL;
	lista = lista_crear();

	if(lista != NULL){
		pa2m_afirmar(lista_desencolar(lista) == -1,"No desencola elementos de una lista vacia");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

void dadaunalistadetreselementos_alencolarunelemento_seencolacorretamenteenlaultimaposciondelalista(){

	lista_t* lista = NULL;
	lista = lista_crear();
	char primer_elemento = 'a', segundo_elemento = 'b', tercer_elemento = 'c', elemento_a_insertar = 'd';
	if(lista != NULL){
		lista_insertar(lista, &(primer_elemento));
		lista_insertar(lista, &(segundo_elemento));
		lista_insertar(lista, &(tercer_elemento));
		lista_encolar(lista,&(elemento_a_insertar));
		pa2m_afirmar(comparador_char(*(char*)lista_elemento_en_posicion(lista,3),elemento_a_insertar),"Encola correctamente el elemento a encolar");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

void dadaunalistadecuatroelementos_aldesencolar_elprimeroelementodelalistasedesencolacorrectamente(){

	lista_t* lista = NULL;
	lista = lista_crear();
	char primer_elemento = 'a', segundo_elemento = 'b', tercer_elemento = 'c', cuarto_elemento = 'd';
	if(lista != NULL){
		lista_insertar(lista, &(primer_elemento));
		lista_insertar(lista, &(segundo_elemento));
		lista_insertar(lista, &(tercer_elemento));
		lista_insertar(lista,&(cuarto_elemento));
		lista_desencolar(lista);
		size_t primera_posicion = 0;
		pa2m_afirmar(comparador_char(*(char*)lista_elemento_en_posicion(lista,primera_posicion),segundo_elemento),"Desencola correctamente el primer elemento encolado");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);
}

void dadaunalistadetreselementos_alquerersabercualeselprimerelementodelacola_devuelvecorrectamenteelelementobuscado(){

	lista_t* lista = NULL;
	lista = lista_crear();
	char primer_elemento = 'a', segundo_elemento = 'b', tercer_elemento = 'c';

	if(lista != NULL){
		lista_insertar(lista, &(primer_elemento));
		lista_insertar(lista, &(segundo_elemento));
		lista_insertar(lista, &(tercer_elemento));

		pa2m_afirmar(comparador_char(*(char*)lista_primero(lista),primer_elemento),"El primero elemento de la lista corresponde al primero");
	} else {
		printf("Error al crear la lista\n");
	}
	lista_destruir(lista);

}

void dadaunalistavacia_alcrearunaiterador_secreauniterador(){
	lista_t* lista_vacia = lista_crear();
  	pa2m_afirmar(lista_iterador_crear(lista_vacia) == NULL, "No puedo crear un iterador sobre una lista vacia");
  	lista_destruir(lista_vacia);

}

void probar_iterador_externo_de_lista(){

	lista_t* lista = NULL;
	lista = lista_crear();
	char primer_elemento = 'a', segundo_elemento = 'b', tercer_elemento = 'c';

	lista_insertar(lista, &(primer_elemento));
	lista_insertar(lista, &(segundo_elemento));
	lista_insertar(lista, &(tercer_elemento));

	lista_iterador_t* iterador = NULL;
	pa2m_afirmar((iterador = lista_iterador_crear(lista)),"Pudo crear un iterador");
	pa2m_afirmar(lista_iterador_tiene_siguiente(iterador),"El iterador tiene siguiente");
	pa2m_afirmar((lista_iterador_elemento_actual(iterador) == &(primer_elemento)) ,"El primer elemento es el correcto");
	pa2m_afirmar(lista_iterador_tiene_siguiente(iterador),"El iterador tiene siguiente");
	lista_iterador_avanzar(iterador);
	pa2m_afirmar((lista_iterador_elemento_actual(iterador) == &(segundo_elemento)) ,"El segundo elemento es el correcto");
	pa2m_afirmar(lista_iterador_tiene_siguiente(iterador),"El iterador tiene siguiente");
	lista_iterador_avanzar(iterador);
	pa2m_afirmar((lista_iterador_elemento_actual(iterador) == &(tercer_elemento)) ,"El tercer elemento elemento es el correcto");
	lista_iterador_avanzar(iterador);
	pa2m_afirmar(lista_iterador_tiene_siguiente(iterador) == false,"El iterador no tiene mas elementos");
	pa2m_afirmar((lista_iterador_elemento_actual(iterador) == NULL),"Si no hay mas elementos el iterador devuelve NULL");

	lista_destruir(lista);	
	lista_iterador_destruir(iterador);

}
/*
Devuelve true en todo momento y aumenta el valor de la variabla enviada por parametro de la funcion 
casteandolo previamente a una variable de tipo size_t.
*/
bool _funcion_(void* elemento, void* aux){
	(*(size_t*)aux)+=1;
	return true;
}
/*
Devuelve false si elemento es 10, sino devuelve true, ademas aumenta el valor de la variabla enviada por parametro de la funcion 
casteandolo previamente a una variable de tipo size_t.
*/
bool funcion_2(void* elemento, void* aux){
	(*(size_t*)aux)+=1;
	if(*(int*)elemento == 10)
		return false;
	return true;
}
/*
Devuelve false si elemento es 20, sino devuelve true, ademas aumenta el valor de la variabla enviada por parametro de la funcion 
casteandolo previamente a una variable de tipo size_t.
*/
bool funcion_3(void* elemento, void* aux){
	(*(size_t*)aux)+=1;
	if(*(int*)elemento == 20)
		return false;
	return true;
}
/*
Devuelve false si elemento es 30, sino devuelve true, ademas aumenta el valor de la variabla enviada por parametro de la funcion 
casteandolo previamente a una variable de tipo size_t.
*/
bool funcion_4(void* elemento, void* aux){
	(*(size_t*)aux)+=1;
	if(*(int*)elemento == 30)
		return false;
	return true;
}

void probar_iterador_interno_de_lista(){
	lista_t* una_lista = lista_crear();
	int n_1 = 10, n_2 = 20, n_3 = 30;

	pa2m_afirmar(0 == lista_con_cada_elemento(una_lista,_funcion_,NULL),"El iterador no itera sobre una lista sin elementos");
	pa2m_afirmar(0 == lista_con_cada_elemento(una_lista,NULL,NULL),"El iterador no itera sobre una lista si el puntero a funcion es NULL");
	lista_insertar(una_lista, &(n_1));
	lista_insertar(una_lista, &(n_2));
	lista_insertar(una_lista, &(n_3));

	size_t contador = 0, contador_aux = 0, contador_aux_2 = 0, contador_aux_3 = 0;

	pa2m_afirmar((ELEMENTOS_INSERTADOS - 3 == lista_con_cada_elemento(una_lista,funcion_2,&(contador))) && (ELEMENTOS_INSERTADOS - 2 == lista_con_cada_elemento(una_lista,funcion_3,&(contador_aux))) && (ELEMENTOS_INSERTADOS - 1 == lista_con_cada_elemento(una_lista,funcion_4,&(contador_aux_2))) && (ELEMENTOS_INSERTADOS == lista_con_cada_elemento(una_lista,_funcion_,&(contador_aux_3))),"El iterador interno itera correctamente sobre los 3 elementos de la lista");

	lista_destruir(una_lista);
}
int main() {
	
  pa2m_nuevo_grupo("Pruebas de listas");
  printf("\n");		
  pa2m_nuevo_grupo("Pruebas de creacion");
  Cuandocreounalista_SeCreaUnalistaVacia();
  pa2m_nuevo_grupo("Pruebas de insercion");
  pruebas_insercion();
  dadaunalistacontreselementos_siagregounelementoenlasegundaposicion_elelementoseinsertaenlaposicioncorrecta();
  dadaunalistadetreselementos_alquererinsertarodenadamenteunelementoenunaposicioninexsitente_seinsertaenlaultimaposicion();
  pa2m_nuevo_grupo("Pruebas de insercion multiple");
  dadaunalistavacia_alinsertarmultipleselementos_seinsertantodosloslementosenlalista();
  dadaunalistavacia_alinsertarmultipleslementosdesdeeliniciodelalista_seinsertancorrectamenteloselementos();
  pa2m_nuevo_grupo("Pruebas de eliminacion");
  dadounalistavacio_cuandoquieraeliminarelultimoelemento_noseeliminanada();
  dadaunalistacontreselementos_aleliminarsegunposicion_seeliminaelementoenlaposicionadecuada();
  dadaunalistadetreselementos_alquerereliminarenunaposicioninvalida_seeliminaelultimoelemento();
  dadaunalistacontreslementos_alquererborrarloselementosdesdeelinicio_seborrancorrectamente();

  pa2m_nuevo_grupo("Pruebas de lista_apilar y lista_desapilar");
  dadaunalistavacia_alquererapilarunelemento_lalistacontienealelementoapilado();
  dadaunalistadetreselementos_alapilarunelemento_seapilacorretamenteenlaprimeraposiciondelalista();
  dadaunalistavacia_alquererdesapilar_lalistapermanecesincambios();
  dadaunalistadetreselementos_aldesapilar_sedesapilaelprimerelementoapilado();
  dadaunalistadetreselementosapilados_alquerersabercualeselultimoelementoapilado_devuelvecorrectamenteelelementobuscado();
  pa2m_nuevo_grupo("Pruebas de lista_encolar y lista_desencolar");
  dadaunalistavacia_alquererencolarunelemento_lalistacontienealelementoencolado();
  dadaunalistadetreselementos_alencolarunelemento_seencolacorretamenteenlaultimaposciondelalista();
  dadaunalistadecuatroelementos_aldesencolar_elprimeroelementodelalistasedesencolacorrectamente();
  dadaunalistavacia_alquererdesencolarunelemento_lalistapermanecesincambios();
  dadaunalistadetreselementos_alquerersabercualeselprimerelementodelacola_devuelvecorrectamenteelelementobuscado();

  pa2m_nuevo_grupo("Pruebas de iteradores externos");
  printf("\n");
  pa2m_nuevo_grupo("Pruebas de creacion");	
  dadaunalistavacia_alcrearunaiterador_secreauniterador();
  probar_iterador_externo_de_lista();

  pa2m_nuevo_grupo("Pruebas de iterador interno");
  printf("\n");
  probar_iterador_interno_de_lista();

  return pa2m_mostrar_reporte();
}
