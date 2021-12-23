#include "simulador_aux.h"

#define ABB_RECORRER_INORDEN   0
#define ABB_RECORRER_PREORDEN  1
#define ABB_RECORRER_POSTORDEN 2


char* agregar_texto_2(char* string_original,char* string_a_agregar){
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

int comparador_id_niveles(void* elemento1,void* elemento2){
    if(!elemento1 || !elemento2){
        return 0;
    }
    DatosDificultadAux* dif1 = elemento1;
    DatosDificultadAux* dif2 = elemento2;

    return ((dif1->id_dificultad)-(dif2->id_dificultad));
}

int comparador_nivel_pokemon(void* elemento1,void* elemento2){
	if(!elemento1 || !elemento2){
        return 0;
    }
    pokemon_t* pok1 = elemento1;
    pokemon_t* pok2 = elemento2;

    return ((int)pokemon_nivel(pok1)-(int)pokemon_nivel(pok2));
}

void destruir_nivel(void* dato){
	DatosDificultad* dificultad = dato;
	free((char*)dificultad->nombre);
	free(dificultad);
}

void destruir_nivel_aux(void* dato){
	DatosDificultadAux* dificultad = dato;
	destruir_nivel(dificultad->nueva_dificultad);
	free(dificultad);
}

void destruir_pokemon_aux(void* elemento){
    pokemon_t* pokemon = elemento;
    free((char*)pokemon_nombre(pokemon));
    free(pokemon);
}

int verificar_nivel(unsigned nivel_adivinado, unsigned nivel_pokemon){
	if(nivel_adivinado == nivel_pokemon){
		return 0;
	}
	return abs((int)nivel_adivinado - (int)nivel_pokemon);
}

unsigned calcular_puntaje_facil(unsigned cantidad_intentos){
	unsigned puntos = 100;
	for(unsigned i = 0; i < cantidad_intentos;i++){
		puntos -= i;
	}
	return puntos;
}

unsigned calcular_puntaje_medio(unsigned cantidad_intentos){
	unsigned puntos = 100;
	for(unsigned i = 0; i < cantidad_intentos;i++){
		puntos -= i*2;
	}
	return puntos;
}


const char* verificacion_a_string_medio(int resultado_verificacion){
	const char* buffer = NULL;
	const char** retorno	= NULL;
	if(resultado_verificacion >= 50){
		buffer = "Frio";
	}else if(resultado_verificacion < 49 && resultado_verificacion > 10){
		buffer = "Tibio";
	}else if(resultado_verificacion <= 10 && resultado_verificacion > 0){
		buffer = "Caliente";
	}else{
		buffer = "Nivel Adivinado Felicidades";
	}
	retorno = &buffer;
	return (*retorno);
}


const char* verificacion_a_string_facil(int resultado_verificacion){
	const char* buffer = NULL;
	const char** retorno = NULL;
	if(resultado_verificacion >= 50){
		buffer = "Estas por por lo menos a 50 de diferencia del correcto";
	}else if(resultado_verificacion < 49 && resultado_verificacion > 10){
		buffer = "Estas entre 49 y 10 de diferencia del correcto";
	}else if(resultado_verificacion <= 10 && resultado_verificacion > 0){
		buffer = "Estas como maximo a 10 de diferencia con el correcto";
	}else{
		buffer = "Nivel Adivinado Felicidades";
	}
	retorno = &buffer;
	return (*retorno);
}


DatosDificultad* dificultad_crear(const char* nombre,unsigned (*calcular_puntaje)(unsigned cantidad_intentos),int (*verificar_nivel)(unsigned nivel_adivinado, unsigned nivel_pokemon),const char* (*verificacion_a_string)(int resultado_verificacion)){
	DatosDificultad* dificultad = calloc(1,sizeof(DatosDificultad));
	if(!dificultad){
		return NULL;
	}
	dificultad->nombre = agregar_texto_2((char*)dificultad->nombre,(char*)nombre);
	dificultad->calcular_puntaje = calcular_puntaje;
	dificultad->verificar_nivel = verificar_nivel;
	dificultad->verificacion_a_string = verificacion_a_string;
	return dificultad;
}

DatosDificultadAux* dificultad_aux_crear(const char* nombre,unsigned (*calcular_puntaje)(unsigned cantidad_intentos),int (*verificar_nivel)(unsigned nivel_adivinado, unsigned nivel_pokemon),const char* (*verificacion_a_string)(int resultado_verificacion),int id){
	DatosDificultadAux* dificultad = calloc(1,sizeof(DatosDificultadAux));
	if(!dificultad){
		return NULL;
	}
	dificultad->nueva_dificultad = dificultad_crear(nombre,calcular_puntaje,verificar_nivel,verificacion_a_string);
	dificultad->id_dificultad = id;
	return dificultad;
}

/**
 * Crea un simulador para un hospital. El simulador toma control del hospital y
 * el mismo no debe ser modificado ni liberado por fuera del simulador.
 *
 * Devuelve el simulador creado o NULL en caso de error.
 */
simulador_t* simulador_crear(hospital_t* hospital){
	if(!hospital){
		return NULL;
	}
	simulador_t* simulador = calloc(1,sizeof(simulador_t));
	if(!simulador){
		return NULL;
	}
	simulador->hospital_pokemon = hospital;
	simulador->pokemones_en_espera = arbol_crear(comparador_nivel_pokemon,NULL);
	

	char* f = "Facil";
	char* m = "Medio";
	DatosDificultadAux* dificultad_facil = dificultad_aux_crear(f,calcular_puntaje_facil,verificar_nivel,verificacion_a_string_facil,0);
	simulador->cantidad_niveles += 1;
	DatosDificultadAux* dificultad_medio = dificultad_aux_crear(m,calcular_puntaje_medio,verificar_nivel,verificacion_a_string_medio,1);
	simulador->cantidad_niveles += 1;	


	simulador->arbol_niveles = arbol_crear(comparador_id_niveles,destruir_nivel_aux);
	arbol_insertar(simulador->arbol_niveles,dificultad_facil);
	arbol_insertar(simulador->arbol_niveles,dificultad_medio);
	

	simulador->dificultad_actual = arbol_buscar(simulador->arbol_niveles,dificultad_medio);
	//printf("La dificultad actual es %s\n",simulador->dificultad_actual->nombre);
	return simulador;
}

/**
 * Simula un evento. Cada evento puede recibir un puntero a un dato que depende
 * de cada evento. En caso de no necesitarse, se debe pasar NULL.
 *
 * Ejecuta el evento correspondiente (ver la documentación) y en caso de ser
 * necesario modifica la información apuntada por dato.
 *
 * Devuelve ErrorSimulacion si no existe el simulador o existe algún error en el
 * manejo de memoria.
 *
 * Devuelve ExitoSimulacion o ErrorSimulacion según corresponda a cada evento.
 */

void inicializar_estadisticas(simulador_t* simulador,void* datos){
	EstadisticasSimulacion* dato_aux = datos;
	dato_aux->entrenadores_atendidos = simulador->cantidad_entrenadores_atendidos;
	dato_aux->entrenadores_totales = hospital_cantidad_entrenadores(simulador->hospital_pokemon);
	dato_aux->pokemon_atendidos = simulador->pokemon_atendidos;
	dato_aux->pokemon_en_espera = simulador->pokemon_en_espera;
	dato_aux->pokemon_totales = hospital_cantidad_pokemon(simulador->hospital_pokemon);
	dato_aux->puntos = simulador->puntos;
	dato_aux->cantidad_eventos_simulados = simulador->cantidad_eventos_simulados;
}

bool iterar_entrenadores(void* elemento,void* aux){
	entrenador_t* entrenador = elemento;
	bool* ctx = aux;
	if(!entrenador->entrenador_atendido){
		//digo que corte la iteracion cuando encuentre a un entrenador sin atender.
		printf("El entrenador sin atener es %s\n",entrenador->nombre_entrenador);
		*(ctx) = false;
		return false;
	}
	return true;
}

bool hay_entrenadores_sin_atender(simulador_t* simulador){
	bool auxiliar = true;
	lista_con_cada_elemento(simulador->hospital_pokemon->entrenadores_en_sala,iterar_entrenadores,&auxiliar);
	return (!auxiliar);
}

bool atender_pokemones_2(void* elemento,void* aux){
    pokemon_t* pok_iterado = elemento;
    simulador_t* simulador = aux;
    //printf("El pokemon a insertar es %s\n",pok_iterado->nombre);
    arbol_insertar(simulador->pokemones_en_espera,pok_iterado);
    simulador->pokemon_en_espera += 1;
    return false;
}

bool atender_pokemones(void* elemento,void* aux){
    entrenador_t* entrenador_iterado = elemento;
    simulador_t* simulador = aux;
    //printf("El entrenador iterado es %s\n",entrenador_iterado->nombre_entrenador);    
    if(!entrenador_iterado->entrenador_atendido){
    	abb_con_cada_elemento(entrenador_iterado->arbol_pokemones,ABB_RECORRER_INORDEN,atender_pokemones_2,(void*)simulador);
    	entrenador_iterado->entrenador_atendido = true;
    	simulador->cantidad_entrenadores_atendidos += 1;
    	return false;
    }
    return true;
}

bool mostrar_poks_atendidos(void* elemento,void* extra){
    pokemon_t* pokemon = elemento;
    printf("Nombre:%s\n",pokemon->nombre);
    printf("Nivel:%d\n",pokemon->nivel);
    //printf("Dueño:%s\n",((entrenador_t*)pokemon->entrenador_del_pokemon)->nombre_entrenador);
    if(pokemon->atendido){
    	printf("El pokemon: %s ya fue tratado\n",pokemon->nombre);
    }
    
    return false;
}

bool asignar_pokemon_menor_nivel(void* elemento,void* extra){
	pokemon_t* pokemon_iterado = elemento;
	simulador_t* simulador = extra;
	if(simulador->pokemon_en_tratamiento == NULL && !pokemon_iterado->atendido){
		printf("NO HAY POKEMONES EN EL CONSULTORIO\n");
		printf("El pokemon a tratar va a ser %s\n",pokemon_iterado->nombre);
		simulador->pokemon_en_tratamiento = pokemon_iterado;
		return true;
	}
	return false;
}

bool cambiar_pokemon_en_tratamiento(void* elemento,void* extra){
	pokemon_t* pokemon_iterado = elemento;
	simulador_t* simulador = extra;
	if (!pokemon_iterado->atendido){
		printf("El nuevo pokemon a a reemplazar es %s\n",pokemon_iterado->nombre);
		simulador->pokemon_en_tratamiento = pokemon_iterado;
		return true;
	}
	return false;
}

bool asignar_pokemon_menor_nivel_2(void* elemento,void* extra){
	pokemon_t* pokemon_iterado = elemento;
	pokemon_t** a_asignar = extra;
	if (!pokemon_iterado->atendido){
		(*a_asignar) = pokemon_iterado;
		return true;
	}
	return false;
}

bool algun_campos_nulo(void* datos){
	DatosDificultad* dificultad = datos;
	if(!dificultad->nombre){
		printf("El nombre es nulo\n");
	}else if(!dificultad->calcular_puntaje){
		printf("Puntaje nulo\n");
	}else if(!dificultad->verificar_nivel){
		printf("nivel verificar nulo\n");
	}else if(!dificultad->verificacion_a_string){
		printf("verificacion a string nulo\n");
	}
	return (!dificultad->nombre || !dificultad->calcular_puntaje || !dificultad->verificar_nivel || !dificultad->verificacion_a_string);
}

typedef struct contexto{
	bool esta_repetido;
	DatosDificultad* dificultad;
}contexto_t;

bool chequear_nombre_repetido(void* elemento,void* extra){
	DatosDificultadAux* dificultad_iterada = elemento;
	contexto_t* ctx = extra;
	if (strcmp(dificultad_iterada->nueva_dificultad->nombre,ctx->dificultad->nombre)==0){
		ctx->esta_repetido = true;
		return true;
	}
	return false;
}	

bool nombre_dificultad_repetido(simulador_t* simulador,void* datos){
	DatosDificultad* dificultad = datos;
	contexto_t ctx;
	ctx.esta_repetido = false;
	ctx.dificultad = dificultad;
	abb_con_cada_elemento(simulador->arbol_niveles,ABB_RECORRER_INORDEN,chequear_nombre_repetido,&ctx);
	if (ctx.esta_repetido == false){
		printf("El nombre no esta repetido \n");
	}
	return (ctx.esta_repetido == false);
}

typedef struct otro_ctx{
	int id;
	bool invalido;
}otro_ctx_t;
 
bool chequear_id_inexistente(void* elemento,void* extra){
	DatosDificultadAux* dificultad_iterada = elemento;
	otro_ctx_t* ctx = extra;
	if (dificultad_iterada->id_dificultad == ctx->id){
		ctx->invalido = true;
		return true;
	}
	return false;
}

bool id_inexistente(simulador_t* simulador,void* datos){
	int* id = datos;
	otro_ctx_t ctx;
	ctx.id = (*id);
	ctx.invalido = false;
	abb_con_cada_elemento(simulador->arbol_niveles,ABB_RECORRER_INORDEN,chequear_id_inexistente,&ctx);
	return (ctx.invalido == false);
}

typedef struct otro_contexto_mas{
	DatosDificultadAux** dificultad_a_buscar;
	int id;
}otro_contexto_mas_t;

bool buscar_dificultad(void* elemento,void* extra){
	DatosDificultadAux* dificultad_iterada = elemento;
	otro_contexto_mas_t* ctx = extra;
	if (dificultad_iterada->id_dificultad == ctx->id){
		*(ctx->dificultad_a_buscar) = dificultad_iterada;
		return true;
	}
	return false;
}

DatosDificultadAux* buscar_dificultad_con_id(abb_t* arbol_niveles,int id){
	DatosDificultadAux* dificultad = NULL;
	otro_contexto_mas_t ctx;
	ctx.dificultad_a_buscar = &dificultad;
	ctx.id = id;
	abb_con_cada_elemento(arbol_niveles,ABB_RECORRER_INORDEN,buscar_dificultad,&ctx);
	return (dificultad);
}

ResultadoSimulacion simulador_simular_evento(simulador_t* simulador, EventoSimulacion evento, void* datos){
	if(!simulador){
		return ExitoSimulacion;
	}

	if(evento == ObtenerEstadisticas){
		if(!datos){
			simulador->cantidad_eventos_simulados += 1;
			return ErrorSimulacion;
		}
		simulador->cantidad_eventos_simulados += 1;
		inicializar_estadisticas(simulador,datos);
		return ExitoSimulacion;
	}else if(evento == AtenderProximoEntrenador){
		if(hay_entrenadores_sin_atender(simulador)){
			lista_con_cada_elemento(simulador->hospital_pokemon->entrenadores_en_sala,atender_pokemones,simulador);
    		abb_con_cada_elemento(simulador->pokemones_en_espera,ABB_RECORRER_INORDEN,mostrar_poks_atendidos,NULL);	
	    	if(!arbol_vacio(simulador->pokemones_en_espera)){
	    		//esta asignacion la hago mientras no haya pokemon en el consultorio, ed, si hay pokemon en tratamiento no reasigno.
	    		abb_con_cada_elemento(simulador->pokemones_en_espera,ABB_RECORRER_INORDEN,asignar_pokemon_menor_nivel,simulador);
	    	}else{
	    		printf("NO QUEDAN POKEMONES POR ATENDER\n");
	    		simulador->cantidad_eventos_simulados += 1;
				return ErrorSimulacion;
	    	}
	    	simulador->cantidad_eventos_simulados += 1;
			return ExitoSimulacion;
		}else{
			simulador->cantidad_eventos_simulados += 1;
			return ErrorSimulacion;
		}
	}else if(evento == ObtenerInformacionPokemonEnTratamiento){
		InformacionPokemon* info = datos;
		info->nombre_pokemon = NULL;
		info->nombre_entrenador = NULL;
		if(!datos){	
			simulador->cantidad_eventos_simulados += 1;
			return ErrorSimulacion;
		}
		//aca me tengo que fijar que si no hay pokemon atendido, me fijo en el pokemon que tiene que ser atendido
		if (simulador->pokemon_en_tratamiento != NULL){
			info->nombre_pokemon = (const char*)simulador->pokemon_en_tratamiento->nombre;
			info->nombre_entrenador = (const char*)pokemon_entrenador(simulador->pokemon_en_tratamiento)->nombre_entrenador;
		}else{
			if(!arbol_vacio(simulador->pokemones_en_espera)){
	    		printf("Actualmente no hay pokemon en tratamiento,busco el proximo a atender\n"); 
	    		pokemon_t* pokemon_a_atender = NULL;
	    		abb_con_cada_elemento(simulador->pokemones_en_espera,ABB_RECORRER_INORDEN,asignar_pokemon_menor_nivel_2,&pokemon_a_atender);
	    		info->nombre_pokemon = (const char*)pokemon_a_atender->nombre;
				info->nombre_entrenador = (const char*)pokemon_entrenador(pokemon_a_atender)->nombre_entrenador;
	    	}else{
	    		printf("NO QUEDAN POKEMONES POR ATENDER\n");
	    		simulador->cantidad_eventos_simulados += 1;
				return ErrorSimulacion;
	    	}
		}
		simulador->cantidad_eventos_simulados += 1;
		return ExitoSimulacion;
	}else if(evento == AdivinarNivelPokemon){
		if(!datos || !simulador->pokemon_en_tratamiento){
			simulador->cantidad_eventos_simulados += 1;
			return ErrorSimulacion;
		}
		pokemon_t* pokemon = simulador->pokemon_en_tratamiento;
		//Cantidad de intentos por adivinar segun cada pokemon.
		pokemon->cantidad_intentos += 1;
		Intento* intento = datos;
		DatosDificultad* dificultad_en_uso = simulador->dificultad_actual->nueva_dificultad;
		int resultado = dificultad_en_uso->verificar_nivel(intento->nivel_adivinado,pokemon->nivel);
		if (resultado == 0){
			intento->es_correcto = true;
			//si el nivel se adivino tengo que atener al proximo pokemon
			//que esta en espera
			simulador->pokemon_atendidos += 1;
			simulador->pokemon_en_espera -= 1;
			simulador->pokemon_en_tratamiento->atendido = true;
			printf("El pokemon %s ya cuenta como atendido\n",simulador->pokemon_en_tratamiento->nombre);
			if(!arbol_vacio(simulador->pokemones_en_espera)){
	    		abb_con_cada_elemento(simulador->pokemones_en_espera,ABB_RECORRER_INORDEN,cambiar_pokemon_en_tratamiento,simulador);   		
	    		//tambien tengo que eliminar del arbol de pokemones en espera el pokemon ya tratado
	    		printf("Borro del arbol de pokemones en espera el pokemon atendido: %s\n",pokemon->nombre);
	    		arbol_borrar(simulador->pokemones_en_espera,pokemon);
	    	}else{
	    		printf("NO QUEDAN POKEMONES POR ATENDER\n");
	    		simulador->cantidad_eventos_simulados += 1;
				return ErrorSimulacion;
	    	}
		}else{
			intento->es_correcto = false;
		}
		const char* a_asignar = dificultad_en_uso->verificacion_a_string(resultado);
		intento->resultado_string = a_asignar;
		simulador->puntos = dificultad_en_uso->calcular_puntaje(pokemon->cantidad_intentos);
		//printf("La cantidad de puntos son %d\n",simulador->puntos);
		simulador->cantidad_eventos_simulados += 1;
		return ExitoSimulacion;
	}else if(evento == AgregarDificultad){
		if(!datos || algun_campos_nulo(datos) || !nombre_dificultad_repetido(simulador,datos)){
			return ErrorSimulacion;
		}
		DatosDificultad* nueva_dificultad = datos;
		int id_a_asignar = simulador->cantidad_niveles;
		printf("Agrego una nueva dificultad cuyo nombre es %s y el id a asignar va a ser %i\n",nueva_dificultad->nombre,id_a_asignar);
		DatosDificultadAux* nueva_dificultad_aux = dificultad_aux_crear(nueva_dificultad->nombre,nueva_dificultad->calcular_puntaje,nueva_dificultad->verificar_nivel,nueva_dificultad->verificacion_a_string,id_a_asignar);
		simulador->cantidad_niveles += 1;
		arbol_insertar(simulador->arbol_niveles,nueva_dificultad_aux);
		simulador->cantidad_eventos_simulados += 1;
		return ExitoSimulacion;
	}else if(evento == SeleccionarDificultad){
		if(!datos || id_inexistente(simulador,datos)){
			simulador->cantidad_eventos_simulados += 1;
			return ErrorSimulacion;
		}
		int id = *(int*)datos;
		printf("La dificultad previa es :%s\n",simulador->dificultad_actual->nueva_dificultad->nombre);
		simulador->dificultad_actual = buscar_dificultad_con_id(simulador->arbol_niveles,id);
		printf("Ahora la nueva dificultad es :%s\n",simulador->dificultad_actual->nueva_dificultad->nombre);
		simulador->cantidad_eventos_simulados += 1;
		return ExitoSimulacion;
	}else if(evento == FinalizarSimulacion){
		simulador->simulacion_finalizada = true;
		simulador->cantidad_eventos_simulados += 1;
		return ExitoSimulacion;
	}else if(evento == ObtenerInformacionDificultad){
		if(!datos){
			return ErrorSimulacion;
		}
		InformacionDificultad* info = datos;
		if(id_inexistente(simulador,&info->id)){
			info->nombre_dificultad = NULL;
			info->id = -1;
			info->en_uso = false;
			simulador->cantidad_eventos_simulados += 1;
			return ExitoSimulacion;
		}
		DatosDificultadAux* dificultad = buscar_dificultad_con_id(simulador->arbol_niveles,info->id);
		info->nombre_dificultad = dificultad->nueva_dificultad->nombre;
		if (simulador->dificultad_actual == dificultad){
			info->en_uso = true;
		}else{
			info->en_uso = false;
		}
		simulador->cantidad_eventos_simulados += 1;
		return ExitoSimulacion;
	}

	return ErrorSimulacion;
}

/**
 * Destruye el simulador y libera la memoria asociada (incluida la del hospital).
 */
void simulador_destruir(simulador_t* simulador){
	if(!simulador){
		return;
	}
	arbol_destruir(simulador->arbol_niveles);
	arbol_destruir(simulador->pokemones_en_espera);
	free(simulador);
}