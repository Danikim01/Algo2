#include "lista.h"
#include "pa2mm.h"

// dado_el_uno_y_el_dos_cuando_los_sumo_obtengo_el_3();

// PRUEBAS INSERTAR FINAL Y ITERADOR
void dada_una_lista_cuando_se_crea_tiene_cero_elementos()
{
  lista_t* lista_nueva = lista_crear();
  pa2m_afirmar(lista_vacia(lista_nueva), "Se crea una lista nueva con 0 elementos");
  lista_destruir(lista_nueva);
}

void dada_una_lista_vacia_cuando_se_inserta_un_elemento_tiene_un_elemento(lista_t* lista, void* elemento)
{
  lista_insertar(lista, elemento);
  pa2m_afirmar(lista->cantidad == 1, "La lista tiene 1 elemento");
}

void dada_una_lista_con_un_elemento_cuando_busco_el_primer_y_ultimo_elemento_son_iguales(lista_t* lista)
{
  pa2m_afirmar(*(char*)lista_ultimo(lista) == *(char*)lista_primero(lista) && *(char*)lista_ultimo(lista) == 'a', "El primer y ultimo elemento es 'a'");
}

void dada_una_lista_con_un_elemento_cuando_inserto_un_elemento_la_lista_tiene_dos_elementos_siendo_el_primer_elemento_el_anterior_y_el_ultimo_el_insertado(lista_t* lista, void* elemento)
{
  lista_insertar(lista, elemento);
  pa2m_afirmar(lista->cantidad == 2, "La lista tiene 2 elementos");
  pa2m_afirmar(*(char*)lista_primero(lista) == 'a', "El primer elemento es 'a'");
  pa2m_afirmar(*(char*)lista_ultimo(lista) == 'b', "El ultimo elemento es 'b'");
}

void dada_una_lista_con_dos_elementos_cuando_inserto_un_elemento_la_lista_tiene_tres_elementos_siendo_el_primer_elemento_el_primero_insertado_y_el_ultimo_elemento_el_recien_insertado(lista_t* lista, void* elemento)
{
  lista_insertar(lista, elemento);
  pa2m_afirmar(lista->cantidad == 3, "La lista tiene 3 elementos");
  pa2m_afirmar(*(char*)lista->nodo_inicio->elemento == 'a', "El primer elemento es 'a'");
  pa2m_afirmar(*(char*)lista_ultimo(lista) == 'c', "El ultimo elemento es 'c'");
}

void dada_una_lista_vacia_cuando_creo_un_iterador_sobre_ella_el_iterador_sera_null()
{
  lista_t* lista_vacia = lista_crear();
  lista_iterador_t* iterador_lista_vacia = lista_iterador_crear(lista_vacia);
  pa2m_afirmar(iterador_lista_vacia->corriente == NULL, "Puedo crear un iterador sobre una lista vacia");
  lista_iterador_destruir(iterador_lista_vacia);
  lista_destruir(lista_vacia);
}

void dada_una_lista_con_elementos_cuando_creo_un_iterador_este_apuntara_al_primer_elemento(lista_iterador_t* iterador)
{
  pa2m_afirmar(*(char*)iterador->corriente->elemento == 'a', "El primer elemento luego de crear un iterador es 'a'");
}

void dado_un_iterador_apuntando_a_un_elemento_cuando_avanzo_el_iterador_apunta_al_siguiente_elemento(lista_iterador_t* iterador)
{
  lista_iterador_avanzar(iterador);
  pa2m_afirmar(*(char*)lista_iterador_elemento_actual(iterador) == 'b', "El iterador avanza al siguiente nodo");
}

void dado_un_iterador_apuntando_a_una_lista_cuando_el_iterador_avanza_por_encima_de_la_cantidad_de_elementos_el_iterador_apuntara_a_null(lista_iterador_t* iterador)
{
  for(int i = 0; i < 3; i++)
    lista_iterador_avanzar(iterador);
  pa2m_afirmar(lista_iterador_elemento_actual(iterador) == NULL, "El iterador avanza por encima de la lista y el elemento actual es NULL");
}

void pruebas_insertar_final_y_iterador()
{
  pa2m_nuevo_grupo("Pruebas crear e insertar");

  dada_una_lista_cuando_se_crea_tiene_cero_elementos();
  lista_t* lista_nueva = lista_crear();
  char a='a', b='b', c='c';
  
  dada_una_lista_vacia_cuando_se_inserta_un_elemento_tiene_un_elemento(lista_nueva, &a);
  dada_una_lista_con_un_elemento_cuando_inserto_un_elemento_la_lista_tiene_dos_elementos_siendo_el_primer_elemento_el_anterior_y_el_ultimo_el_insertado(lista_nueva, &b);
  dada_una_lista_con_dos_elementos_cuando_inserto_un_elemento_la_lista_tiene_tres_elementos_siendo_el_primer_elemento_el_primero_insertado_y_el_ultimo_elemento_el_recien_insertado(lista_nueva, &c);

  pa2m_nuevo_grupo("Pruebas iterador");

  dada_una_lista_vacia_cuando_creo_un_iterador_sobre_ella_el_iterador_sera_null();

  lista_iterador_t* lista_iterador = lista_iterador_crear(lista_nueva);
  dada_una_lista_con_elementos_cuando_creo_un_iterador_este_apuntara_al_primer_elemento(lista_iterador);
  dado_un_iterador_apuntando_a_un_elemento_cuando_avanzo_el_iterador_apunta_al_siguiente_elemento(lista_iterador);
  dado_un_iterador_apuntando_a_una_lista_cuando_el_iterador_avanza_por_encima_de_la_cantidad_de_elementos_el_iterador_apuntara_a_null(lista_iterador);

  lista_destruir(lista_nueva);
  lista_iterador_destruir(lista_iterador);

  return;
}
// ------------------------------------

// PRUEBAS CANTIDAD DE ELEMENTOS
void dada_una_lista_vacia_cuando_obtengo_su_cantidad_de_elementos_es_cero(lista_t* lista)
{
  pa2m_afirmar(lista_elementos(lista) == 0, "La lista no tiene elementos");
}

void dada_una_lista_con_un_elemento_cuando_obtengo_su_cantidad_de_elementos_es_uno(lista_t* lista, void* elemento)
{
  lista_insertar(lista, elemento);
  pa2m_afirmar(lista_elementos(lista) == 1, "La lista tiene 1 elemento");
}

void dada_una_lista_con_un_elemento_cuando_lo_borro_la_cantidad_de_elementos_es_cero(lista_t* lista)
{
  lista_borrar(lista);
  pa2m_afirmar(lista_elementos(lista) == 0, "La lista no tiene elementos");
}

void dada_una_lista_vacia_cuando_inserto_dos_elementos_la_cantidad_de_elementos_es_dos(lista_t* lista, void* elemento1, void* elemento2)
{
  lista_insertar(lista, elemento1);
  lista_insertar(lista, elemento2);
  pa2m_afirmar(lista_elementos(lista) == 2, "La lista tiene 3 elementos");
}

void dada_una_lista_de_tres_elementos_cuando_elimino_el_ultimo_elemento_la_cantidad_de_elementos_es_dos(lista_t* lista)
{
  lista_borrar(lista);
  pa2m_afirmar(lista_elementos(lista) == 1, "La lista tiene 1 elemento");
}

void pruebas_cantidad_elementos()
{
  lista_t* lista = lista_crear();
  int uno=1, dos=2, tres=3;

  pa2m_nuevo_grupo("Pruebas con cantidad de elementos");
  dada_una_lista_vacia_cuando_obtengo_su_cantidad_de_elementos_es_cero(lista);
  dada_una_lista_con_un_elemento_cuando_obtengo_su_cantidad_de_elementos_es_uno(lista, &uno);
  dada_una_lista_con_un_elemento_cuando_lo_borro_la_cantidad_de_elementos_es_cero(lista);
  dada_una_lista_vacia_cuando_inserto_dos_elementos_la_cantidad_de_elementos_es_dos(lista, &dos, &tres);
  dada_una_lista_de_tres_elementos_cuando_elimino_el_ultimo_elemento_la_cantidad_de_elementos_es_dos(lista);

  lista_destruir(lista);
  return;
}
// ------------------------------------

// PRUEBAS DE INSERCION Y ELIMINACION EN POSICIONES ESPECIFICAS
void dada_una_lista_vacia_cuando_quiero_acceder_a_un_elemento_en_la_cuarta_posicion_el_resultado_sera_null(lista_t* lista)
{
  pa2m_afirmar(lista_elemento_en_posicion(lista, 4) == NULL, "Buscar en una posicion inexistente devuelve NULL");
}

void dada_una_lista_vacia_cuando_quiero_borrar_un_elemento_la_funcion_devuelve_error(lista_t* lista)
{
  pa2m_afirmar(lista_borrar_de_posicion(lista, 4) == -1, "No puedo borrar elementos de una lista vacia");
}

void dada_una_lista_cuando_busco_una_posicion_inexistente_la_funcion_devuelve_null(lista_t* lista)
{
  pa2m_afirmar(lista_elemento_en_posicion(lista, 4) == NULL, "Buscar en una posicion inexistente devuelve NULL");
}

void dada_una_lista_vacia_cuando_inserto_en_la_primer_posicion_el_elemento_se_inserta_en_la_primer_posicion(lista_t* lista, void* elemento)
{
  pa2m_afirmar(lista_insertar_en_posicion(lista, elemento, 0) == 0, "Puedo insertar en una lista vacia");
  pa2m_afirmar(*(int*)lista_elemento_en_posicion(lista, 0) == 1, "1 se inserto en la posicion 0");
}

void dada_una_lista_con_un_elemento_cuando_inserto_en_la_primer_posicion_se_inserta_al_principio_y_el_anterior_se_desplaza(lista_t* lista, void* elemento)
{
  pa2m_afirmar(lista_insertar_en_posicion(lista, elemento, 0) == 0, "Puedo insertar en una lista con un elemento previo");
  pa2m_afirmar(*(int*)lista_elemento_en_posicion(lista, 0) == 2, "2 se inserto en la posicion 0");
  pa2m_afirmar(*(int*)lista_elemento_en_posicion(lista, 1) == 1, "1 se desplazo a la posicion 1");
}

void dada_una_lista_con_dos_elementos_cuando_elimino_el_primero_el_unico_elemento_restante_es_el_principio_y_fin(lista_t* lista)
{
  pa2m_afirmar(lista_borrar_de_posicion(lista, 0) == 0, "Elimino el primer elemento");
  pa2m_afirmar(*(int*)lista_primero(lista) == 1, "1 paso a ser el inicio");
  pa2m_afirmar(*(int*)lista_ultimo(lista) == 1, "1 sigue siendo el fin");
}

void dada_una_lista_con_un_elemento_eliminado_previamente_cuando_inserto_en_la_primer_posicion_se_inserta_al_principio_y_el_anterior_se_desplaza(lista_t* lista, void* elemento)
{
  pa2m_afirmar(lista_insertar_en_posicion(lista, elemento, 0) == 0, "Inserto el 3 al principio");
  pa2m_afirmar(*(int*)lista_primero(lista) == 3, "3 paso a ser el inicio");
  pa2m_afirmar(*(int*)lista_ultimo(lista) == 1, "1 sigue siendo el fin"); 
}

void dada_una_lista_con_dos_elementos_cuando_inserto_en_el_medio_el_ultimo_elemento_se_desplaza(lista_t* lista, void* elemento)
{
  pa2m_afirmar(lista_insertar_en_posicion(lista, elemento, 1) == 0, "Puedo insertar en una lista con un dos elementos previos");
  pa2m_afirmar(*(int*)lista_primero(lista) == 3, "3 se mantuvo en la posicion 0");
  pa2m_afirmar(*(int*)lista_elemento_en_posicion(lista, 1) == 4, "4 se inserto en la posicion 1");
  pa2m_afirmar(*(int*)lista_ultimo(lista) == 1, "1 se movio a la posicion 2"); 
}

void dada_una_lista_con_tres_elementos_cuando_elimino_el_elemento_del_medio_el_principio_y_fin_son_adyacentes(lista_t* lista)
{
  pa2m_afirmar(lista_borrar_de_posicion(lista, 1) == 0, "Elimino el elemento del medio");
  lista_iterador_t* iterador = lista_iterador_crear(lista);
  lista_iterador_avanzar(iterador);
  pa2m_afirmar(*(int*)lista_iterador_elemento_actual(iterador) == 1, "1 es elemento siguiente a la primer posicion"); 
  lista_iterador_destruir(iterador);
}

void dada_una_lista_con_dos_elementos_cuando_inserto_en_una_posicion_mayor_a_la_cantidad_de_elementos_se_inserta_al_final(lista_t* lista, void* elemento)
{
  pa2m_afirmar(lista_insertar_en_posicion(lista, elemento, 10) == 0, "Insertar en una lista con una posicion superior a la cantidad de elementos inserta al final");
  pa2m_afirmar(*(int*)lista_ultimo(lista) == 5, "5 se inserto en la ultima posicion");
}

void dada_una_lista_con_tres_elementos_cuando_borro_en_una_posicion_mayor_a_la_cantidad_de_elementos_se_borra_el_ultimo_elemento(lista_t* lista)
{
  pa2m_afirmar(lista_borrar_de_posicion(lista, 10) == 0, "Eliminar un elemento inalcanzable elimina el ultimo elemento");
  pa2m_afirmar(*(int*)lista_ultimo(lista) == 1, "1 se encuentra ahora en la ultima posicion");
}

void pruebas_insertar_eliminar_en_posicion()
{
  pa2m_nuevo_grupo("Prueba de insercion/eliminacion en posiciones especificas");
  lista_t* lista = lista_crear();
  int uno=1, dos=2, tres=3, cuatro=4, cinco=5;

  dada_una_lista_vacia_cuando_quiero_acceder_a_un_elemento_en_la_cuarta_posicion_el_resultado_sera_null(lista);
  dada_una_lista_vacia_cuando_quiero_borrar_un_elemento_la_funcion_devuelve_error(lista);
  dada_una_lista_cuando_busco_una_posicion_inexistente_la_funcion_devuelve_null(lista);
  dada_una_lista_vacia_cuando_inserto_en_la_primer_posicion_el_elemento_se_inserta_en_la_primer_posicion(lista, &uno);
  dada_una_lista_con_un_elemento_cuando_inserto_en_la_primer_posicion_se_inserta_al_principio_y_el_anterior_se_desplaza(lista, &dos);
  dada_una_lista_con_dos_elementos_cuando_elimino_el_primero_el_unico_elemento_restante_es_el_principio_y_fin(lista);
  dada_una_lista_con_un_elemento_eliminado_previamente_cuando_inserto_en_la_primer_posicion_se_inserta_al_principio_y_el_anterior_se_desplaza(lista, &tres);
  dada_una_lista_con_dos_elementos_cuando_inserto_en_el_medio_el_ultimo_elemento_se_desplaza(lista, &cuatro);
  dada_una_lista_con_tres_elementos_cuando_elimino_el_elemento_del_medio_el_principio_y_fin_son_adyacentes(lista);
  dada_una_lista_con_dos_elementos_cuando_inserto_en_una_posicion_mayor_a_la_cantidad_de_elementos_se_inserta_al_final(lista, &cinco);
  dada_una_lista_con_tres_elementos_cuando_borro_en_una_posicion_mayor_a_la_cantidad_de_elementos_se_borra_el_ultimo_elemento(lista);

  lista_destruir(lista);

  return;
}
// ------------------------------------

// PRUEBAS DE PILA
void dada_una_pila_vacia_cuando_obtengo_su_tope_es_null(lista_t* pila)
{
  pa2m_afirmar(lista_tope(pila) == NULL,"El tope de una pila vacia es NULL");
}

void dada_una_pila_vacia_cuando_desapilo_da_error(lista_t* pila)
{
  pa2m_afirmar(lista_desapilar(pila) == -1,"No puedo desapilar en una lista vacia");
}

void dada_una_pila_vacia_cuando_apilo_un_elemento_este_se_vuelve_el_tope(lista_t* pila, void* elemento)
{
  pa2m_afirmar(lista_apilar(pila, elemento) == 0,"Puedo apilar sobre una lista vacia");
  pa2m_afirmar(*(char*)lista_tope(pila) == 'a',"El tope de la pila es 'a'");
}

void dada_una_pila_con_un_elemento_cuando_apilo_dos_elementos_el_tope_es_el_ultimo_apilado(lista_t* pila, void* elemnto1, void* elemnto2)
{
  pa2m_afirmar(lista_apilar(pila, elemnto1) + lista_apilar(pila, elemnto2) == 0,"Puedo apilar 'b' y luego 'c'");
  pa2m_afirmar(*(char*)lista_tope(pila) == 'c',"El tope de la pila  es 'c");
}

void dada_una_pila_con_tres_elementos_cuando_desapilo_el_tope_pasa_a_ser_el_segundo_elemento(lista_t* pila)
{
  pa2m_afirmar(lista_desapilar(pila) == 0,"Puedo desapilar en una lista con tres elementos");
  pa2m_afirmar(*(char*)lista_tope(pila) == 'b',"El nuevo tope de la lista es 'b'");
}

void dada_una_pila_con_dos_elementos_habiendo_desapilando_previamente_cuando_apilo_el_tope_es_el_agregado(lista_t* pila, void* elemento)
{
  pa2m_afirmar(lista_apilar(pila, elemento) == 0,"Puedo apilar sobre una lista con elementos previos habiendo eliminado el ultimo");
  pa2m_afirmar(*(char*)lista_tope(pila) == 'd',"El nuevo tope de la lista es 'd'");
}

void pruebas_pila()
{
  pa2m_nuevo_grupo("Prueba de pila");
  lista_t* pila = lista_crear();
  char a='a', b='b', c='c', d='d';

  dada_una_pila_vacia_cuando_obtengo_su_tope_es_null(pila);
  dada_una_pila_vacia_cuando_desapilo_da_error(pila);
  dada_una_pila_vacia_cuando_apilo_un_elemento_este_se_vuelve_el_tope(pila, &a);
  dada_una_pila_con_un_elemento_cuando_apilo_dos_elementos_el_tope_es_el_ultimo_apilado(pila, &b, &c);
  dada_una_pila_con_tres_elementos_cuando_desapilo_el_tope_pasa_a_ser_el_segundo_elemento(pila);
  dada_una_pila_con_dos_elementos_habiendo_desapilando_previamente_cuando_apilo_el_tope_es_el_agregado(pila, &d);

  lista_destruir(pila);
  return;
}
// ------------------------------------

// PRUEBAS COLA
void dada_una_cola_vacia_cuando_obtengo_el_primero_es_null(lista_t* cola)
{
  pa2m_afirmar(lista_primero(cola) == NULL,"El primer elemento de una cola vacia es NULL")
}

void dada_una_cola_vacia_cuando_desencolo_da_error(lista_t* cola)
{
  pa2m_afirmar(lista_desencolar(cola) == -1,"No puedo desapilar en una lista vacia");
}

void dada_una_cola_vacia_cuando_encolo_un_elemento_el_primero_sera_el_agregado(lista_t* cola, void* elemento)
{
  pa2m_afirmar(lista_encolar(cola, elemento) == 0, "Puedo encolar en una lista vacia");
  pa2m_afirmar(*(char*)lista_primero(cola) == 'a', "el primer elemento de la cola es 'a'");
}

void dada_una_cola_con_un_elemento_cuando_encolo_dos_elementos_el_primero_sigue_siendo_el_elemento_previo(lista_t* cola, void* elemento1, void* elemento2)
{
  pa2m_afirmar(lista_encolar(cola, elemento1) + lista_encolar(cola, elemento2) == 0,"Puedo encolar 'b' y luego 'c'");
  pa2m_afirmar(*(char*)lista_primero(cola) == 'a',"El primero de la cola es 'a");
}

void dada_una_cola_con_tres_elementos_cuando_desencolo_el_primero_pasa_a_ser_el_segundo_elemento(lista_t* cola)
{
  pa2m_afirmar(lista_desencolar(cola) == 0,"Puedo desencolar en una lista con tres elementos");
  pa2m_afirmar(*(char*)lista_primero(cola) == 'b',"El nuevo primero de la cola es 'b'");
}

void dada_una_cola_con_dos_elementos_habiendo_desencolado_previamente_cuando_encolo_el_primero_sigue_siendo_el_mismo(lista_t* cola, void* elemento)
{
  pa2m_afirmar(lista_encolar(cola, elemento) == 0,"Puedo encolar sobre una cola con elementos previos habiendo desencolado anteriormente");
  pa2m_afirmar(*(char*)lista_primero(cola) == 'b',"El primer elemento de la cola sigue siendo 'b'");
}

void pruebas_cola()
{
  pa2m_nuevo_grupo("Prueba de cola");
  lista_t* cola = lista_crear();
  char a='a', b='b', c='c', d='d';

  dada_una_cola_vacia_cuando_obtengo_el_primero_es_null(cola);
  dada_una_cola_vacia_cuando_desencolo_da_error(cola);
  dada_una_cola_vacia_cuando_encolo_un_elemento_el_primero_sera_el_agregado(cola, &a);
  dada_una_cola_con_un_elemento_cuando_encolo_dos_elementos_el_primero_sigue_siendo_el_elemento_previo(cola, &b, &c);
  dada_una_cola_con_tres_elementos_cuando_desencolo_el_primero_pasa_a_ser_el_segundo_elemento(cola);
  dada_una_cola_con_dos_elementos_habiendo_desencolado_previamente_cuando_encolo_el_primero_sigue_siendo_el_mismo(cola, &d);

  lista_destruir(cola);
  return;
}
// ------------------------------------

// PRUEBAS FUNCION lista_con_cada_elemento 
bool encontrar_elemento_caracter(void* elemento_actual, void* elemento_buscado)
{
  if (!elemento_actual || !elemento_buscado)
    return false;

  bool continuar = true;

  if(*(char*)elemento_actual == *(char*)elemento_buscado)
    continuar = false;

  return continuar;
}

bool mostrar_elemento(void* elemento_actual, void* vacio)
{
  if (!elemento_actual)
    return false;

  printf("%c", *(char*)elemento_actual);
  return true;
}

void dada_una_funcion_que_recibe_como_argumento_otra_si_esta_es_nula_el_resultado_sera_null(lista_t* lista)
{
  pa2m_afirmar(lista_con_cada_elemento(lista, NULL, NULL) == 0,
  "Pasar una funcion NULL devuelve 0 elementos iterados");  
}

void dada_una_funcion_que_recibe_como_argumento_otra_si_esta_imprime_todos_los_elementos_itera_todos_los_elementos(lista_t* lista)
{
  pa2m_afirmar(lista_con_cada_elemento(lista, mostrar_elemento, NULL) == 4,
  "Imprimo la lista entera");  
}

void dada_una_funcion_que_recibe_como_argumento_otra_si_esta_busca_un_elemento_itera_hasta_encontrarlo(lista_t* lista)
{
  char elemento_buscado = 'b';
  pa2m_afirmar(lista_con_cada_elemento(lista, encontrar_elemento_caracter, &elemento_buscado) == 2,
  "El elemento b se encuentra en la segunda posicion de la lista");  
}

void dada_una_funcion_que_recibe_como_argumento_otra_si_esta_busca_un_elemento_y_este_es_null_no_iterara(lista_t* lista)
{
  pa2m_afirmar(lista_con_cada_elemento(lista, encontrar_elemento_caracter, NULL) == 1,
  "No puedo encontrar un elemento si el buscado es NULL");  
}

void pruebas_puntero_a_funcion()
{
  pa2m_nuevo_grupo("Pruebas de funcion con puntero a funcion");

  lista_t* lista_funciones = lista_crear();
  char a='a', b='b', c='c', d='d';

  lista_insertar(lista_funciones, &a);
  lista_insertar(lista_funciones, &b);
  lista_insertar(lista_funciones, &c);
  lista_insertar(lista_funciones, &d);

  dada_una_funcion_que_recibe_como_argumento_otra_si_esta_es_nula_el_resultado_sera_null(lista_funciones);
  dada_una_funcion_que_recibe_como_argumento_otra_si_esta_imprime_todos_los_elementos_itera_todos_los_elementos(lista_funciones);
  dada_una_funcion_que_recibe_como_argumento_otra_si_esta_busca_un_elemento_itera_hasta_encontrarlo(lista_funciones);
  dada_una_funcion_que_recibe_como_argumento_otra_si_esta_busca_un_elemento_y_este_es_null_no_iterara(lista_funciones);

  lista_destruir(lista_funciones);
  return;
}
// ------------------------------------

int main() {
  
  pruebas_insertar_final_y_iterador(); 
  pruebas_cantidad_elementos();
  pruebas_insertar_eliminar_en_posicion();
  pruebas_pila();
  pruebas_cola();
  pruebas_puntero_a_funcion();

  return pa2m_mostrar_reporte();
}