#include "pa2mm.h"
#include "src/salon.h"
#include <stdlib.h>
#include <string.h>

void prueba_creacion_salon(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	pa2m_afirmar(salon,"Se pudo leer el salon");

	salon_destruir(salon);
}

void prueba_no_puedo_leer_salon_con_entrenadores_sin_pokemon(){
	salon_t* salon = salon_leer_archivo("salones/salon_prueba_1.txt");
	pa2m_afirmar(salon == NULL,"No Se pudo leer un salon con entrenadores sin pokemon");
	salon_destruir(salon);
}

void prueba_no_puedo_leer_salon_con_entrenadores_duplicados(){
	salon_t* salon = salon_leer_archivo("archivo_prueba_2.sal");
	pa2m_afirmar(salon == NULL,"No Se pudo leer un salon con entrenadores duplicados");
	salon_destruir(salon);
}

bool hay_entrenador(entrenador_t* entrenador,void* extra){
	extra=extra;
	return (entrenador != NULL);
}

bool entrenadores_con_ciertas_victorias(entrenador_t* entrenador,void* extra){
	return (victorias_entrenador(entrenador) == (int)extra);
}

bool entrenadores_con_cierto_nombre(entrenador_t* entrenador,void* extra){
	return (strcmp(nombre_entrenador(entrenador),(char*)extra) == 0);
}

bool entrenadores_con_cierta_cantidad_de_pokemones(entrenador_t* entrenador,void* extra){
	return (cantidad_pokemones_entrenador(entrenador) == (int)extra);
}

bool entrenadores_con_cierto_pokemon_aux(entrenador_t* entrenador,void* extra){
	return(entrenador_tiene_pokemon(lista_pokemones(entrenador),(char*)extra) == false);
}


void prueba_lista_entrenadores(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	lista_t* lista = NULL;
	lista = salon_filtrar_entrenadores(salon,hay_entrenador,NULL);
	pa2m_afirmar(lista,"Se pudo crear una lista filtrada con todos los entrenadores del salon");
	pa2m_afirmar(lista_elementos(lista) == 3,"La lista filtrada tiene 3 elementos");
	pa2m_afirmar(strcmp(nombre_entrenador((entrenador_t*)lista_elemento_en_posicion(lista,0)),"Lucas") == 0,"En la primera posicion de la lista se encuentra el entrenador correcto");
	lista_destruir(lista);
	salon_destruir(salon);
}

void prueba_filtrar_entrenadores_segun_victorias(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	lista_t* lista = NULL;
	int victorias = 1;
	lista = salon_filtrar_entrenadores(salon,entrenadores_con_ciertas_victorias,(void*)victorias);
	pa2m_afirmar(lista,"Se pudo crear una lista filtrada con entrenadores con 1 victoria");
	pa2m_afirmar(lista_elementos(lista) == 1,"La lista filtrada tiene solo 1 elemento");
	pa2m_afirmar(strcmp(nombre_entrenador((entrenador_t*)lista_elemento_en_posicion(lista,0)),"Lucas") == 0,"En la primera posicion de la lista se encuentra el entrenador correcto");
	lista_destruir(lista);
	salon_destruir(salon);
}

void prueba_filtrar_entrenadores_segun_nombre(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	lista_t* lista = NULL;
	char* nombre = "Manu";
	lista = salon_filtrar_entrenadores(salon,entrenadores_con_cierto_nombre,(void*)nombre);
	pa2m_afirmar(lista,"Se pudo crear una lista filtrada con entrenadores cuyo nombre sea Manu");
	pa2m_afirmar(lista_elementos(lista) == 1,"La lista filtrada tiene solo 1 elemento");
	pa2m_afirmar(strcmp(nombre_entrenador((entrenador_t*)lista_elemento_en_posicion(lista,0)),"Manu") == 0,"En la primera posicion de la lista se encuentra el entrenador correcto");
	lista_destruir(lista);
	salon_destruir(salon);
}

void prueba_filtrar_entrenadores_segun_pokemon(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	lista_t* lista = NULL;
	char* pokemon = "Pikachu";
	lista = salon_filtrar_entrenadores(salon,entrenadores_con_cierto_pokemon_aux,(void*)pokemon);
	pa2m_afirmar(lista,"Se pudo crear una lista filtrada con entrenadores cuyo pokemon sea Pikachu");
	pa2m_afirmar(lista_elementos(lista) == 1,"La lista filtrada tiene solo 1 elemento");
	pa2m_afirmar(strcmp(nombre_entrenador((entrenador_t*)lista_elemento_en_posicion(lista,0)),"Lucas") == 0,"En la primera posicion de la lista se encuentra el entrenador correcto");
	lista_destruir(lista);
	salon_destruir(salon);
}

void prueba_multiples_filtros_segun_cantidad_pokemones(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	lista_t* lista = NULL;
	size_t cantidad_pokemones = 3;
	lista = salon_filtrar_entrenadores(salon,entrenadores_con_cierta_cantidad_de_pokemones,(void*)cantidad_pokemones);
	pa2m_afirmar(lista,"Se pudo crear una lista filtrada con entrenadores que tienen 2 pokemones");
	pa2m_afirmar(lista_elementos(lista) == 2,"La lista filtrada tiene 2 elementos");
	pa2m_afirmar(strcmp(nombre_entrenador((entrenador_t*)lista_elemento_en_posicion(lista,0)),"Lucas") == 0,"En la primera posicion de la lista se encuentra el entrenador correcto");
	pa2m_afirmar(strcmp(nombre_entrenador((entrenador_t*)lista_elemento_en_posicion(lista,1)),"Mariano") == 0,"En la segunda posicion de la lista se encuentra el entrenador correcto");
	lista_destruir(lista);
	salon_destruir(salon);
}

void prueba_ejecutar_comandos_entrenadores(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	char* string_entreandores = NULL;
	string_entreandores = salon_ejecutar_comando(salon,"ENTRENADORES");
	pa2m_afirmar(strcmp(string_entreandores,"Lucas,1\nManu,5\nMariano,3\n")==0,"Devuelve correctamente los entrenadores con sus victorias");
	free(string_entreandores);
	salon_destruir(salon);
}

void prueba_ejecutar_comando_entrenadores_con_n_victorias(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	char* string_entreandores_n_victorias = NULL;
	string_entreandores_n_victorias = salon_ejecutar_comando(salon,"ENTRENADORES:victorias,3");
	pa2m_afirmar(strcmp(string_entreandores_n_victorias,"Manu\nMariano\n")==0,"Devuelve correctamente los entrenadores con n victorias");
	free(string_entreandores_n_victorias);
	salon_destruir(salon);
}

void prueba_ejecutar_comando_entrenadores_con_cierto_pokemon(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	char* string_entreandores_cierto_pokemon = NULL;
	string_entreandores_cierto_pokemon = salon_ejecutar_comando(salon,"ENTRENADORES:pokemon,Lugia");
	pa2m_afirmar(strcmp(string_entreandores_cierto_pokemon,"Mariano\n")==0,"Devuelve correctamente los entrenadores con cierto pokemon");
	free(string_entreandores_cierto_pokemon);
	salon_destruir(salon);
}

void prueba_ejecutar_listar_pokemones_de_cierto_entrenador(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	char* string_entreandores_equipo = NULL;
	string_entreandores_equipo = salon_ejecutar_comando(salon,"EQUIPO:Manu");
	pa2m_afirmar(strcmp(string_entreandores_equipo,"Chikorita,25,5,3,8,7\nCharizard,15,7,8,9,5\n")==0,"Devuelve correctamente el entrenador con su equipo pokemon");
	free(string_entreandores_equipo);
	salon_destruir(salon);
}

void prueba_ejecutar_reglas(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	char* string_reglas = salon_ejecutar_comando(salon,"REGLAS");
	pa2m_afirmar(strcmp(string_reglas,"CLASICO,0.8*nivel+fuerza+2*velocidad\nMODERNO,0.5*nivel+0.9*defensa+3*inteligencia\nCaracter,el pokemon ganador es aquel cuya suma de los caracteres de su nombre sea mayor\nLargo,el pokemon ganador es aquel cuya longuitud de los caracteres de su nombre sea mayor\nNivel,el pokemon ganador es aquel cuyo nivel sea mayor\n")==0,"Devuelve correctamente las reglas registradas en el sistema");
	free(string_reglas);
	salon_destruir(salon);
}

void prueba_ejecutar_reglas_comparacion(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	char* string_reglas_comparacion = salon_ejecutar_comando(salon,"COMPARAR:Lucas,Mariano,MODERNO");
	pa2m_afirmar(strcmp(string_reglas_comparacion,"2\n2\n2\n")==0,"El ganador es el entrenador 2:Mariano");
	free(string_reglas_comparacion);
	salon_destruir(salon);
}

void prueba_ejecutar_reglas_comparacion_2(){
	salon_t* salon = salon_leer_archivo("salones/salon_estandar.txt");
	char* string_reglas_comparacion_2 = salon_ejecutar_comando(salon,"COMPARAR:Lucas,Stephi,CLASICO");
	pa2m_afirmar(strcmp(string_reglas_comparacion_2,"1\n1\n2\n2\n2\n1\n1\n1\n2\n2\n2\n")==0,"El ganador es el entrenador 2:Mariano");
	char* string_reglas_comparacion_3 = salon_ejecutar_comando(salon,"COMPARAR:Mariano,Pucci,MODERNO");
	pa2m_afirmar(strcmp(string_reglas_comparacion_3,"2\n2\n1\n1\n1\n2\n2\n")==0,"El ganador es el entrenador 2:Mariano");
	free(string_reglas_comparacion_3);
	free(string_reglas_comparacion_2);
	salon_destruir(salon);
}



void prueba_ejecutar_reglas_caracter(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	char* string_reglas_caracter = salon_ejecutar_comando(salon,"COMPARAR:Mariano,Lucas,Caracter");
	pa2m_afirmar(strcmp(string_reglas_caracter,"1\n2\n2\n2\n")==0,"El ganador es el entrenador 2:Lucas");
	free(string_reglas_caracter);
	salon_destruir(salon);
}

void prueba_ejecutar_reglas_nombre_mas_largo(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	char* string_reglas_caracter = salon_ejecutar_comando(salon,"COMPARAR:Lucas,Manu,Largo");
	pa2m_afirmar(strcmp(string_reglas_caracter,"2\n1\n1\n")==0,"El ganador es el entrenador 1:Lucas");
	free(string_reglas_caracter);
	salon_destruir(salon);
}

void prueba_ejecutar_reglas_pokemon_mayor_nivel(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	char* string_reglas_caracter = salon_ejecutar_comando(salon,"COMPARAR:Mariano,Manu,Nivel");
	pa2m_afirmar(strcmp(string_reglas_caracter,"1\n1\n")==0,"El ganador es el entrenador 1:Mariano");
	free(string_reglas_caracter);
	salon_destruir(salon);
}

void prueba_guardar_salon(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	char* salon_guardar = salon_ejecutar_comando(salon,"GUARDAR:salon_nuevo.sal");
	pa2m_afirmar(strcmp(salon_guardar,"OK") == 0,"Se guardo corectamente el salon a un archivo");
	free(salon_guardar);
	salon_destruir(salon);
}

bool es_entrenador_aux(entrenador_t* entrenador,void* extra){
	extra=extra;
	return (entrenador != NULL);
}

bool mostrar_pok(void* elemento,void* extra){
	printf("%s\n",(char*)elemento);
	return true;
}

void pruebas_varias_agregar_quitar_y_guardar_salon(){
	salon_t* salon = salon_leer_archivo("salones/salon_estandar.txt");
	char* string_pruebas_equipo = salon_ejecutar_comando(salon,"EQUIPO:Mariano");
	pa2m_afirmar(strcmp(string_pruebas_equipo,"Lapras,46,47,18,29,40\nArticuno,21,12,60,14,15\nDragonair,71,92,71,34,25\nLapras,81,22,90,7,35\n") == 0,"Devuelve el equipo de mariano en el orden correcto");
	char* string_pruebas_quitar_1 = salon_ejecutar_comando(salon,"QUITAR_POKEMON:Mariano,Lapras");
	pa2m_afirmar(strcmp(string_pruebas_quitar_1,"OK") == 0,"El comando QUITAR_POKEMON:Mariano,Lapras devuelve OK");
	
	char* string_pruebas_equipo_2 = salon_ejecutar_comando(salon,"EQUIPO:Mariano");
	pa2m_afirmar(strcmp(string_pruebas_equipo_2,"Articuno,21,12,60,14,15\nDragonair,71,92,71,34,25\nLapras,81,22,90,7,35\n") == 0,"Devuelve el equipo de mariano en el orden correcto sin el primer Lapras");
	free(string_pruebas_equipo_2);

	char* string_pruebas_quitar_2 = salon_ejecutar_comando(salon,"QUITAR_POKEMON:Mariano,Lapras");
	pa2m_afirmar(strcmp(string_pruebas_quitar_2,"OK") == 0,"El comando QUITAR_POKEMON:Mariano,Lapras devuelve OK");
	free(string_pruebas_quitar_2);

	char* string_pruebas_equipo_3 = salon_ejecutar_comando(salon,"EQUIPO:Mariano");
	pa2m_afirmar(strcmp(string_pruebas_equipo_3,"Articuno,21,12,60,14,15\nDragonair,71,92,71,34,25\n") == 0,"Devuelve el equipo de mariano en el orden correcto sin el segundo Lapras");
	free(string_pruebas_equipo_3);

	char* string_pruebas_quitar_3 = salon_ejecutar_comando(salon,"QUITAR_POKEMON:Mariano,Dragonair");
	pa2m_afirmar(strcmp(string_pruebas_quitar_3,"OK") == 0,"El comando QUITAR_POKEMON:Mariano,Dragonair devuelve OK");
	free(string_pruebas_quitar_3);

	char* string_pruebas_quitar_4 = salon_ejecutar_comando(salon,"QUITAR_POKEMON:Mariano,Articuno");
	pa2m_afirmar(string_pruebas_quitar_4 == NULL,"El comando QUITAR_POKEMON:Mariano,Articuno devuelve ERROR");
	free(string_pruebas_quitar_4);

	char* string_pruebas_equipo_4 = salon_ejecutar_comando(salon,"EQUIPO:Mariano");
	pa2m_afirmar(strcmp(string_pruebas_equipo_4,"Articuno,21,12,60,14,15\n") == 0,"Devuelve el equipo de mariano en el orden correcto");
	free(string_pruebas_equipo_4);
	
	char* string_agregar_pok_1 = salon_ejecutar_comando(salon,"AGREGAR_POKEMON:Mariano,Poke,1,2,3,4,5");
	pa2m_afirmar(strcmp(string_agregar_pok_1,"OK") == 0,"El comando AGREGAR_POKEMON:Mariano,Poke,1,2,3,4,5 devuelve OK");
	free(string_agregar_pok_1);

	lista_t* lista_entrenadores = salon_filtrar_entrenadores(salon,entrenadores_con_cierto_nombre,(void*)"Mariano");



	lista_destruir(lista_entrenadores);

	char* string_pruebas_equipo_5 = salon_ejecutar_comando(salon,"EQUIPO:Mariano");
	pa2m_afirmar(strcmp(string_pruebas_equipo_5,"Articuno,21,12,60,14,15\nPoke,1,2,3,4,5\n") == 0,"Devuelve el equipo de mariano en el orden correcto");
	free(string_pruebas_equipo_5);
	
	char* salon_guardar = salon_ejecutar_comando(salon,"GUARDAR:salon_nuevo.sal");
	pa2m_afirmar(strcmp(salon_guardar,"OK") == 0,"Se guardo corectamente el salon a un archivo con sus cambios");
	
	free(salon_guardar);
	free(string_pruebas_equipo);
	free(string_pruebas_quitar_1);

	salon_destruir(salon);
}

void pruebas_agregar_varias_y_guardar_salon(){
	salon_t* salon = salon_leer_archivo("salones/salon_estandar.txt");
	
	char* string_pruebas_agregar_1 = salon_ejecutar_comando(salon,"AGREGAR_POKEMON:Mariano,Pok_1,1,2,3,4,5");
	pa2m_afirmar(strcmp(string_pruebas_agregar_1,"OK") == 0,"El comando AGREGAR_POKEMON:Mariano,Pok_1,1,2,3,4,5 devuelve OK");
	free(string_pruebas_agregar_1);

	char* string_pruebas_equipo_2 = salon_ejecutar_comando(salon,"EQUIPO:Mariano");
	pa2m_afirmar(strcmp(string_pruebas_equipo_2,"Lapras,46,47,18,29,40\nArticuno,21,12,60,14,15\nDragonair,71,92,71,34,25\nLapras,81,22,90,7,35\nPok_1,1,2,3,4,5\n") == 0,"Devuelve el equipo de mariano en el orden correcto con el primer Pok_1");


	free(string_pruebas_equipo_2);

	char* string_pruebas_agregar_2 = salon_ejecutar_comando(salon,"AGREGAR_POKEMON:Mariano,Pok_2,1,2,3,4,5");
	pa2m_afirmar(strcmp(string_pruebas_agregar_2,"OK") == 0,"El comando AGREGAR_POKEMON:Mariano,Pok_2,1,2,3,4,5 devuelve OK");
	free(string_pruebas_agregar_2);

	char* string_pruebas_equipo_3 = salon_ejecutar_comando(salon,"EQUIPO:Mariano");

	pa2m_afirmar(strcmp(string_pruebas_equipo_3,"Lapras,46,47,18,29,40\nArticuno,21,12,60,14,15\nDragonair,71,92,71,34,25\nLapras,81,22,90,7,35\nPok_1,1,2,3,4,5\nPok_2,1,2,3,4,5\n") == 0,"Devuelve el equipo de mariano en el orden correcto con el segundo Pok_2");
	free(string_pruebas_equipo_3);

	char* string_pruebas_agregar_3 = salon_ejecutar_comando(salon,"AGREGAR_POKEMON:Mariano,Pok_3,1,2,3,4,5");
	pa2m_afirmar(strcmp(string_pruebas_agregar_3,"OK") == 0,"El comando AGREGAR_POKEMON:Mariano,Pok_3,1,2,3,4,5 devuelve OK");
	free(string_pruebas_agregar_3);

	char* string_pruebas_equipo_4 = salon_ejecutar_comando(salon,"EQUIPO:Mariano");

	pa2m_afirmar(strcmp(string_pruebas_equipo_4,"Lapras,46,47,18,29,40\nArticuno,21,12,60,14,15\nDragonair,71,92,71,34,25\nLapras,81,22,90,7,35\nPok_1,1,2,3,4,5\nPok_2,1,2,3,4,5\nPok_3,1,2,3,4,5\n") == 0,"Devuelve el equipo de mariano en el orden correcto con el Pok_3");
	free(string_pruebas_equipo_4);

	char* string_pruebas_quitar_1 = salon_ejecutar_comando(salon,"QUITAR_POKEMON:Mariano,Pok_3");
	pa2m_afirmar(strcmp(string_pruebas_quitar_1,"OK") == 0,"El comando QUITAR_POKEMON:Mariano,Pok_3 devuelve OK");
	free(string_pruebas_quitar_1);

	char* string_pruebas_equipo_5 = salon_ejecutar_comando(salon,"EQUIPO:Mariano");
	pa2m_afirmar(strcmp(string_pruebas_equipo_5,"Lapras,46,47,18,29,40\nArticuno,21,12,60,14,15\nDragonair,71,92,71,34,25\nLapras,81,22,90,7,35\nPok_1,1,2,3,4,5\nPok_2,1,2,3,4,5\n") == 0,"Devuelve el equipo de mariano en el orden correcto");
	free(string_pruebas_equipo_5);

	char* salon_guardar = salon_ejecutar_comando(salon,"GUARDAR:salon_nuevo_3.sal");
	pa2m_afirmar(strcmp(salon_guardar,"OK") == 0,"Se guardo corectamente el salon a un archivo con sus cambios");
	free(salon_guardar);


	salon_destruir(salon);
}

void pruebas_varias_quitar_y_guardar_salon(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	char* string_pruebas_quitar_inexistente = salon_ejecutar_comando(salon,"QUITAR_POKEMON:Lucas,KAKAKAAK");
	pa2m_afirmar(string_pruebas_quitar_inexistente == NULL,"No se puede quitar un pokemon inexistente");
	char* string_pruebas_quitar = salon_ejecutar_comando(salon,"QUITAR_POKEMON:Lucas,Pikachu");
	pa2m_afirmar(strcmp(string_pruebas_quitar,"OK") == 0,"Se pudo quitar un pokemon al entrenador Lucas");
	char* string_pruebas_quitar_2 = salon_ejecutar_comando(salon,"QUITAR_POKEMON:Lucas,Bulbasaur");
	pa2m_afirmar(strcmp(string_pruebas_quitar_2,"OK") == 0,"Se pudo quitar otro pokemon al entrenador Lucas");
	char* string_pruebas_quitar_3 = salon_ejecutar_comando(salon,"QUITAR_POKEMON:Lucas,Squirtle");
	pa2m_afirmar(string_pruebas_quitar_3 == NULL,"No se puede quitar un pokemon a un entreandor con solo 1 pokemon");
	char* string_pruebas_quitar_4 = salon_ejecutar_comando(salon,"QUITAR_POKEMON:Mariano,Lugia");
	pa2m_afirmar(strcmp(string_pruebas_quitar_4,"OK") == 0,"Se quito un pokemon al entreandor Mariano");
	char* string_pruebas_quitar_5 = salon_ejecutar_comando(salon,"QUITAR_POKEMON:Manu,Charizard");
	pa2m_afirmar(strcmp(string_pruebas_quitar_5,"OK") == 0,"Se quito un pokemon al entreandor Manu");
	char* salon_guardar = salon_ejecutar_comando(salon,"GUARDAR:salon_nuevo_2.sal");
	pa2m_afirmar(strcmp(salon_guardar,"OK") == 0,"Se guardo corectamente el salon a un archivo con sus cambios");
	free(salon_guardar);
	free(string_pruebas_quitar);
	free(string_pruebas_quitar_2);
	free(string_pruebas_quitar_4);
	free(string_pruebas_quitar_5);
	salon_destruir(salon);
}

void pruebas_con_NULL_y_formatos_incorrectos(){
	salon_t* salon = salon_leer_archivo("salon_original.sal");
	char* string_comando_invalido = NULL;
	string_comando_invalido = salon_ejecutar_comando(salon,"jkhdkfjahsdfjadh");
	pa2m_afirmar(string_comando_invalido==NULL,"Ingresar un comando invalido con 1 solo parametro (no registrado) devuelve NULL");
	free(string_comando_invalido);

	char* string_comando_invalido_2 = NULL;
	string_comando_invalido_2 = salon_ejecutar_comando(salon,"jkhdkfjahsdfjadh,asdfasdvhja,fuhf728y");
	pa2m_afirmar(string_comando_invalido_2==NULL,"Ingresar un comando invalido con 3 parametros (no registrados) devuelve NULL");
	free(string_comando_invalido_2);

	char* string_entreandores_n_victorias = NULL;
	string_entreandores_n_victorias = salon_ejecutar_comando(salon,"ENTRENADORES:victorias,900");
	pa2m_afirmar(strcmp(string_entreandores_n_victorias,"\0")==0,"Al buscar un entrenador con una cantidad de victorias no registradas, devuelve un string vacio");
	free(string_entreandores_n_victorias);

	char* string_entreandores_n_victorias_invalido = NULL;
	string_entreandores_n_victorias_invalido = salon_ejecutar_comando(salon,"ENTRENADORES:victorias,900,#");
	pa2m_afirmar(string_entreandores_n_victorias_invalido==NULL,"Al ejecutar el comando ENTRENADORES con una cantidad incorrecta de parametros, devuelve NULL");
	free(string_entreandores_n_victorias_invalido);

	char* string_entreandores_cierto_pokemon = NULL;
	string_entreandores_cierto_pokemon = salon_ejecutar_comando(salon,"ENTRENADORES:pokemon,kokow");
	pa2m_afirmar(strcmp(string_entreandores_cierto_pokemon,"\0")==0,"Al buscar un entrenador con un pokemon no registrado, devuelve un string vacio");
	free(string_entreandores_cierto_pokemon);

	char* string_entreandores_cierto_pokemon_invalido = NULL;
	string_entreandores_cierto_pokemon_invalido = salon_ejecutar_comando(salon,"ENTRENADORES:pokemon,victorias,25");
	pa2m_afirmar(string_entreandores_cierto_pokemon_invalido==NULL,"Al buscar un entrenador con un pokemon no registrado y ejecutar ENTRENADORES:pokemon con una cantidad erronea de parametros, devuelve un NULL");
	free(string_entreandores_cierto_pokemon_invalido);

	char* string_entreandores_inexistente = NULL;
	string_entreandores_inexistente = salon_ejecutar_comando(salon,"AGREGAR_POKEMON:Moriano,Poke,1,2,3,4,5 ");
	pa2m_afirmar(string_entreandores_inexistente==NULL,"Al agregar un pokemon a un entrenador inexistente, devuelve un NULL");
	free(string_entreandores_inexistente);

	char* string_entreandores_equipo = NULL;
	string_entreandores_equipo = salon_ejecutar_comando(salon,"EQUIPO:kokdosak");
	pa2m_afirmar(string_entreandores_equipo==NULL,"Al buscar un el equpo de un entreandor inexistente, devuelve NULL");
	free(string_entreandores_equipo);

	char* string_reglas_comparacion = salon_ejecutar_comando(salon,"COMPARAR:Lucas,Danielito,Moderno");
	pa2m_afirmar(string_reglas_comparacion==NULL,"Al querer COMPARAR un entreandor con otro invalido, devuelve NULL");
	free(string_reglas_comparacion);

	char* string_reglas_regla_no_valida = salon_ejecutar_comando(salon,"COMPARAR:Mariano,Lucas,unareglanoregistrada");
	pa2m_afirmar(string_reglas_regla_no_valida==NULL,"Al querer COMPARAR dos entreandores validos con una regla no registrada, devuelve NULL");
	free(string_reglas_regla_no_valida);

	char* string_sin_reglas = salon_ejecutar_comando(salon,"COMPARAR:Lucas,Manu");
	pa2m_afirmar(string_sin_reglas==NULL,"Al querer COMPARAR dos entrenadores validos sin una regla, devuelve NULL");
	free(string_sin_reglas);

	char* string_comparacion_un_entrenador = salon_ejecutar_comando(salon,"COMPARAR:Mariano");
	pa2m_afirmar(string_comparacion_un_entrenador==NULL,"Al querer COMPARAR solo 1 entrenador, devuelve NULL");
	free(string_comparacion_un_entrenador);

	salon_destruir(salon);

}

int main(){
	
    pa2m_nuevo_grupo("Pruebas TP_2");
    pa2m_nuevo_grupo("Pruebas creacion de salon");
    prueba_creacion_salon();
    prueba_no_puedo_leer_salon_con_entrenadores_sin_pokemon();
    prueba_no_puedo_leer_salon_con_entrenadores_duplicados();    
    pa2m_nuevo_grupo("Pruebas filtrar entreandores segun cierto criterio");
    prueba_lista_entrenadores();
    prueba_filtrar_entrenadores_segun_victorias();
    prueba_filtrar_entrenadores_segun_nombre();
    prueba_filtrar_entrenadores_segun_pokemon();
    prueba_multiples_filtros_segun_cantidad_pokemones();
    pa2m_nuevo_grupo("Pruebas ejecucion de comandos");
    pa2m_nuevo_grupo("Pruebas de comando: ENTRENADORES");
    prueba_ejecutar_comandos_entrenadores();
    prueba_ejecutar_comando_entrenadores_con_n_victorias();
    prueba_ejecutar_comando_entrenadores_con_cierto_pokemon();
    pa2m_nuevo_grupo("Pruebas de comando: EQUIPO");
    prueba_ejecutar_listar_pokemones_de_cierto_entrenador();
    pa2m_nuevo_grupo("Pruebas de comando: REGLAS");
    prueba_ejecutar_reglas();
    pa2m_nuevo_grupo("Pruebas de comando: COMPARAR");
    prueba_ejecutar_reglas_comparacion();
    prueba_ejecutar_reglas_comparacion_2();
    prueba_ejecutar_reglas_caracter();
    prueba_ejecutar_reglas_nombre_mas_largo();
    prueba_ejecutar_reglas_pokemon_mayor_nivel();
    pa2m_nuevo_grupo("Pruebas de guardar salon agregando y/o quitando pokemones");
 	prueba_guardar_salon();
	pruebas_agregar_varias_y_guardar_salon();
    pruebas_varias_agregar_quitar_y_guardar_salon();
    pruebas_varias_quitar_y_guardar_salon();
    pa2m_nuevo_grupo("Pruebas con NULL y ejecucion de comandos invalidos");
    pruebas_con_NULL_y_formatos_incorrectos();
	
    return pa2m_mostrar_reporte();
}
