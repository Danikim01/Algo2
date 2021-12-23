#include "hospital.h"
#include "hospital_aux.h"

#define ABB_RECORRER_INORDEN   0
#define ABB_RECORRER_PREORDEN  1
#define ABB_RECORRER_POSTORDEN 2

size_t hospital_cantidad_pokemon(hospital_t* hospital){
    if(!hospital){
        return 0;
    }
    return (size_t)hospital->pokemon_totales;
}

size_t hospital_cantidad_entrenadores(hospital_t* hospital){
    if(!hospital){
        return 0;
    }
    return (size_t)lista_elementos(hospital->entrenadores_en_sala);
}

int comparador_nombres_pokemones(void* elemento1,void* elemento2){
    if(!elemento1 || !elemento2){
        return 0;
    }
    pokemon_t* pok1 = elemento1;
    pokemon_t* pok2 = elemento2;

    return (strcmp(pok1->nombre,pok2->nombre));
}

void destruir_pokemon(void* elemento){
    pokemon_t* pokemon = elemento;
    free((char*)pokemon->nombre);
    free(pokemon);
}

hospital_t* hospital_crear(){
    hospital_t* hospital = calloc(1,sizeof(hospital_t));
    if(!hospital)
        return NULL;
    hospital->entrenadores_en_sala = lista_crear();
    return hospital;
}

pokemon_t* crear_pokemon(){
    pokemon_t* pokemon = calloc(1,sizeof(pokemon_t));
    if(!pokemon)
        return NULL;
    return pokemon;
}

entrenador_t* entrenador_crear(){
    entrenador_t* entrenador = calloc(1,sizeof(entrenador_t));
    entrenador->arbol_pokemones = arbol_crear(comparador_nombres_pokemones,destruir_pokemon);
    return entrenador;
}

void destruir_entrenador(entrenador_t* entrenador){
    arbol_destruir(entrenador->arbol_pokemones);
    free((char*)entrenador->nombre_entrenador);
    free(entrenador);
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

void procesar_pokemon_y_agregar(hospital_t* hospital,char** linea_spliteada,int cant_elementos,pokemon_t* pokemon,entrenador_t* entrenador){
    pokemon = crear_pokemon();

    for(int i = 0; i < cant_elementos;i++){
            if(i == 0){
                entrenador->id = (int)atoi(linea_spliteada[i]);
            }else if(i == 1){
                entrenador->nombre_entrenador = agregar_texto(entrenador->nombre_entrenador,linea_spliteada[i]);
            }else if(i%2==0){
                pokemon->nombre = agregar_texto(pokemon->nombre,linea_spliteada[i]);
                pokemon->entrenador_del_pokemon = entrenador;
            }else{
                pokemon->nivel = (size_t)atoi(linea_spliteada[i]);
                arbol_insertar(entrenador->arbol_pokemones,pokemon);
                entrenador->cantidad_pokemones += 1;
                hospital->pokemon_totales += 1;
                pokemon = crear_pokemon();
            }  
        }
 
    free(pokemon);
}

bool mostrar_poks(void* elemento,void* extra){
    pokemon_t* pokemon = elemento;
    printf("Nombre:%s\n",pokemon->nombre);
    printf("Nivel:%d\n",pokemon->nivel);
    printf("Dueño:%s\n",((entrenador_t*)pokemon->entrenador_del_pokemon)->nombre_entrenador);
    return false;
}

bool insertar_pokemones_a_arbol_2(void* elemento,void* aux){
    pokemon_t* pok_iterado = elemento;
    abb_t* arbol = aux;
    //printf("El pokemon a insertar es %s\n",pok_iterado->nombre);
    arbol_insertar(arbol,pok_iterado);
    return false;
}

bool insertar_pokemones_a_arbol(void* elemento,void* aux){
    entrenador_t* entrenador_iterado = elemento;
    abb_t* arbol = aux;
    //printf("El entrenador iterado es %s\n",entrenador_iterado->nombre_entrenador);    
    abb_con_cada_elemento(entrenador_iterado->arbol_pokemones,ABB_RECORRER_INORDEN,insertar_pokemones_a_arbol_2,(void*)arbol);
    return true;
}

bool hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo){
    if(!hospital || !nombre_archivo)
        return false;

    FILE* archivo = fopen(nombre_archivo,"r");
    if(!archivo)
        return false;

    pokemon_t* pokemon = NULL;
    entrenador_t* entrenador = NULL;

    char separador = ';';
    char* linea_leida = fgets_alloc(archivo);
    if (linea_leida){
        hospital->texto_leido = agregar_texto(hospital->texto_leido,linea_leida);
    }
    char** linea_spliteada = split(linea_leida,separador);

    while(linea_leida != NULL){
        int cant_elementos = (int)vtrlen(linea_spliteada);
        entrenador = entrenador_crear();
        procesar_pokemon_y_agregar(hospital,linea_spliteada,cant_elementos,pokemon,entrenador);
            
        //if(entrenador){
        //    printf("El nombre del entrenador cuyo id es:%d, es:%s\n",entrenador->id,entrenador->nombre_entrenador);
        //    printf("Cantidad pokemones:%d\n",entrenador->cantidad_pokemones);
        //    printf("El equipo del entrenador es el siguiente\n"); 
        //    abb_con_cada_elemento(entrenador->arbol_pokemones,ABB_RECORRER_INORDEN,mostrar_poks,NULL);
        //}

        lista_insertar(hospital->entrenadores_en_sala,entrenador);
        hospital->entrenadores_totales += 1;

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

/*
size_t hospital_cantidad_pokemon(hospital_t* hospital){
    if(!hospital){
        return  0;
    }
    return hospital->pokemon_totales;
}

size_t hospital_cantidad_entrenadores(hospital_t* hospital){
    if(!hospital){
        return  0;
    }
    return hospital->entrenadores_totales;
}
*/

entrenador_t* pokemon_entrenador(pokemon_t* pokemon){
    if(!pokemon){
        return NULL;
    }
    return pokemon->entrenador_del_pokemon;
}

//////////////////////////////////////////////////////

typedef bool (*arbol_aplicar_funcion)(pokemon_t* p);

typedef struct contexto{
    arbol_aplicar_funcion funcion;
}contexto_t;

bool aplicar_f_inorder(void* elemento,void* aux){
    pokemon_t* pok_iterado = elemento;
    contexto_t* ctx = aux;
    if (!ctx->funcion(pok_iterado)){
        return true;
    }
    return false;
}

/**
 * Aplica una función a cada uno de los pokemon almacenados en el hospital. La
 * función debe aplicarse a cada pokemon en orden alfabético.
 *
 * La función a aplicar recibe el pokemon y devuelve true o false. Si la función
 * devuelve true, se debe seguir aplicando la función a los próximos pokemon si
 * quedan. Si la función devuelve false, no se debe continuar.
 *
 * Devuelve la cantidad de pokemon a los que se les aplicó la función (hayan devuelto true o false).
 */
size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)){
    if(!hospital || !funcion){
        return 0;
    }
    size_t recorridos = 0;
    contexto_t contexto = {funcion};
    
    abb_t* arbol_auxiliar = arbol_crear(comparador_nombres_pokemones,NULL);
    lista_con_cada_elemento(hospital->entrenadores_en_sala,insertar_pokemones_a_arbol,(void*)arbol_auxiliar);
    abb_con_cada_elemento(arbol_auxiliar,ABB_RECORRER_INORDEN,mostrar_poks,NULL);
    recorridos += abb_con_cada_elemento(arbol_auxiliar,ABB_RECORRER_INORDEN,aplicar_f_inorder,(void*)&contexto);
    arbol_destruir(arbol_auxiliar);



    return recorridos;
}

bool destruir_cada_entrenador(void* elemento,void* extra){
    entrenador_t* entrenador = elemento;
    destruir_entrenador(entrenador);
    return true;
}

void hospital_destruir(hospital_t* hospital){
    if (!hospital){
        return;
    }
    lista_con_cada_elemento(hospital->entrenadores_en_sala,destruir_cada_entrenador,NULL);
    lista_destruir(hospital->entrenadores_en_sala);
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

bool hospital_guardar_archivo(hospital_t* hospital, const char* nombre_archivo){
    if (!hospital){
        return false;
    }
    FILE* archivo_guardar = fopen(nombre_archivo,"w");
    if (!archivo_guardar){
        return false;
    }
    printf("%s\n",hospital->texto_leido);
    fprintf(archivo_guardar,"%s\n",hospital->texto_leido);
    fclose(archivo_guardar);
    return true;
}
