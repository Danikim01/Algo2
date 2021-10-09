#include "salon.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
* Pre condiciones : La funcion recibe un entrenador inicializado en NULL, un pokemon que corresponde a ese entrenador tambien vacio y el salon que viene inicializado 
* (en caso de que el salon sea vacio, la funcion no hace nada).
* Post condiciones : La funcion inicializa el entrenador recibido tal que tenga 5 victorias y tambien inicializ sus pokemones, por otra parte, la funcion
* tambien se encarga de agregar el entrenador inicializado en todos sus campos al salon tras haber agregado el equipo pokemon inicializado al entrenador.
*/

void inicializar_y_agregar_entrenador_5_victorias(entrenador_t* entrenador_5_victorias,pokemon_t* pokemon,salon_t* salon){

    if(!entrenador_5_victorias || !pokemon || !salon)
        return;

    strcpy(entrenador_5_victorias->nombre,"Entrenador1");
    entrenador_5_victorias->victorias = 5;
    strcpy(pokemon->nombre,"Pokemon1");
    pokemon->nivel = 4;
    pokemon->fuerza = 3;
    pokemon->inteligencia = 3;
    pokemon->defensa = 4;
    pokemon->velocidad =3;


    entrenador_5_victorias->equipo = vtradd(entrenador_5_victorias->equipo,pokemon);

    salon = salon_agregar_entrenador(salon,entrenador_5_victorias);
   
}

/*
* Pre condiciones : La funcion recibe un entrenador inicializado en NULL, un pokemon que corresponde a ese entrenador tambien vacio y el salon que viene inicializado 
* (en caso de que el salon sea vacio, la funcion no hace nada).
* Post condiciones : La funcion inicializa el entrenador recibido tal que tenga 7 victorias y tambien inicializa sus pokemones, por otra parte, la funcion
* tambien se encarga de agregar el entrenador inicializado en todos sus campos al salon tras haber agregado el equipo pokemon inicializado al entrenador.
*/

void inicializar_y_agregar_entrenador_7_victorias(entrenador_t* entrenador_7_victorias, pokemon_t* pokemon_2,salon_t* salon){

    if(!entrenador_7_victorias || !pokemon_2 || !salon){
        return;
    }

    strcpy(entrenador_7_victorias->nombre,"Entrenador2");
    entrenador_7_victorias->victorias = 7;
    strcpy(pokemon_2->nombre,"Pokemon2");
    pokemon_2->nivel = 4;
    pokemon_2->fuerza = 3;
    pokemon_2->inteligencia = 3;
    pokemon_2->defensa = 4;
    pokemon_2->velocidad =3;


    entrenador_7_victorias->equipo = vtradd(entrenador_7_victorias->equipo,pokemon_2);
    
    salon = salon_agregar_entrenador(salon,entrenador_7_victorias);
}


int main(){

	salon_t* salon;


    printf("Crear un salon usando el archivo 'salon_original.sal'\n");

    const char* nombre_archivo = "salon_original.sal";

    salon = salon_leer_archivo(nombre_archivo);

    if(!salon){
    	return -1;
    }

    printf("\nObtener los entrenadores con al menos 3 ligas ganadas y mostrarlos por pantalla.\n");

    entrenador_t** entrenador_3_ligas_ganadas = salon_obtener_entrenadores_mas_ganadores(salon,3);

    if(!entrenador_3_ligas_ganadas){
        return -1;
    }

    size_t cant_ganadores_3_ligas = vtrlen(entrenador_3_ligas_ganadas);

    for(size_t j = 0; j < cant_ganadores_3_ligas;j++){

        salon_mostrar_entrenador(entrenador_3_ligas_ganadas[j]);
    }

    printf("\nAgregar 2 entrenadores al salon con 5 y 7 victorias respectivamente.\n");

    entrenador_t* entrenador_5_victorias = NULL;
    pokemon_t* pokemon = NULL;

    entrenador_5_victorias = calloc(1,sizeof(entrenador_t));

    if(!entrenador_5_victorias)
        return -1;

    pokemon = calloc(1,sizeof(pokemon_t));

    if(!pokemon)
        return -1;

    inicializar_y_agregar_entrenador_5_victorias(entrenador_5_victorias,pokemon,salon);


    entrenador_t* entrenador_7_victorias = NULL;
    pokemon_t* pokemon_2 = NULL;

    entrenador_7_victorias = calloc(1,sizeof(entrenador_t));

    if(!entrenador_7_victorias)
        return -1;

    pokemon_2 = calloc(1,sizeof(pokemon_t));

    if(!pokemon_2)
        return -1;

    inicializar_y_agregar_entrenador_7_victorias(entrenador_7_victorias,pokemon_2,salon);


    printf("\nObtener los entrenadores con al menos 5 ligas ganadas y mostrarlos por pantalla.\n");

    entrenador_t** entrenador_5_ligas_ganadas = salon_obtener_entrenadores_mas_ganadores(salon,5);

    if(!entrenador_5_ligas_ganadas){
    	return -1;
    }

    size_t cant_ganadores_5_ligas = vtrlen(entrenador_5_ligas_ganadas);

    for(size_t j = 0; j < cant_ganadores_5_ligas;j++){

        salon_mostrar_entrenador(entrenador_5_ligas_ganadas[j]);
    }
    
    printf("\nGuardar el salon a un nuevo archivo 'salon_modificado.sal'\n");

    int cant_entrenadores_guardados = 0;

    cant_entrenadores_guardados =  salon_guardar_archivo(salon,"salon_modificado.sal");

    if(!cant_entrenadores_guardados){
    	return -1;
    }
    
    free(entrenador_3_ligas_ganadas);
    free(entrenador_5_ligas_ganadas);
    salon_destruir(salon);

    printf("\nSalir con valor de retorno 0\n");
    
    return 0;
}
