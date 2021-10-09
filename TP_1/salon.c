#include "salon.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SEPARADOR ';'

/*
Pre condiciones : La funcion recibe un entrenador_t entrenador cuyo espacio en memoria dinamica fue reservado previamente (en este caso mediante un calloc lo cual llena de ceros los valores en memoria reservada)
y recibe, a su vez, el puntero a un puntero a char de elementos inicializados, los cuales corresponden al nombre y a la cantidad de victorias del entrenador a inicializar.

Post condiciones : La funcion inicializa del struct entrenador_t* el nombre del entrenador y la cantidad de victorias.
*/

void inicializar_entrenador(entrenador_t* entrenador, char** elementos){

	if(!entrenador || !elementos)
		return;

    strcpy(entrenador->nombre,(elementos[0]));
    entrenador->victorias = atoi(elementos[1]);

}

/*
Pre condiciones : La funcion recibe un pokemon_t* pokemon cuyo espacio en memoria dinamica fue reservado previamente (en este caso mediante un calloc lo cual llena de ceros los valores en memoria reservada)
y recibe, a su vez, el puntero a un puntero a char de elementos inicializados, los cuales corresponden a los datos del pokemon a inicializar.

Post condiciones : La funcion inicializa del struct pokemon_t* todos los campos del mismo.
*/

void inicializar_pokemon(pokemon_t* pokemon, char** elementos){

	if(!pokemon || !elementos)
		return;

    strcpy(pokemon->nombre,(elementos[0]));
    pokemon->nivel = atoi(elementos[1]);
    pokemon->fuerza = atoi(elementos[3]);
    pokemon->inteligencia = atoi(elementos[4]);
    pokemon->defensa = atoi(elementos[2]);
    pokemon->velocidad = atoi(elementos[5]);

}


salon_t* salon_leer_archivo(const char* nombre_archivo){


	FILE *archivo = fopen(nombre_archivo,"r");

	if(!archivo){
		return NULL;
	}

	pokemon_t* pokemon;

  	entrenador_t* entrenador;

 	salon_t* salon;

 	salon = calloc(1,sizeof(salon_t));

	if(!salon){
		return NULL;
	}

	char separador = SEPARADOR;
	char* linea_leida = fgets_alloc(archivo);
	char** linea_spliteada = split(linea_leida,separador);
	size_t tamanio_linea = vtrlen(linea_spliteada);

	 while(linea_leida != NULL){

        entrenador = calloc(1,sizeof(entrenador_t));
        
		if(!entrenador){
			return NULL;
		}

		inicializar_entrenador(entrenador,linea_spliteada);

        free(linea_leida);
        vtrfree(linea_spliteada);

        linea_leida = fgets_alloc(archivo);
        linea_spliteada = split(linea_leida,separador);
        tamanio_linea = vtrlen(linea_spliteada);

        while(tamanio_linea != 2 && linea_leida != NULL){

            pokemon = calloc(1,sizeof(pokemon_t));
            
            if(!pokemon){
				return NULL;
			}

			inicializar_pokemon(pokemon,linea_spliteada);
            
            entrenador->equipo =  vtradd(entrenador->equipo,pokemon);

            free(linea_leida);
        	vtrfree(linea_spliteada);

            linea_leida = fgets_alloc(archivo);
            linea_spliteada = split(linea_leida,separador);
            tamanio_linea = vtrlen(linea_spliteada);

        }

        salon->entrenadores = vtradd(salon->entrenadores,entrenador);

    }


	fclosen(archivo);
	

	return salon;
	

}


int salon_guardar_archivo(salon_t* salon, const char* nombre_archivo){


	FILE* archivo_guardar = fopen(nombre_archivo,"w");

	int cant_entrenadores = 0;

	cant_entrenadores = (int)(vtrlen(salon->entrenadores));

	if(!archivo_guardar){
		return -1;
	}

	size_t i;
	size_t j; 

	for(i = 0; i < (vtrlen(salon->entrenadores));i++){

		fprintf(archivo_guardar,"%s;%i\n",salon->entrenadores[i]->nombre,salon->entrenadores[i]->victorias);

		for(j = 0; j < (vtrlen(salon->entrenadores[i]->equipo)) ;j++){

			fprintf(archivo_guardar,"%s;%i;%i;%i;%i;%i\n",salon->entrenadores[i]->equipo[j]->nombre,salon->entrenadores[i]->equipo[j]->nivel,salon->entrenadores[i]->equipo[j]->defensa,salon->entrenadores[i]->equipo[j]->fuerza,salon->entrenadores[i]->equipo[j]->inteligencia,salon->entrenadores[i]->equipo[j]->velocidad);
		}

	}
		

	fclosen(archivo_guardar);

	
	return cant_entrenadores;

}

/*
Pre condiciones : La funcion recibe un puntero a un salon cuyo espacio en memoria para los entrenadores , es decir para salon->entrenadores, tuvo que haber sido reservado previamente,
por otra parte recibe un entrenador auxiliar inicializado en NULL, y una cant_entrenadores que requiere llegar con un valor cargado.

Post condiciones : La funcion ordena el vector de entrenadores ascendentemente segun la cantidad de victorias.
*/

void ordenar_vector_dinamico(salon_t* salon,entrenador_t* entrenador_aux,size_t cant_entrenadores){


		for(size_t i = 0; i < cant_entrenadores;i++){
			for(size_t j = 0; j < (cant_entrenadores-i-1);j++){
				if((salon->entrenadores[j]->victorias) > (salon->entrenadores[j+1]->victorias)){

					entrenador_aux = salon->entrenadores[j];
					salon->entrenadores[j] = salon->entrenadores[j+1];
					salon->entrenadores[j+1] = entrenador_aux;
				}
			}
		}


}


salon_t* salon_agregar_entrenador(salon_t* salon, entrenador_t* entrenador){


	if(!salon || !entrenador)
		return NULL;

	void* aux = realloc(salon->entrenadores,sizeof(entrenador_t));
	if(!aux){
		return NULL;
	}

	salon->entrenadores = aux;

	salon->entrenadores = vtradd(salon->entrenadores, entrenador);

	entrenador_t* entrenador_aux = NULL;

	ordenar_vector_dinamico(salon,entrenador_aux,vtrlen(salon->entrenadores));

	return salon;

}

entrenador_t** salon_obtener_entrenadores_mas_ganadores(salon_t* salon, int cantidad_minima_victorias){

	
	if(!salon){
		return NULL;
	}

	entrenador_t** entrenador__a_devolver = NULL;

	size_t cantidad_entrendadores = vtrlen(salon->entrenadores);

	for(size_t i = 0; i < cantidad_entrendadores;i++){

		if( (salon->entrenadores[i]->victorias) >= cantidad_minima_victorias){

			entrenador__a_devolver = vtradd(entrenador__a_devolver,salon->entrenadores[i]);

		}
	}

    return entrenador__a_devolver;
    
}


void salon_mostrar_entrenador(entrenador_t* entrenador){

	if(!entrenador){
		return;
	}

	size_t j;
	size_t cant_pokemones = vtrlen(entrenador->equipo);


	printf("%s - %i\n",entrenador->nombre,entrenador->victorias);

	for(j = 0; j < cant_pokemones ;j++){

		printf("%s;%i;%i;%i;%i;%i\n",entrenador->equipo[j]->nombre,entrenador->equipo[j]->nivel,entrenador->equipo[j]->defensa,entrenador->equipo[j]->fuerza,entrenador->equipo[j]->inteligencia,entrenador->equipo[j]->velocidad);
	}

}

void salon_destruir(salon_t* salon){

	if(!salon){
		return;
	}

	
	size_t cantidad_entrendadores = vtrlen(salon->entrenadores);

	for(size_t i = 0; i < cantidad_entrendadores;i++){

		vtrfree(salon->entrenadores[i]->equipo);
		free(salon->entrenadores[i]);

	}

	free(salon->entrenadores);

	free(salon);
	

}
