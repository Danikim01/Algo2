#include "src/hospital.h"
#include "src/simulador_aux.h"
#include "src/simulador.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define ABB_RECORRER_INORDEN   0
#define ABB_RECORRER_PREORDEN  1
#define ABB_RECORRER_POSTORDEN 2

typedef struct{
    simulador_t* simulador;
    bool jugando;
}Juego;

char leer_comando(){
    char linea[100];
    char* leido;
    leido = fgets(linea, 100, stdin);
    if(!leido)
        return 0;
    while(*leido == ' ')
        leido++;
    return (char)tolower(*leido);
}

void mostrar_estadisticas(Juego* juego){
    EstadisticasSimulacion e;
    simulador_simular_evento(juego->simulador, ObtenerEstadisticas, &e);
    printf("La cantidad de entrenadores atendidos actualmente es %d\n",e.entrenadores_atendidos);
    printf("La cantidad de entrenadores totales son %d\n",e.entrenadores_totales);
    printf("La cantidad de pokemones atendidos actualmente es %d\n",e.pokemon_atendidos);
    printf("La cantidad de pokemones en pokemon_en_espera actualmente es %d\n",e.pokemon_en_espera);
    printf("La cantidad de pokemones totales actualmente son %d\n",e.pokemon_totales);
    printf("La cantidad de puntos son %d\n",e.puntos);
    printf("La cantidad de eventos simulados actualmente son %d\n",e.cantidad_eventos_simulados);
}

void atender_prox_entrenador(Juego* juego){
    simulador_simular_evento(juego->simulador,AtenderProximoEntrenador,NULL);
}

void adivinar_nivel_pokemon(Juego* juego){
    Intento intento;
    printf("Ingrese un posible nivel del pokemon (es un numero,claro): ");
    scanf("%d",&intento.nivel_adivinado);
    ResultadoSimulacion retorno = simulador_simular_evento(juego->simulador,AdivinarNivelPokemon,&intento);
    if (retorno == ErrorSimulacion){
        printf("Se produjo un error: Es posible que no haya pokemon siendo atendido\n");
        return;
    } 
    printf("%s\n",intento.resultado_string);  
    while (!intento.es_correcto){
        printf("Ingrese un posible nivel del pokemon (es un numero,claro): ");
        scanf("%d",&intento.nivel_adivinado);
        printf("%s\n",intento.resultado_string);
        retorno = simulador_simular_evento(juego->simulador,AdivinarNivelPokemon,&intento);
    }
}

void terminar_simulacion(Juego* juego){
    simulador_simular_evento(juego->simulador,FinalizarSimulacion,NULL);
    juego->jugando = false;
}

void mostrar_pokemon_tratado(Juego* juego){
    InformacionPokemon info_poke;
    ResultadoSimulacion resultado;
    resultado = simulador_simular_evento(juego->simulador,ObtenerInformacionPokemonEnTratamiento,&info_poke);
    if(resultado == ErrorSimulacion){
        printf("No hay pokemon siendo tratado actualmente\n");
        return;
    }
    printf("El nombre del pokemon tratado es: %s\n",info_poke.nombre_pokemon);
    printf("El entrenador del pokemon tratado es: %s\n",info_poke.nombre_entrenador);
}

void cambiar_dificultad(Juego* juego){
    int id_ingresado;
    printf("Ingrese el id de la dificultad a seleccionar: ");
    scanf("%i",&id_ingresado);
    printf("\n");
    ResultadoSimulacion nuevo_retorno = simulador_simular_evento(juego->simulador,SeleccionarDificultad,&id_ingresado);
    while(nuevo_retorno == ErrorSimulacion){
        printf("Ingrese nuevamente id de la dificultad a seleccionar: ");
        scanf("%i",&id_ingresado);
        printf("\n");
        nuevo_retorno = simulador_simular_evento(juego->simulador,SeleccionarDificultad,&id_ingresado);
    }
}

void buscar_dificultad_particular(Juego* juego){
    InformacionDificultad info_dificultad;
    printf("Ingrese el id de la dificultad buscada: ");
    scanf("%i",&info_dificultad.id);
    printf("\n");
    ResultadoSimulacion nuevo_retorno = simulador_simular_evento(juego->simulador,ObtenerInformacionDificultad,&info_dificultad);
    
    while(nuevo_retorno == ErrorSimulacion){
        printf("Ingrese nuevamente el id de la dificultad buscada: ");
        scanf("%i",&info_dificultad.id);
        printf("\n");
        nuevo_retorno = simulador_simular_evento(juego->simulador,ObtenerInformacionDificultad,&info_dificultad);
    }

    if(nuevo_retorno == ExitoSimulacion){
        printf("Nombre: %s\n",info_dificultad.nombre_dificultad);
        printf("Id: %i\n",info_dificultad.id);
        if(info_dificultad.en_uso){
            printf("Estado: Actualmente en uso\n");
        }else{
            printf("Estado: Actualmente no se esta usando\n");
        }
    }
}

bool mostrar_dif(void* elemento,void* extra){
    DatosDificultadAux* dificultad_iterada = elemento;
    simulador_t* simulador = extra;
    if (dificultad_iterada == simulador->dificultad_actual){
        printf("%i.",dificultad_iterada->id_dificultad);
        printf("%s (en uso)\n",dificultad_iterada->nueva_dificultad->nombre);
    }else{
        printf("%i.",dificultad_iterada->id_dificultad);
        printf("%s\n",dificultad_iterada->nueva_dificultad->nombre);
    }
    return false;
}

void mostrar_dificultades(Juego* juego){
    printf("Dificultades disponibles:\n");
    abb_con_cada_elemento(juego->simulador->arbol_niveles,ABB_RECORRER_INORDEN,mostrar_dif,juego->simulador);
}

void presentacion(){
    printf("(e) Mostrar Estadisticas\n");
    printf("(p) Atender Entrenador\n");
    printf("(i) Mostrar pokemon\n");
    printf("(a) Tratar pokemon\n");
    printf("(d) Cambiar dificultad\n");
    printf("(o) Buscar dificultad\n");
    printf("(k) Mostrar dificultades\n");
    printf("(h) Ayuda\n");
    printf("(q) Salir\n");
}

void mostrar_ayuda(){
    printf("(e) Mostrar Estadisticas\n");
    printf("\n");
    printf("\t\t Muestra las estadisticas del juego\n");
    printf("\n");
    printf("(p) Atender Entrenador\n");
    printf("\n");
    printf("\t\t Atiende al proximo entrenador en espera\n");
    printf("\n");
    printf("(i) Mostrar pokemon\n");
    printf("\n");
    printf("\t\t Muestra la informacion del pokemon siendo atendido actualmente\n");
    printf("\n");
    printf("(a) Tratar pokemon\n");
    printf("\n");
    printf("\t\t Comienza el tratamiento del pokemon (basicamente consiste en adivinar su nivel :)\n");
    printf("\n");
    printf("(d) Cambiar dificultad\n");
    printf("\n");
    printf("\t\t El juego tiene dificultades estandares (presione el comando 'o' para verlas), con este comando se puede cambiar la dificultad\n");
    printf("\n");
    printf("(o) Buscar dificultad\n");
    printf("\n");
    printf("\t\t Muestra la informacion de una dificultad en particular (la que usted quiera)\n");
    printf("\n");
    printf("(k) Mostrar dificultades\n");
    printf("\n");
    printf("\t\t Muestra las dificultades disponibles del juego\n");
    printf("\n");
    printf("(h) Ayuda\n");
    printf("\n");
    printf("\t\t Muestra esta ayuda que esta viendo usted ahora mismo\n");
    printf("\n");
    printf("(q) Salir\n");
    printf("\n");
    printf("\t\t Finaliza el juego\n");
    printf("\n");
}

void ejecutar_comando(Juego *juego, char comando){
    switch (comando) {
        case 'q': //Salir del juego
            terminar_simulacion(juego);
            break;
        case 'e':
            mostrar_estadisticas(juego);
            break;
        case 'p':
            atender_prox_entrenador(juego);
            break;
        case 'h':
            mostrar_ayuda();
            break;        
        case 'i':
            mostrar_pokemon_tratado(juego);
            break;
        case 'a':
            adivinar_nivel_pokemon(juego);
            break; 
        case 'd':
            cambiar_dificultad(juego);
            break;  
        case 'o':
            buscar_dificultad_particular(juego);
            break;
        case 'k':
            mostrar_dificultades(juego);
            break;       
        default: //el resto de las cosas
            break;
    }
}

void inicializar_juego(Juego* juego,hospital_t* hospital){
    juego->simulador = simulador_crear(hospital);
    if(!juego->simulador){
        return;
    }
    juego->jugando = true;
}

void destruir_juego(Juego juego){
    simulador_destruir(juego.simulador);
}


int main(int argc, char *argv[]) {

    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    Juego juego;
    inicializar_juego(&juego, hospital);
    presentacion();
    do{
        printf("> ");
        char c = leer_comando();
        ejecutar_comando(&juego, c);
    }while(juego.jugando);

    hospital_destruir(hospital);
    destruir_juego(juego);

    return 0;
}
