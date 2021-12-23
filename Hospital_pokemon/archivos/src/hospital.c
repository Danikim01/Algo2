#include "hospital.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>
struct _hospital_pkm_t{
    size_t cantidad_pokemon;
    pokemon_t** vector_pokemones;
    size_t cantidad_entrenadores;
    char* texto_leido;
};

struct _pkm_t{
    char* nombre;
    size_t nivel;
};

hospital_t* hospital_crear(){
    hospital_t* hospital = calloc(1,sizeof(hospital_t));
    if(!hospital)
        return NULL;
    //hospital->texto_leido = calloc(1,sizeof(char)*500);
    //if(!hospital->texto_leido){
    //    free(hospital);
    //    return NULL;
    //}
    return hospital;
}

pokemon_t* crear_pokemon(){
    pokemon_t* pokemon = calloc(1,sizeof(pokemon_t));
    if(!pokemon)
        return NULL;
    return pokemon;
}

char* agregar_texto(char* string_original,char* string_a_agregar){
    size_t largo = 0;
    if(string_original != NULL){
        largo = strlen(string_original);
    }
    char* str = realloc(string_original,largo+strlen(string_a_agregar)+1);
    if(!str){
        return NULL;
    }
    string_original = str;
    strcpy(string_original+largo,string_a_agregar);
    return string_original;

}

void procesar_pokemon_y_agregar(hospital_t* hospital,char** linea_spliteada,int cant_elementos,pokemon_t* pokemon){
    pokemon = crear_pokemon();
    for(int i = 2; i < cant_elementos;i++){
            if(i%2==0){
                pokemon->nombre = agregar_texto(pokemon->nombre,linea_spliteada[i]);
            }else{
                pokemon->nivel = (size_t)atoi(linea_spliteada[i]);
                hospital->vector_pokemones = vtradd(hospital->vector_pokemones,pokemon);
                hospital->cantidad_pokemon += 1;
                pokemon = crear_pokemon();
            }  
        }
    free(pokemon);
}

bool hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo){
    if(!hospital || !nombre_archivo)
        return false;

    FILE* archivo = fopen(nombre_archivo,"r");
    if(!archivo)
        return false;

    pokemon_t* pokemon = NULL;
    char separador = ';';
    char* linea_leida = fgets_alloc(archivo);
    if (linea_leida){
        hospital->texto_leido = agregar_texto(hospital->texto_leido,linea_leida);
    }
    char** linea_spliteada = split(linea_leida,separador);

    while(linea_leida != NULL){
        int cant_elementos = (int)vtrlen(linea_spliteada);
        procesar_pokemon_y_agregar(hospital,linea_spliteada,cant_elementos,pokemon);
        hospital->cantidad_entrenadores += 1;
        free(linea_leida);
        vtrfree(linea_spliteada);

        linea_leida = fgets_alloc(archivo);
        if (linea_leida){
            hospital->texto_leido = agregar_texto(hospital->texto_leido,linea_leida);
        }
        linea_spliteada = split(linea_leida,separador);
    }

    fclosen(archivo);
    return true;
}

size_t hospital_cantidad_pokemon(hospital_t* hospital){
    if(!hospital){
        return 0;
    }
    return (hospital->cantidad_pokemon);
}

size_t hospital_cantidad_entrenadores(hospital_t* hospital){
    if(!hospital){
        return 0;
    }
    return (hospital->cantidad_entrenadores);
}

void ordenar_vector(pokemon_t** vector_pok,size_t tope){
    pokemon_t* aux;
    for(size_t i = 0; i < tope;i++){
        for(size_t j = 0; j < tope-i-1;j++){
            if(strcmp(vector_pok[j]->nombre,vector_pok[j+1]->nombre)>0){
                aux = vector_pok[j];
                vector_pok[j] = vector_pok[j+1];
                vector_pok[j+1] = aux;
            }  
        }
    }
}

size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)){
    if(!hospital || !funcion)
        return 0;
    size_t retorno = 0;

    ordenar_vector(hospital->vector_pokemones,hospital->cantidad_pokemon);

    for(int i = 0; i < hospital->cantidad_pokemon;i++){
        if(!funcion(hospital->vector_pokemones[i])){
            return retorno;
        }
        retorno += 1;
    }
    
    return retorno;
}

void liberar_vector_pokemones(pokemon_t** vector,size_t tope){
    for(int i = 0; i < tope;i++){
        free(vector[i]->nombre);
        free(vector[i]);
    }
    free(vector);
}

void hospital_destruir(hospital_t* hospital){
    if(!hospital){
        return;
    }
    liberar_vector_pokemones(hospital->vector_pokemones,hospital->cantidad_pokemon);
    free((char*)hospital->texto_leido);
    free(hospital);
}

size_t pokemon_nivel(pokemon_t* pokemon){
    if(!pokemon){
        return 0;
    }
    return pokemon->nivel;
}

const char* pokemon_nombre(pokemon_t* pokemon){
    if(!pokemon){
        return NULL;
    }
    return pokemon->nombre;
}

/**
 * Guarda un archivo con el nombre dado. El formato del archivo debe ser igual
 * al del caso de lectura.
 *
 * Devuelve true si pudo guardarlo o false en caso de error.
 */
bool hospital_guardar_archivo(hospital_t* hospital, const char* nombre_archivo){
    if (!hospital){
        return false;
    }
    FILE* archivo_guardar = fopen(nombre_archivo,"w");
    if (!archivo_guardar){
        return false;
    }
    //printf("%s\n",hospital->texto_leido);
    fprintf(archivo_guardar,"%s\n",hospital->texto_leido);
    fclose(archivo_guardar);
    return true;
}