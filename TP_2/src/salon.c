#include "salon.h"
#include "util.h"
#include "abb.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_NOMBRE_ENTRENADOR 100
#define MAX_NOMBRE_POKEMON 100
const char SEPARADOR = ';';
const char separador_1 = ':';
const char separador_2 = ',';
#define ABB_RECORRER_INORDEN   0
#define ABB_RECORRER_PREORDEN  1
#define ABB_RECORRER_POSTORDEN 2
#define MAX_REGLAS 10
#define MAX_COMANDOS 9
#define ERROR -1
	
typedef struct pokemon{
	char nombre[MAX_NOMBRE_POKEMON];
	long int nivel;
	long int fuerza;
	long int inteligencia;
	long int defensa;
	long int velocidad;
}pokemon_t;

struct _entrenador_t{
	char nombre[MAX_NOMBRE_ENTRENADOR];
	int victorias;
	lista_t* lista_pokemones; 
	size_t cantidad_pokemones;
};

typedef struct Comando{
	const char* nombre;
	char* (*ejecutar)(salon_t*, const char* linea);
}Comando_t;

struct _salon_t{
    abb_t* entrenadores;
    size_t cantidad_entrenadores;
    hash_t* tabla_reglas;
    hash_t* Comandos;
};


typedef struct contexto{
	const char* nombre_entrenador;
	entrenador_t** entrenador;
	const char* nombre_pokemon;
	pokemon_t** pokemon;
	size_t* posicion;
}contexto_t;

char* listar_entrenadores_del_salon(salon_t* salon,const char* linea);
char* lista_entrenadores_del_salon_con_n_victorias(salon_t* salon,const char* linea);
char* lista_entrenadores_del_salon_con_cierto_pokemon(salon_t* salon,const char* linea);
char* lista_pokemones_de_cierto_entrenador(salon_t* salon,const char* nombre_entrenador);
char* reglas_de_batalla(salon_t* salon, const char* linea);
char* comparar_pokemon_entrenadores(salon_t* salon, const char* linea);
char* agregar_pokemon_a_entrenador(salon_t* salon, const char* linea);
char* quitar_pokemon_a_entrenador(salon_t* salon,const char* linea);
char* guardar_archivo(salon_t* salon,const char* nombre_archivo);

/*
Libera el elemento que le llega por parametro
*/
void destruir_elemento(void* elemento){
  if(elemento){
    free(elemento);
  }
}

/*
Realiza una copia del string (en forma de char*) que le llega por parametro reservando memoria para ello
*/
char* duplicar_string(const char* s){
  if(!s)
    return NULL;

  char* p = malloc(strlen(s)+1);
  strcpy(p,s);
  return p;
}

/*
Reserva memoria para la estructura pokemon_t y la devuelve.
*/
pokemon_t* pokemon_crear(){
	pokemon_t* pokemon = calloc(1,sizeof(pokemon_t));
	if(!pokemon){
		return NULL;
	}
	return pokemon;
}

/*
Libera la memoria reservada para un entrenador_t entrenador.
*/
void entrenador_destruir(entrenador_t* entrenador){
	lista_destruir(entrenador->lista_pokemones);
	free(entrenador);
}

/*
 * Destructor de elementos. Cada vez que un elemento deja el arbol
 * se invoca al destructor pasandole
 * el elemento.
 */
void destructor_de_entrenadores(void* elemento){
    if(!elemento)
        return;
    entrenador_destruir((entrenador_t*)elemento);
}

/*
 * Comparador de elementos. Recibe dos elementos del arbol y devuelve
 * 0 en caso de ser iguales, >0 si el primer elemento es mayor al
 * segundo o <0 si el primer elemento es menor al segundo.
 */
int comparador_entrenadores(void* elemento1,void* elemento2){
	if(!elemento1 || !elemento2){
		return 0;
	}

	entrenador_t* entrenador_1 = elemento1;
	entrenador_t* entrenador_2 = elemento2;

	return (strcmp(entrenador_1->nombre,entrenador_2->nombre));
}

entrenador_t* entrenador_crear(){
	entrenador_t* entrenador = calloc(1,sizeof(entrenador_t));
	if(!entrenador){
		return NULL;
	}
	entrenador->lista_pokemones = lista_crear();
	return entrenador;
}
/*
Reseva la memoria necesaria para la estructura Comando_t Comando y la devuelve.
*/
Comando_t* comando_crear(const char* nombre,char* (*ejecutar)(salon_t*, const char* linea)){
	Comando_t* Comando = calloc(1,sizeof(Comando_t));
	if(!Comando) return NULL;
	Comando->nombre = nombre;
	Comando->ejecutar = ejecutar;
	return Comando;
}

/*
Agrega un comando a la tabla de hash de comandos, devuelve retorno en caso de ejecucion correcta,ERROR en caso contrario 
*/
int agregar_comando(hash_t* Comandos,const char* nombre,char* (*ejecutar)(salon_t*, const char* linea)){
	Comando_t* Comando = comando_crear(nombre,ejecutar);
	int retorno = hash_insertar(Comandos,nombre,Comando);
	if(retorno == ERROR) return ERROR;
	return retorno;
}
/*
Reserva la memoria necesaria para crear un salon_t salon, devuelve NULL si no se pudo.
*/
salon_t* salon_crear(){
	salon_t* salon = calloc(1,sizeof(salon_t));
	if(!salon)
		return NULL;
	salon->entrenadores = arbol_crear(comparador_entrenadores,destructor_de_entrenadores);
	salon->tabla_reglas = hash_crear(destruir_elemento,MAX_REGLAS);
	salon->Comandos = hash_crear(destruir_elemento,MAX_COMANDOS);
	int retorno_comando_1 = agregar_comando(salon->Comandos,"ENTRENADORES",listar_entrenadores_del_salon);
	int retorno_comando_2 = agregar_comando(salon->Comandos,"ENTRENADORES_VICTORIAS",lista_entrenadores_del_salon_con_n_victorias);
	int retorno_comando_3 = agregar_comando(salon->Comandos,"ENTRENADORES_POKEMON",lista_entrenadores_del_salon_con_cierto_pokemon);
	int retorno_comando_4 = agregar_comando(salon->Comandos,"ENTRENADORES_EQUIPO",lista_pokemones_de_cierto_entrenador);
	int retorno_comando_5 = agregar_comando(salon->Comandos,"REGLAS",reglas_de_batalla);
	int retorno_comando_6 = agregar_comando(salon->Comandos,"COMPARAR_ENTRENADORES",comparar_pokemon_entrenadores);
	int retorno_comando_7 = agregar_comando(salon->Comandos,"AGREGAR_POKEMON",agregar_pokemon_a_entrenador);
	int retorno_comando_8 = agregar_comando(salon->Comandos,"QUITAR_POKEMON",quitar_pokemon_a_entrenador);
	int retorno_comando_9 = agregar_comando(salon->Comandos,"GUARDAR",guardar_archivo);
	int retorno_1 = hash_insertar(salon->tabla_reglas,"CLASICO",duplicar_string(",0.8*nivel+fuerza+2*velocidad\n"));
	int retorno_2 = hash_insertar(salon->tabla_reglas,"MODERNO",duplicar_string(",0.5*nivel+0.9*defensa+3*inteligencia\n"));
	int retorno_3 = hash_insertar(salon->tabla_reglas,"Caracter",duplicar_string(",el pokemon ganador es aquel cuya suma de los caracteres de su nombre sea mayor\n"));
	int retorno_4 = hash_insertar(salon->tabla_reglas,"Largo",duplicar_string(",el pokemon ganador es aquel cuya longuitud de los caracteres de su nombre sea mayor\n"));
	int retorno_5 = hash_insertar(salon->tabla_reglas,"Nivel",duplicar_string(",el pokemon ganador es aquel cuyo nivel sea mayor\n"));
	if(retorno_1 == ERROR || retorno_2 == ERROR || retorno_3 == ERROR || retorno_4 == ERROR || retorno_5 == ERROR){
		return NULL;
	}
	if(retorno_comando_1 == ERROR || retorno_comando_2 == ERROR || retorno_comando_3 == ERROR || retorno_comando_4 == ERROR || retorno_comando_5 == ERROR || retorno_comando_6 == ERROR || retorno_comando_7 == ERROR || retorno_comando_8 == ERROR || retorno_comando_9 == ERROR){
		return NULL;
	}
	return salon;
}

/*
Pre condiciones : La funcion recibe un entrenador_t entrenador cuyo espacio en memoria dinamica fue reservado previamente (en este caso mediante un calloc lo cual llena de ceros los valores en memoria reservada)
y recibe, a su vez, el puntero a un puntero a char de elementos inicializados, los cuales corresponden al nombre y a la cantidad de victorias del entrenador a inicializar.
Post condiciones : La funcion inicializa del struct entrenador_t* el nombre del entrenador y la cantidad de victorias.
*/

void inicializar_nombre_y_victorias_entrenador(entrenador_t* entrenador, char** elementos){

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
    pokemon->nivel = (long int)atoi(elementos[1]);
    pokemon->fuerza = (long int)atoi(elementos[3]);
    pokemon->inteligencia = (long int)atoi(elementos[4]);
    pokemon->defensa = (long int)atoi(elementos[2]);
    pokemon->velocidad = (long int)atoi(elementos[5]);

}
/*
Agrega un entrenador_t* nuevo_entrenador al arbol de entenadores.Devuelve ERROR si no se pudo realizar la insercion.
*/
int agregar_entrenador(salon_t* salon, entrenador_t* nuevo_entrenador){
    
    if(!salon){
        return ERROR;
    }

    int exito_abb = arbol_insertar(salon->entrenadores, nuevo_entrenador);
    if(exito_abb == ERROR){
        return ERROR;
    }
   
    return 0;
}

int victorias_entrenador(entrenador_t* entrenador){
	return (entrenador->victorias);
}

char* nombre_entrenador(entrenador_t* entrenador){
	return (entrenador->nombre);
}

lista_t* lista_pokemones(entrenador_t* entrenador){
	return (entrenador->lista_pokemones);
}

bool entrenador_tiene_pokemon(lista_t* lista_pokemones,const char* nombre_pokemon){

	lista_iterador_t* it = NULL;
	for(it = lista_iterador_crear(lista_pokemones); lista_iterador_tiene_siguiente(it); lista_iterador_avanzar(it)){

		if(it){
			if(strcmp(((pokemon_t*)lista_iterador_elemento_actual(it))->nombre,nombre_pokemon) == 0){
				lista_iterador_destruir(it);
				return false;
			}
		} else {
			lista_iterador_destruir(it);
			return false;
		}
			
	}

	lista_iterador_destruir(it);
    return true;
}

size_t cantidad_pokemones_entrenador(entrenador_t* entrenador){
	return (entrenador->cantidad_pokemones);
}
/*
Recibe un string reservando mas memoria para el nuevo string recibido, para finalmente concatenarlo y devolver un nuevo string concatenado.
*/
char* agregar_texto(char* string,char* texto){
	size_t tamanio_actual = 0;
	if(string != NULL){
		tamanio_actual = strlen(string);
	}
	char* nuevo_string = realloc(string,tamanio_actual+strlen(texto)+1);
	if(!nuevo_string){
		return NULL;
	}
	string = nuevo_string;
	strcpy(string+tamanio_actual,texto);
	return string;
}
/*
Recorre el arbol de forma preorden desde el nodo raiz del arbol de entrenadores y se corta cuando algun
entrenador del mismo coincida con el nombre enviado por parametro. 
*/
int existe_entrenador_en_salon(nodo_abb_t* nodo_actual, char* nombre){
	if(!nodo_actual){
		return 1;
	}

	if(nodo_actual && strcmp(((entrenador_t*)nodo_actual->elemento)->nombre,nombre) == 0){
       	return 0;
    }  

    if(!existe_entrenador_en_salon(nodo_actual->izquierda,nombre)){
    	return 0;
    }
    
    if(!existe_entrenador_en_salon(nodo_actual->derecha,nombre)){
    	return 0;
    }
    

    return 1;
}
/*
Recorre el arbol de forma preorden desde el nodo raiz del arbol de entrenadores y se corta cuando algun
entrenador del mismo no tenga una lista vacia de pokemones.
*/
int entrenador_no_tiene_equipo(nodo_abb_t* nodo_actual){
	if(!nodo_actual)
		return 1;
	if(nodo_actual){
		if(lista_vacia(((entrenador_t*)nodo_actual->elemento)->lista_pokemones)){
			return 0;
		}
	}
	
	if(!entrenador_no_tiene_equipo(nodo_actual->izquierda)){
		return 0;
	}
	
	if(!entrenador_no_tiene_equipo(nodo_actual->derecha)){
		return 0;
	}
	return 1;
}


salon_t* salon_leer_archivo(const char* nombre_archivo){
    FILE *archivo = fopen(nombre_archivo,"r");
	if(!archivo){
		return NULL;
	}

	pokemon_t* pokemon = NULL;
  	entrenador_t* entrenador = NULL;
 	salon_t* salon = salon_crear();
 	if(!salon){
 		return NULL;
 	}

	char separador = SEPARADOR;
	char* linea_leida = fgets_alloc(archivo);
	char** linea_spliteada = split(linea_leida,separador);
	size_t tamanio_linea = vtrlen(linea_spliteada);

	 while(linea_leida != NULL){
        entrenador = entrenador_crear();
        if(!entrenador){
        	free(linea_leida);
        	vtrfree(linea_spliteada);
        	fclosen(archivo);
        	return NULL;
        } 
        	
		inicializar_nombre_y_victorias_entrenador(entrenador,linea_spliteada);
		if(existe_entrenador_en_salon(salon->entrenadores->nodo_raiz,entrenador->nombre) == 0){
			free(linea_leida);
	        vtrfree(linea_spliteada);
	        entrenador_destruir(entrenador);
	        fclosen(archivo);
	        salon_destruir(salon);
	        return NULL;
		}
        free(linea_leida);
        vtrfree(linea_spliteada);

        linea_leida = fgets_alloc(archivo);
        linea_spliteada = split(linea_leida,separador);
        tamanio_linea = vtrlen(linea_spliteada);

        while(tamanio_linea != 2 && linea_leida != NULL){

           	pokemon = pokemon_crear();
           	if(!pokemon){
           		free(linea_leida);
	        	vtrfree(linea_spliteada);
	        	free(pokemon);
	        	fclosen(archivo);
	        	return NULL;
           	}
			inicializar_pokemon(pokemon,linea_spliteada);	

			int insercion = lista_insertar(entrenador->lista_pokemones,pokemon);

			if(insercion == ERROR || lista_vacia(entrenador->lista_pokemones)){
				free(linea_leida);
				free(pokemon);
	        	vtrfree(linea_spliteada);
	        	fclosen(archivo);
	        	salon_destruir(salon);
				return NULL;
			}
			entrenador->cantidad_pokemones += 1;

            free(linea_leida);
        	vtrfree(linea_spliteada);

            linea_leida = fgets_alloc(archivo);
            linea_spliteada = split(linea_leida,separador);
            tamanio_linea = vtrlen(linea_spliteada);

        }
        
        int agregado_abb = agregar_entrenador(salon,entrenador);
        if(agregado_abb == ERROR){
        	free(linea_leida);
	        vtrfree(linea_spliteada);
	        fclosen(archivo);
	        salon_destruir(salon);
            return NULL;
        }
        salon->cantidad_entrenadores += 1;
    }

	if(arbol_vacio(salon->entrenadores) || entrenador_no_tiene_equipo(salon->entrenadores->nodo_raiz) == 0){
		free(linea_leida);
        vtrfree(linea_spliteada);
        fclosen(archivo);
        salon_destruir(salon);
        return NULL;
	}
	
	fclosen(archivo);

	return salon;
}
/*
Apendea la informacion de cada uno de los elementos enviados por parametro (que corresponden a elementos del arbol de entrenadores) 
a un archivo enviado por parametro.
*/
bool recorrer_arbol_y_guardar(void* elemento, void* extra){
	if(!elemento)
		return true;
	FILE* archivo;
	const char* nombre_archivo = (char*)extra;
	archivo = fopen(nombre_archivo,"a");
	entrenador_t* entrenador_aux = ((entrenador_t*)elemento);
	
	fprintf(archivo,"%s;%i\n",entrenador_aux->nombre,entrenador_aux->victorias);

	for (size_t i = 0; i < lista_elementos(entrenador_aux->lista_pokemones); i++){
        pokemon_t* pokemon = lista_elemento_en_posicion(entrenador_aux->lista_pokemones, i);
        fprintf(archivo, "%s;%li;%li;%li;%li;%li\n",pokemon->nombre,pokemon->nivel,pokemon->defensa,pokemon->fuerza,pokemon->inteligencia,pokemon->velocidad);
    }

	fclose(archivo);
	return false;
	
}

int salon_guardar_archivo(salon_t* salon, const char* nombre_archivo){
	FILE* archivo_guardar = fopen(nombre_archivo,"wt");
	if(!archivo_guardar){
		return ERROR;
	}

	int cant_entrenadores = 0;

	cant_entrenadores = (int)(abb_con_cada_elemento(salon->entrenadores,ABB_RECORRER_PREORDEN,recorrer_arbol_y_guardar,(void*)nombre_archivo));
		
	fclose(archivo_guardar);
	return cant_entrenadores;

}

salon_t* salon_agregar_entrenador(salon_t* salon, entrenador_t* entrenador){
	if(!salon || !entrenador)
		return NULL;
    
	int agregado_abb = agregar_entrenador(salon,entrenador);
	if(agregado_abb == ERROR){
		return NULL;
	}

    return salon;
}
/*
 * Inserta a una lista (enviada por parametro) aquellos entrenadores que cumplen con la función de filtro provista.
 * Mediante un recorrido inorden del arbol.
 */
void abb_recorrido_inorden(nodo_abb_t* nodo_actual,lista_t* lista, bool (*f)(entrenador_t*, void*), void* extra){
	if(!nodo_actual|| !lista){
        return;
    }

    if(nodo_actual->izquierda){
        abb_recorrido_inorden(nodo_actual->izquierda,lista,f,extra);
    }


    if(nodo_actual && f(((entrenador_t*)nodo_actual->elemento),extra)){
       	int insercion = lista_insertar(lista,((entrenador_t*)nodo_actual->elemento));
       	if(insercion == ERROR){
       		return;
       	}
    }  

    if(nodo_actual->derecha){
        abb_recorrido_inorden(nodo_actual->derecha,lista,f,extra);
    }
}

lista_t* salon_filtrar_entrenadores(salon_t* salon , bool (*f)(entrenador_t*, void*), void* extra){
	if(!salon || !f){
		return NULL;
	}
	lista_t* lista_entrenadores = lista_crear();
	if(lista_entrenadores){
		abb_recorrido_inorden(salon->entrenadores->nodo_raiz,lista_entrenadores,f,extra);
	}else{ 
		return NULL;
	}
	
	return lista_entrenadores;
    
}

//Devuelve true en caso de que haya entrenador (no sea NULL), caso contrario devuelve false.
bool es_entrenador(entrenador_t* entrenador,void* extra){
	extra=extra;
	return (entrenador != NULL);
}
/*
Devuelve un string con cada uno de los entrenadores del salon junto a sus victorias. En caso de error
devuelve NULL.
*/
char* listar_entrenadores_del_salon(salon_t* salon,const char* linea){
	char* string = NULL;
	char string_aux[100];
	lista_t* lista = salon_filtrar_entrenadores(salon,es_entrenador,NULL);
	if(lista_vacia(lista)){
		lista_destruir(lista);
		return NULL;
	}
	while(!lista_vacia(lista)){
		string = agregar_texto(string,((entrenador_t*)lista_primero(lista))->nombre);
		string = agregar_texto(string,",");
		sprintf(string_aux,"%d",((entrenador_t*)lista_primero(lista))->victorias);
		string = agregar_texto(string,string_aux);
		string = agregar_texto(string,"\n");
		lista_desencolar(lista);
	}
	lista_destruir(lista);
	return string;
}

/*
Devuelve true si el entrenador enviado por parametro tiene una cantidad de victorias mayor o igual a la enviada por parametro.
Caso contrario devuelve false.
*/
bool es_entrenador_con_n_victorias(entrenador_t* entrenador,void* extra){
	size_t victorias_pedidas = (size_t)extra;
	return (entrenador->victorias >= victorias_pedidas);
}	
/*
Devuelve un string con los nombres de los entrenadores que cumplan con la condicion que establezca la linea enviada por parametro.
En este caso la linea establece la cantidad de victorias que tiene que tener un entrenador.
Para lo cual, se splitea la linea para procesar cuantos entrenadores tienen n victorias o mas.
*/
char* lista_entrenadores_del_salon_con_n_victorias(salon_t* salon,const char* linea){
	if(!salon)
		return NULL;
	char** linea_sub_spliteada = split(linea,separador_2);
	if(!linea_sub_spliteada || (strcmp(linea_sub_spliteada[0],"victorias") != 0) || vtrlen(linea_sub_spliteada) != 2){
		vtrfree(linea_sub_spliteada);
		return NULL;
	}

	char* string = NULL;
	size_t victorias_pedidas = (size_t)(atoi(linea_sub_spliteada[1]));
	lista_t* lista = salon_filtrar_entrenadores(salon,es_entrenador_con_n_victorias,(void*)victorias_pedidas);
	if(lista_vacia(lista)){
		vtrfree(linea_sub_spliteada);
		lista_destruir(lista);
		string = agregar_texto(string,"\0");
		return string;
	}
	while(!lista_vacia(lista)){
		string = agregar_texto(string,((entrenador_t*)lista_primero(lista))->nombre);
		string = agregar_texto(string,"\n");
		lista_desencolar(lista);
	}

	vtrfree(linea_sub_spliteada);
	lista_destruir(lista);
	return string;
}
/*
Devuelve true si el entrenador tiene dentro de su lista de pokemones algun pokemon enviado por parametro. 
En caso contrario o de error devuelve false.
*/
bool es_entrenador_con_cierto_pokemon(entrenador_t* entrenador,void* extra){
	if(!entrenador){
		return false;
	}
	const char* nombre_pokemon = (const char*)extra;
	bool tiene_pokemon = entrenador_tiene_pokemon(entrenador->lista_pokemones,nombre_pokemon);
	return (!tiene_pokemon);
}
/*
Devuelve un string con los nombres de los entrenadores que cumplan con la condicion que establezca la linea enviada por parametro.
En este caso la linea establece el pokemon que tiene que tener cierto entrenador.
Para lo cual, se splitea la linea para procesar cuantos entrenadores cumplen con la condicion.
*/
char* lista_entrenadores_del_salon_con_cierto_pokemon(salon_t* salon,const char* linea){
	if(!salon)
		return NULL;
	char** linea_sub_spliteada = split(linea,separador_2);
	if((strcmp(linea_sub_spliteada[0],"pokemon") != 0) || vtrlen(linea_sub_spliteada) != 2 ||!linea_sub_spliteada){
		vtrfree(linea_sub_spliteada);
		return NULL;
	}
	char* string = NULL;
	lista_t* lista = salon_filtrar_entrenadores(salon,es_entrenador_con_cierto_pokemon,(void*)linea_sub_spliteada[1]);
	if(lista_vacia(lista)){
		vtrfree(linea_sub_spliteada);
		lista_destruir(lista);
		string = agregar_texto(string,"\0");
		return string;
	}
	while(!lista_vacia(lista)){
		string = agregar_texto(string,((entrenador_t*)lista_primero(lista))->nombre);
		string = agregar_texto(string,"\n");
		lista_desencolar(lista);
	}
	lista_destruir(lista);
	vtrfree(linea_sub_spliteada);
	return string;
}
/*
Devuelve true si el nombre del entrenador coinciden con el enviado por parametro.En caso contrario o de error devuelve false.
*/
bool mismo_nombre_entrenador(entrenador_t* entrenador,void* extra){
	if(strcmp(entrenador->nombre,(char*)extra) == 0){
		return true;
	}
	return false;
}
/*
Recorre la lista de pokemones y devuelve un string con los pokemones y sus atributos.En caso de error devuelve NULL.
*/
char* ingresar_pokemones_a_vector(lista_t* lista_pokemones,char string_pokemones[]){

	char* string_pokemones_2 = NULL;
	lista_iterador_t* it = NULL;
	for(it = lista_iterador_crear(lista_pokemones); lista_iterador_tiene_siguiente(it); lista_iterador_avanzar(it)){
		if(it){
			sprintf(string_pokemones,"%s,%ld,%ld,%ld,%ld,%ld\n",((pokemon_t*)lista_iterador_elemento_actual(it))->nombre,((pokemon_t*)lista_iterador_elemento_actual(it))->nivel,((pokemon_t*)lista_iterador_elemento_actual(it))->defensa,((pokemon_t*)lista_iterador_elemento_actual(it))->fuerza,((pokemon_t*)lista_iterador_elemento_actual(it))->inteligencia,((pokemon_t*)lista_iterador_elemento_actual(it))->velocidad);
			string_pokemones_2 = agregar_texto(string_pokemones_2,string_pokemones);
		} else {
			lista_iterador_destruir(it);
			return NULL;
		}
	}
	lista_iterador_destruir(it);
	return string_pokemones_2;
}

//inicializa un vector poniendo todos sus elemento en 0.
void inicializar_vector(char vector[],int tope){
	for(int i = 0; i < tope;i++){
		vector[i] = 0;
	}
}

/*
Devuelve un string con los pokemones (con sus atributos) de cierto entrenador cuyo nombre se envia por parametro.
Siempre y cuando no haya errores y el entrenador exista en el salon.
En caso de error devuelve NULL.
*/
char* lista_pokemones_de_cierto_entrenador(salon_t* salon,const char* nombre_entrenador){
	char* string = NULL;
	char string_aux[600];
	inicializar_vector(string_aux,600);
	lista_t* lista = salon_filtrar_entrenadores(salon,mismo_nombre_entrenador,(void*)nombre_entrenador);
	if(lista_vacia(lista)){
		lista_destruir(lista);
		return NULL;
	}
	while(!lista_vacia(lista)){
		string = ingresar_pokemones_a_vector(((entrenador_t*)lista_primero(lista))->lista_pokemones,string_aux);
		lista_desencolar(lista);
	}
	lista_destruir(lista);
	return string;
}

/*
Devuelve un string de las reglas de batalla registradas en el salon junto a su respectiva descripcion.
En caso de error devuelve NULL.
*/
char* reglas_de_batalla(salon_t* salon, const char* linea){
	char* string = NULL;
	string = agregar_texto(string,"CLASICO");
	string = agregar_texto(string,(char*)hash_obtener(salon->tabla_reglas,"CLASICO"));
	string = agregar_texto(string,"MODERNO");
	string = agregar_texto(string,(char*)hash_obtener(salon->tabla_reglas,"MODERNO"));
	string = agregar_texto(string,"Caracter");
	string = agregar_texto(string,(char*)hash_obtener(salon->tabla_reglas,"Caracter"));
	string = agregar_texto(string,"Largo");
	string = agregar_texto(string,(char*)hash_obtener(salon->tabla_reglas,"Largo"));
	string = agregar_texto(string,"Nivel");
	string = agregar_texto(string,(char*)hash_obtener(salon->tabla_reglas,"Nivel"));
	return string;
}
/*
Recibe un pokemon y devuelve un double que se calcula siguiendo la regla que establece el modo de batalla CLASICO.
*/
double calcular_coeficiente_de_batalla_clasico(void* elemento){
	if(!(pokemon_t*)elemento){
		return 0.0;
	}
	return (0.8*((double)(((pokemon_t*)elemento)->nivel))+((double)(((pokemon_t*)elemento)->fuerza))+2*((double)(((pokemon_t*)elemento)->velocidad)));
}
/*
Recibe un pokemon y devuelve un double que se calcula siguiendo la regla que establece el modo de batalla MODERNO.
*/
double calcular_coeficiente_de_batalla_moderno(void* elemento){
	if(!(pokemon_t*)elemento) {
		return 0.0;
	}
	return (0.5*((double)(((pokemon_t*)elemento)->nivel))+0.9*((double)(((pokemon_t*)elemento)->defensa))+3*((double)(((pokemon_t*)elemento)->inteligencia)));
}


//Devuelve la suma de los valores de cada uno de los caracteres correspondientes al string enviado por parametro.
int calcular_suma_de_caracteres(const char* k){
	int valor = 0;
	while(*(k)){
		valor += *(k);
		(k)++;
	}
	return valor;
}

//Devuelve la longuitud del string enviado por parametro.
size_t calcular_largo_caracter(const char* k){
	return (strlen(k));
}

char* output_combates(entrenador_t* entrenador_1, entrenador_t* entrenador_2,const char* regla){
	char* string = NULL;
	double coeficiente_1 = 0.0,coeficiente_2 = 0.0;
	int caracteres_1 = 0,caracteres_2 = 0;
	size_t caracter_1 = 0,caracter_2 = 0;
	long int nivel_1 = 0,nivel_2 = 0;
	lista_iterador_t* it_1 = lista_iterador_crear(entrenador_1->lista_pokemones);
	lista_iterador_t* it_2 = lista_iterador_crear(entrenador_2->lista_pokemones);

	while(lista_iterador_tiene_siguiente(it_1) && lista_iterador_tiene_siguiente(it_2)){
		if(strcmp(regla,"CLASICO") == 0){
			coeficiente_1 = calcular_coeficiente_de_batalla_clasico(lista_iterador_elemento_actual(it_1));
			coeficiente_2 = calcular_coeficiente_de_batalla_clasico(lista_iterador_elemento_actual(it_2));
			if(coeficiente_1 > coeficiente_2){
				lista_iterador_avanzar(it_2);
				string = agregar_texto(string,"1\n");
			}else if(coeficiente_1 < coeficiente_2){
				lista_iterador_avanzar(it_1);
				string = agregar_texto(string,"2\n");
			}else{
				lista_iterador_avanzar(it_2);
				string = agregar_texto(string,"1\n");
			}
		}else if(strcmp(regla,"MODERNO")==0){
			coeficiente_1 = calcular_coeficiente_de_batalla_moderno(lista_iterador_elemento_actual(it_1));
			coeficiente_2 = calcular_coeficiente_de_batalla_moderno(lista_iterador_elemento_actual(it_2));
			if(coeficiente_1 > coeficiente_2){
				lista_iterador_avanzar(it_2);
				string = agregar_texto(string,"1\n");
			}else if(coeficiente_1 < coeficiente_2){
				lista_iterador_avanzar(it_1);
				string = agregar_texto(string,"2\n");
			}else{
				lista_iterador_avanzar(it_2);
				string = agregar_texto(string,"1\n");
			}
		}else if(strcmp(regla,"Caracter") == 0){
			caracteres_1 = calcular_suma_de_caracteres(lista_iterador_elemento_actual(it_1));
			caracteres_2 = calcular_suma_de_caracteres(lista_iterador_elemento_actual(it_2));
			if(caracteres_1 > caracteres_2){
				lista_iterador_avanzar(it_2);
				string = agregar_texto(string,"1\n");
			}else if(caracteres_1 < caracteres_2){
				lista_iterador_avanzar(it_1);
				string = agregar_texto(string,"2\n");
			}else{
				lista_iterador_avanzar(it_2);
				string = agregar_texto(string,"1\n");
			}
		}else if(strcmp(regla,"Largo")==0){
			caracter_1 = calcular_largo_caracter(((pokemon_t*)lista_iterador_elemento_actual(it_1))->nombre);
			caracter_2 = calcular_largo_caracter(((pokemon_t*)lista_iterador_elemento_actual(it_2))->nombre);
			if(caracter_1 > caracter_2){
				lista_iterador_avanzar(it_2);
				string = agregar_texto(string,"1\n");
			}else if(caracter_1 < caracter_2){
				lista_iterador_avanzar(it_1);
				string = agregar_texto(string,"2\n");
			}else{
				lista_iterador_avanzar(it_2);
				string = agregar_texto(string,"1\n");
			}
		}else if(strcmp(regla,"Nivel")==0){
			nivel_1 = (((pokemon_t*)lista_iterador_elemento_actual(it_1))->nivel);
			nivel_2 = (((pokemon_t*)lista_iterador_elemento_actual(it_2))->nivel);
			if(nivel_1 > nivel_2){
				lista_iterador_avanzar(it_2);
				string = agregar_texto(string,"1\n");
			}else if(nivel_1 < nivel_2){
				lista_iterador_avanzar(it_1);
				string = agregar_texto(string,"2\n");
			}else{
				lista_iterador_avanzar(it_2);
				string = agregar_texto(string,"1\n");
			}
		}
	}
		lista_iterador_destruir(it_1);
		lista_iterador_destruir(it_2);
		return string;
}

/*
Recibe dos entrenadores, la regla de batalla y devuelve un string con los resultados de la batalla de ambos entrenadores.
Dependiendo de la regla de batalla enviada por parametro.En caso de error devuelve NULL.
*/
char* batallar_pokemones(entrenador_t* entrenador_1, entrenador_t* entrenador_2,const char* regla){
	return (output_combates(entrenador_1,entrenador_2,regla));
}
/*
Recibe un salon y una linea a procesar que contiene el nombre de los entrenadores a comparar y la regla de la batalla. 
Dependiendo de la regla de batalla enviada por parametro y el nombre de los entrenadores, se realiza la batalla entre ambos.
Devuelve el string con el resultado de la batalla (o comparacion) entre ambos entrenadores. En caso de error devuelve NULL.
*/
char* comparar_pokemon_entrenadores(salon_t* salon, const char* linea){
		
	char** linea_sub_spliteada = split(linea,separador_2);
	if(!linea_sub_spliteada || vtrlen(linea_sub_spliteada) != 3){
		vtrfree(linea_sub_spliteada);
		return NULL;
	}

	entrenador_t* entrenador_1 = arbol_buscar(salon->entrenadores,(char*)linea_sub_spliteada[0]);
	entrenador_t* entrenador_2 = arbol_buscar(salon->entrenadores,(char*)linea_sub_spliteada[1]);

	if(!salon || !entrenador_1  || !entrenador_2 || hash_obtener(salon->tabla_reglas,linea_sub_spliteada[2]) == NULL){
		vtrfree(linea_sub_spliteada);
		return NULL;
	}
	char* string = NULL;
	
	string = batallar_pokemones(entrenador_1,entrenador_2,linea_sub_spliteada[2]);

	vtrfree(linea_sub_spliteada);
	return string;
}
/*
Recorre el arbol de entrenadores empezando por el nodo raiz buscando aquel entrenador que coincida con el nombre enviado por parametro.
Si coinciden los nombres, se asigna al entrenador enviado por parametro, el entrenador que cumple con la condicion.
En caso de error retorna. 
*/
void buscar_entrenador_preoden(nodo_abb_t* nodo_actual,const char* nombre_entrenador,entrenador_t** entrenador){
	if(!nodo_actual){
        return;
    }

    if(nodo_actual && strcmp(((entrenador_t*)nodo_actual->elemento)->nombre,nombre_entrenador) == 0){
       	(*entrenador) = ((entrenador_t*)nodo_actual->elemento);
    }  

    if(nodo_actual->izquierda){
        buscar_entrenador_preoden(nodo_actual->izquierda,nombre_entrenador,entrenador);
    }
   
    if(nodo_actual->derecha){
        buscar_entrenador_preoden(nodo_actual->derecha,nombre_entrenador,entrenador);
    }
}
/*
Pre condiciones : La funcion recibe un pokemon_t* pokemon cuyo espacio en memoria dinamica fue reservado previamente (en este caso mediante un calloc lo cual llena de ceros los valores en memoria reservada)
y recibe, a su vez, el puntero a un puntero a char de elementos inicializados, los cuales corresponden a los datos del pokemon a inicializar.
Post condiciones : La funcion inicializa del struct pokemon_t* todos los campos del mismo.
*/
void inicializar_pokemon_aux(pokemon_t* pokemon,char** elementos){
	if(!pokemon || !elementos){
		return;
	}
	strcpy(pokemon->nombre,elementos[1]);
	pokemon->nivel = (long int)atoi(elementos[2]);
	pokemon->defensa = (long int)atoi(elementos[3]);
	pokemon->fuerza = (long int)atoi(elementos[4]);
	pokemon->inteligencia = (long int)atoi(elementos[5]);
	pokemon->velocidad = (long int)atoi(elementos[6]);
}

bool buscar_entrenador(void* elemento,void* contexto){
	entrenador_t* iterado = (entrenador_t*)elemento;
	contexto_t* extra = contexto;

	if(strcmp(extra->nombre_entrenador,iterado->nombre)==0){
		*(extra->entrenador) = iterado;
		return true;
	}
	return false;
}

/*
Agrega a un entrenador del salon, un pokemon a su lista de pokemones, devolviendo el string "OK" si se pudo realizar la operacion.
En caso de error devuelve NULL.
*/
char* agregar_pokemon_a_entrenador(salon_t* salon, const char* linea){
	if(!salon){
		return NULL;
	}

	char** linea_sub_spliteada = split(linea,separador_2);
	if(!linea_sub_spliteada || vtrlen(linea_sub_spliteada) != 7){
		vtrfree(linea_sub_spliteada);
		return NULL;
	}

	char* string = NULL;

	entrenador_t* entrenador = NULL;
	pokemon_t* pokemon = NULL;
	contexto_t contexto = {linea_sub_spliteada[0],&entrenador,NULL,NULL,0};
	//buscar_entrenador_preoden(salon->entrenadores->nodo_raiz,linea_sub_spliteada[0],&entrenador);
	abb_con_cada_elemento(salon->entrenadores,ABB_RECORRER_PREORDEN,buscar_entrenador,&contexto);

	if(entrenador){
		pokemon = pokemon_crear();
	}else{
		vtrfree(linea_sub_spliteada);
		return NULL;
	}
	inicializar_pokemon_aux(pokemon,linea_sub_spliteada);

	int insercion = lista_insertar(entrenador->lista_pokemones,pokemon);

	if(insercion == ERROR || lista_vacia(entrenador->lista_pokemones)){
		vtrfree(linea_sub_spliteada);
		return NULL;
	}
	entrenador->cantidad_pokemones += 1;
	string = agregar_texto(string,"OK");

	vtrfree(linea_sub_spliteada);
	return string;
}
/*
Busca en la lista de pokemones el pokemon cuyo nombre coincida con el enviado por parametro y si coinciden los nombres
se le asigna al pokemon enviado al por parametro, el pokemon correspondiente. 
Por otra parte, determina la posicion en la que se encuentra dicho pokemon.
*/
void buscar_pokemon_y_determinar_posicion(lista_t* lista_pokemones, const char* nombre_pokemon,pokemon_t** pokemon,size_t* posicion){
	if(!lista_pokemones)
		return;
	lista_iterador_t* it = NULL;

	for(it = lista_iterador_crear(lista_pokemones); lista_iterador_tiene_siguiente(it); lista_iterador_avanzar(it)){
		if(it){
			if(strcmp(((pokemon_t*)lista_iterador_elemento_actual(it))->nombre,nombre_pokemon)==0){
				(*pokemon) = ((pokemon_t*)lista_iterador_elemento_actual(it));
				lista_iterador_destruir(it);
				return;
			}
			(*posicion) += 1;
		} else {
			return;
		}	
	}
	lista_iterador_destruir(it);
    return;
}
/*
Elimina el pokemon de la lista de pokemones del entrenador en la posicion correspondiente, devolviendo el string "OK" si se pudo realizar la operacion.
En caso de error devuelve NULL.
*/
char* eliminar_pokemon(entrenador_t* entrenador,size_t posicion){
	char* string = NULL;
	if(!entrenador || entrenador->cantidad_pokemones <= 1){
		return NULL;
	}
	free(((pokemon_t*)lista_elemento_en_posicion(entrenador->lista_pokemones,posicion)));
	int eliminacion = lista_borrar_de_posicion(entrenador->lista_pokemones,posicion);
	if(eliminacion == ERROR) return NULL;
	entrenador->cantidad_pokemones -= 1;
	string = agregar_texto(string,"OK");
	return string;
}

bool encontrar_pokemon_y_determinar_posicion(void* elemento,void* contexto){
	pokemon_t* iterado = (pokemon_t*)elemento;
	contexto_t* extra = (contexto_t*)contexto;
	if(strcmp(iterado->nombre,extra->nombre_pokemon)==0){
		*(extra->pokemon) = iterado;
		return false;
	}
	*(extra->posicion) += 1;
	return true;
}
/*
Procesando en primer lugar la linea enviada por parametro, se la splitea para ver extraer el pokemon a eliminar del entrenador correspondiente.
Elimina el pokemon de la lista de pokemones a un entrenador del salon devolviendo el string "OK" si se pudo realizar la operacion.
En caso de error devuelve NULL.
*/
char* quitar_pokemon_a_entrenador(salon_t* salon,const char* linea){
	if(!salon){
		return NULL;
	}
	char** linea_sub_spliteada = split(linea,separador_2);
	if(!linea_sub_spliteada || vtrlen(linea_sub_spliteada) != 2){
		vtrfree(linea_sub_spliteada);
		return NULL;
	}
	entrenador_t* entrenador_aux = NULL;
	pokemon_t* pokemon_a_eliminar = NULL;
	size_t posicion_pokemon_a_eliminar = 0;
	//buscar_entrenador_preoden(salon->entrenadores->nodo_raiz,linea_sub_spliteada[0],&entrenador_aux);
	contexto_t contexto = {linea_sub_spliteada[0],&entrenador_aux,linea_sub_spliteada[1],&pokemon_a_eliminar,&posicion_pokemon_a_eliminar};
	abb_con_cada_elemento(salon->entrenadores,ABB_RECORRER_PREORDEN,buscar_entrenador,&contexto);

	//buscar_pokemon_y_determinar_posicion(entrenador_aux->lista_pokemones,linea_sub_spliteada[1],&pokemon_a_eliminar,&posicion_pokemon_a_eliminar);
	lista_con_cada_elemento(entrenador_aux->lista_pokemones,encontrar_pokemon_y_determinar_posicion,&contexto);

	if(!entrenador_aux || !pokemon_a_eliminar){
		vtrfree(linea_sub_spliteada);
		return NULL;
	}
	char* string = NULL;
	string = eliminar_pokemon(entrenador_aux,posicion_pokemon_a_eliminar);

	vtrfree(linea_sub_spliteada);
	return string;
}
/**
 * Guarda el salón a un archivo (reescribiendo el archivo si existe).
 * Devuelve el string "OK" si se pudo realizar la operacion.
 * Devuelve NULL si no se pudo realizar la operacion.
 */
char* guardar_archivo(salon_t* salon,const char* nombre_archivo){
	if(!salon || !nombre_archivo)
		return NULL;
	char* string = NULL;
	int insercion = salon_guardar_archivo(salon,nombre_archivo);
	if(insercion == ERROR || insercion == 0){
		return NULL;
	}
	string = agregar_texto(string,"OK");
	return string;
}
/*
Recibe una vector de punteros a caracteres a ser spliteado.
Tras haber hecho el split se procesan los caracteres y dependiendo de cual sea devuelve una string particular
asociado al comando procesado en la linea.
*/
char* procesar_comando(char** linea_spliteada){
	char* string = NULL;
	if(strcmp(linea_spliteada[0],"ENTRENADORES")==0){
		if(linea_spliteada[1] == NULL){
			string = agregar_texto(string,"ENTRENADORES");
			return string;
		}else{
			char** linea_sub_spliteada = split(linea_spliteada[1],separador_2);
			if((strcmp(linea_sub_spliteada[0],"victorias")==0)&&(linea_sub_spliteada[1] != NULL)){
				string = agregar_texto(string,"ENTRENADORES_VICTORIAS");
				vtrfree(linea_sub_spliteada);
				return string;
			}else if((strcmp(linea_sub_spliteada[0],"pokemon")==0)&&(linea_sub_spliteada[1] != NULL)){
				string = agregar_texto(string,"ENTRENADORES_POKEMON");
				vtrfree(linea_sub_spliteada);
				return string;
			}else{
				vtrfree(linea_sub_spliteada);
				return NULL;
			}	
		}
	}else if(strcmp(linea_spliteada[0],"EQUIPO") == 0 && (linea_spliteada[1]) != NULL){
		string = agregar_texto(string,"ENTRENADORES_EQUIPO");
		return string;
	}else if(strcmp(linea_spliteada[0],"REGLAS") == 0 && (linea_spliteada[1]) == NULL){
		string = agregar_texto(string,"REGLAS");
		return string;
	}else if(strcmp(linea_spliteada[0],"COMPARAR") == 0 && linea_spliteada[1] != NULL){
		char** linea_sub_spliteada = split(linea_spliteada[1],separador_2);
		if(linea_sub_spliteada[0] && linea_sub_spliteada[1] && linea_sub_spliteada[2]){
			string = agregar_texto(string,"COMPARAR_ENTRENADORES");
			vtrfree(linea_sub_spliteada);
			return string;
		}else{
			vtrfree(linea_sub_spliteada);
			return NULL;
		}
	}else if(strcmp(linea_spliteada[0],"AGREGAR_POKEMON") == 0 && linea_spliteada[1] != NULL){
		string = agregar_texto(string,"AGREGAR_POKEMON");
		return string;
	}else if(strcmp(linea_spliteada[0],"QUITAR_POKEMON") == 0 && linea_spliteada[1] != NULL){
		string = agregar_texto(string,"QUITAR_POKEMON");
		return string;
	}else if(strcmp(linea_spliteada[0],"GUARDAR") == 0 && linea_spliteada[1] != NULL){
		string = agregar_texto(string,"GUARDAR");
		return string;
	}
	return string;
}

char* salon_ejecutar_comando(salon_t* salon, const char* comando){
	if(!salon || !comando){
		return NULL;
	}
	char* string = NULL;
	char** linea_spliteada = split(comando,separador_1);
	char* comando_a_procesar = procesar_comando(linea_spliteada);
	Comando_t* Comando = hash_obtener(salon->Comandos,comando_a_procesar);
	if(Comando){
		string = Comando->ejecutar(salon,linea_spliteada[1]);
		vtrfree(linea_spliteada);
		free(comando_a_procesar);
		return string;
	}else{
		vtrfree(linea_spliteada);
		free(comando_a_procesar);
		return NULL;
	}
	vtrfree(linea_spliteada);
	free(comando_a_procesar);
	return NULL;
}
/*
Destruye la memoria asociada a cada uno de los pokemones de algun entrenador.
*/
bool destruir_pokemon_de_cada_entrenador(void* elemento,void* extra){
	if(!elemento)
		return true;

	entrenador_t* entrenador_aux = ((entrenador_t*)elemento);
	
	for (size_t i = 0; i < lista_elementos(entrenador_aux->lista_pokemones); i++){
        pokemon_t* pokemon = lista_elemento_en_posicion(entrenador_aux->lista_pokemones, i);
        free(pokemon);
    }

    return false;
}


void salon_destruir(salon_t* salon){
	if(salon == NULL){
		return;
	}
    abb_con_cada_elemento(salon->entrenadores,ABB_RECORRER_PREORDEN,destruir_pokemon_de_cada_entrenador,NULL);
	arbol_destruir(salon->entrenadores);
	hash_destruir(salon->tabla_reglas);
	hash_destruir(salon->Comandos);
	free(salon);
}