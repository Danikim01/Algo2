#include "pa2mm.h"
#include "src/hospital.h"
#include "src/simulador.h"

#include "string.h"
#include <stdbool.h>

bool ignorar_pokemon(pokemon_t* p){
    p = p;
    return true;
}

/* No intenten esto en sus casas */
/* Ya vamos a ver como evitar esto en el TDA Lista */
struct{
    pokemon_t* pokemon[500];
    size_t cantidad;
} acumulados;

void resetear_acumulados(){
    acumulados.cantidad = 0;
}

bool acumular_pokemon(pokemon_t* p){
    acumulados.pokemon[acumulados.cantidad] = p;
    acumulados.cantidad++;
    return true;
}

bool acumular_pokemon_hasta_miltank(pokemon_t* p){
    acumulados.pokemon[acumulados.cantidad] = p;
    acumulados.cantidad++;
    return strcmp(pokemon_nombre(p), "miltank");
}

bool acumulados_en_orden_correcto(){
    if(acumulados.cantidad < 2)
        return true;
    pokemon_t* anterior = acumulados.pokemon[0];
    //printf("El pokemon anterior es:%s\n",pokemon_nombre(anterior));
    for(int i=1;i<acumulados.cantidad;i++){
        pokemon_t* actual =  acumulados.pokemon[i];
        //printf("El pokemon actual es:%s\n",pokemon_nombre(actual));
        if(strcmp(pokemon_nombre(anterior), pokemon_nombre(actual)) > 0){
            //printf("PORQUE ENTRAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
            return false;
        }
    }
    return true;
}

/* Pruebas */

void puedoCrearYDestruirUnHospital(){
    hospital_t* h=NULL;

    pa2m_afirmar((h=hospital_crear()), "Crear un hospital devuelve un hospital");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital se crea con cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital se crea con cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema(){
    hospital_t* h=NULL;

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital NULL tiene cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital NULL tiene cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon de un hospital NULL resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/archivo_vacio.hospital"), "Puedo leer un archivo vacío");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital vacío tiene cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital vacío tiene tiene cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/un_entrenador.hospital"), "Puedo leer un archivo con un entrenador");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==1, "El hospital tiene 1 entrenador");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==3, "El hospital tiene 3 pokemon");

    resetear_acumulados();
    size_t cantidad_recorridos = hospital_a_cada_pokemon(h, acumular_pokemon);
    printf("LA CANTIDAD DE RECORRIDOS SON:%d\n",cantidad_recorridos);
    pa2m_afirmar(cantidad_recorridos==3, "Recorrer los pokemon resulta en 3 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}

void dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==5, "El hospital tiene 5 entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==24, "El hospital tiene 24 pokemon");

    resetear_acumulados();
    size_t cantidad_recorridos = hospital_a_cada_pokemon(h, acumular_pokemon);
    printf("LA CANTIDAD DE RECORRIDOS SON:%d\n",cantidad_recorridos);
    pa2m_afirmar(cantidad_recorridos==24, "Recorrer los pokemon resulta en 24 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}


void dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer otro archivo con varios entrenadores");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un tercer archivo con varios entrenadores");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==15, "El hospital tiene 15 entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==72, "El hospital tiene 72 pokemon");

    resetear_acumulados();
    size_t cantidad_recorridos = hospital_a_cada_pokemon(h, acumular_pokemon);
    printf("LA CANTIDAD DE RECORRIDOS SON:%d\n",cantidad_recorridos);
    pa2m_afirmar(cantidad_recorridos==72, "Recorrer los pokemon resulta en 72 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}


int verificar_nivel_muy_dificil(unsigned nivel_adivinado, unsigned nivel_pokemon){
    if(nivel_adivinado == nivel_pokemon){
        return 0;
    }
    return abs((int)nivel_adivinado - (int)nivel_pokemon);
}

unsigned calcular_puntaje_muy_dificil(unsigned cantidad_intentos){
    unsigned puntos = 100;
    for(unsigned i = 0; i < cantidad_intentos;i++){
        puntos -= 30;
    }
    return puntos;
}

const char* verificacion_a_muy_dificil(int resultado_verificacion){
    const char* buffer = NULL;
    const char** retorno    = NULL;
    if(resultado_verificacion >= 50){
        buffer = "A Pipiiiikachuuuuuuuuu no le gustan las manzanas";
    }else if(resultado_verificacion < 49 && resultado_verificacion > 10){
        buffer = "Me gustan los charmander shiny";
    }else if(resultado_verificacion <= 10 && resultado_verificacion > 0){
        buffer = "Groudon en los volcanes";
    }else{
        buffer = "Bienvenido al salon de la fama";
    }
    retorno = &buffer;
    return (*retorno);
}

void PruebasSimulador(){
    hospital_t* h=hospital_crear();

    pa2m_nuevo_grupo("Separador");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");
    simulador_t* s=simulador_crear(h);
    EstadisticasSimulacion e;
    pa2m_afirmar(ExitoSimulacion == simulador_simular_evento(s,ObtenerEstadisticas,&e),"El evento ObtenerEstadisticas devuele ExitoSimulacion");
    pa2m_afirmar(e.entrenadores_atendidos==0,"La cantidad de entrenadores atendidos actualmente es 0");
    pa2m_afirmar(e.entrenadores_totales==5,"La cantidad de entrenadores totales son 5");
    pa2m_afirmar(e.pokemon_atendidos==0,"La cantidad de pokemones atendidos actualmente es 0");
    pa2m_afirmar(e.pokemon_en_espera==0,"La cantidad de pokemones en pokemon_en_espera actualmente es 0");
    pa2m_afirmar(e.pokemon_totales==24,"La cantidad de pokemones totales actualmente son 24");
    pa2m_afirmar(e.puntos==0,"La cantidad de puntos son 0");
    pa2m_afirmar(e.cantidad_eventos_simulados==1,"La cantidad de cantidad_eventos_simulados actualmente son 1");

    pa2m_nuevo_grupo("Separador");
    pa2m_afirmar(ExitoSimulacion == simulador_simular_evento(s,AtenderProximoEntrenador,NULL),"El evento AtenderProximoEntrenador devuele ExitoSimulacion");
    pa2m_afirmar(ExitoSimulacion == simulador_simular_evento(s,ObtenerEstadisticas,&e),"El evento ObtenerEstadisticas devuele ExitoSimulacion");
    pa2m_afirmar(e.entrenadores_atendidos==1,"La cantidad de entrenadores atendidos actualmente es 1");
    pa2m_afirmar(e.pokemon_en_espera==4,"La cantidad de pokemones en pokemon_en_espera actualmente es 4");
    pa2m_afirmar(e.cantidad_eventos_simulados==3,"La cantidad de cantidad_eventos_simulados actualmente son 3");

    pa2m_nuevo_grupo("Separador");
    InformacionPokemon info_poke;
    pa2m_afirmar(ExitoSimulacion == simulador_simular_evento(s,ObtenerInformacionPokemonEnTratamiento,&info_poke),"El evento ObtenerInformacionPokemonEnTratamiento devuele ExitoSimulacion");
    pa2m_afirmar(strcmp(info_poke.nombre_pokemon,"rampardos")==0,"El pokemon con menor nivel es rampardos");
    pa2m_afirmar(strcmp(info_poke.nombre_entrenador,"lucas")==0,"El pokemon con menor nivel tiene como entrenador a lucas");
    
    pa2m_nuevo_grupo("Separador");
    Intento intento;
    intento.nivel_adivinado = 67;
    ResultadoSimulacion retorno = simulador_simular_evento(s,AdivinarNivelPokemon,&intento);
    pa2m_afirmar(ExitoSimulacion == retorno,"El evento adivinar pokemon devuelve ExitoSimulacion");
    pa2m_afirmar(strcmp(intento.resultado_string,"Frio")==0,"El string resultado de la adivinacion es correcta");
    pa2m_afirmar(!intento.es_correcto,"Todavia el nivel no se adivino");

    
    intento.nivel_adivinado = 30;
    retorno = simulador_simular_evento(s,AdivinarNivelPokemon,&intento);
    pa2m_afirmar(ExitoSimulacion == retorno,"El evento adivinar pokemon devuelve ExitoSimulacion");
    pa2m_afirmar(strcmp(intento.resultado_string,"Tibio")==0,"El string resultado de la adivinacion es correcta");
    pa2m_afirmar(!intento.es_correcto,"Todavia el nivel no se adivino");

    pa2m_nuevo_grupo("Separador");
    pa2m_afirmar(ExitoSimulacion == simulador_simular_evento(s,AtenderProximoEntrenador,NULL),"El evento AtenderProximoEntrenador devuele ExitoSimulacion");
    pa2m_afirmar(ExitoSimulacion == simulador_simular_evento(s,ObtenerInformacionPokemonEnTratamiento,&info_poke),"El evento ObtenerInformacionPokemonEnTratamiento devuele ExitoSimulacion");
    pa2m_afirmar(strcmp(info_poke.nombre_pokemon,"rampardos")==0,"El pokemon con menor nivel siguen siendo rampardos (aunque haya atendido a otro entrenador)");
    pa2m_afirmar(strcmp(info_poke.nombre_entrenador,"lucas")==0,"El pokemon con menor nivel tiene como entrenador a lucas");

    intento.nivel_adivinado = 15;
    retorno = simulador_simular_evento(s,AdivinarNivelPokemon,&intento);
    pa2m_afirmar(ExitoSimulacion == retorno,"El evento adivinar pokemon devuelve ExitoSimulacion");
    pa2m_afirmar(strcmp(intento.resultado_string,"Caliente")==0,"El string resultado de la adivinacion es correcta");
    pa2m_afirmar(!intento.es_correcto,"Todavia el nivel no se adivino");

    intento.nivel_adivinado = 10;
    retorno = simulador_simular_evento(s,AdivinarNivelPokemon,&intento);
    pa2m_afirmar(ExitoSimulacion == retorno,"El evento adivinar pokemon devuelve ExitoSimulacion");
    pa2m_afirmar(strcmp(intento.resultado_string,"Nivel Adivinado Felicidades")==0,"El string resultado de la adivinacion es correcta");
    pa2m_afirmar(intento.es_correcto,"El nivel se adivino");
    
    pa2m_nuevo_grupo("Separador");
    pa2m_afirmar(ExitoSimulacion == simulador_simular_evento(s,ObtenerInformacionPokemonEnTratamiento,&info_poke),"El evento ObtenerInformacionPokemonEnTratamiento devuele ExitoSimulacion");
    pa2m_afirmar(strcmp(info_poke.nombre_pokemon,"toxicroak")==0,"El siguiente pokemon con menor nivel es toxicroak");
    pa2m_afirmar(strcmp(info_poke.nombre_entrenador,"valen")==0,"El pokemon con menor nivel tiene como entrenador a valen");    

    pa2m_nuevo_grupo("Separador");
    pa2m_afirmar(ExitoSimulacion == simulador_simular_evento(s,ObtenerEstadisticas,&e),"El evento ObtenerEstadisticas devuele ExitoSimulacion");
    pa2m_afirmar(e.pokemon_atendidos==1,"La cantidad de pokemones atendidos actualmente es 0");
    pa2m_afirmar(e.puntos==88,"La cantidad de puntos son 88 (4 intentos)");
    
    DatosDificultad nueva_dificulti;
    nueva_dificulti.nombre = "Muy dificil";
    nueva_dificulti.calcular_puntaje = calcular_puntaje_muy_dificil;
    nueva_dificulti.verificar_nivel = verificar_nivel_muy_dificil;
    nueva_dificulti.verificacion_a_string = verificacion_a_muy_dificil;

    pa2m_nuevo_grupo("Separador");
    ResultadoSimulacion nuevo_retorno = simulador_simular_evento(s,AgregarDificultad,&nueva_dificulti);
    pa2m_afirmar(ExitoSimulacion == nuevo_retorno,"El evento AgregarDificultad devuele ExitoSimulacion");
    
    InformacionDificultad info_dif;
    info_dif.id = 2;
    nuevo_retorno = simulador_simular_evento(s,ObtenerInformacionDificultad,&info_dif);
    pa2m_afirmar(ExitoSimulacion == nuevo_retorno,"El evento ObtenerInformacionDificultad devuele ExitoSimulacion");
    pa2m_afirmar(strcmp(info_dif.nombre_dificultad,"Muy dificil")==0,"El nombre de la dificultad agregada recientemente es muy dificil");
    pa2m_afirmar(!info_dif.en_uso,"Dificultad todvia no esta en uso");

    pa2m_nuevo_grupo("Separador");
    int cero = 0;
    nuevo_retorno = simulador_simular_evento(s,SeleccionarDificultad,&cero);
    pa2m_afirmar(nuevo_retorno == ExitoSimulacion,"El evento SeleccionarDificultad devuelve ExitoSimulacion");

    info_dif.id = 0;
    nuevo_retorno = simulador_simular_evento(s,ObtenerInformacionDificultad,&info_dif);
    pa2m_afirmar(ExitoSimulacion == nuevo_retorno,"El evento ObtenerInformacionDificultad devuele ExitoSimulacion");
    pa2m_afirmar(strcmp(info_dif.nombre_dificultad,"Facil")==0,"El nombre seleccionada recientemente es facil");
    pa2m_afirmar(info_dif.en_uso,"Dificultad facil esta en uso");

    info_dif.id = 54;
    nuevo_retorno = simulador_simular_evento(s,ObtenerInformacionDificultad,&info_dif);
    pa2m_afirmar(ExitoSimulacion == nuevo_retorno,"El evento ObtenerInformacionDificultad devuele ExitoSimulacion");
    pa2m_afirmar(!info_dif.nombre_dificultad,"El nombre de la dificultad invalida es NULL");
    pa2m_afirmar(info_dif.id,"Dificultad invalida tiene id -1");

    pa2m_nuevo_grupo("Separador");
    int dos = 2;
    nuevo_retorno = simulador_simular_evento(s,SeleccionarDificultad,&dos);
    pa2m_afirmar(nuevo_retorno == ExitoSimulacion,"Puedo seleccionar la nueva dificultad agregada");

    info_dif.id = 2;
    nuevo_retorno = simulador_simular_evento(s,ObtenerInformacionDificultad,&info_dif);
    pa2m_afirmar(ExitoSimulacion == nuevo_retorno,"El evento ObtenerInformacionDificultad devuele ExitoSimulacion");
    pa2m_afirmar(strcmp(info_dif.nombre_dificultad,"Muy dificil")==0,"El nombre seleccionada recientemente es Muy dificil");
    pa2m_afirmar(info_dif.en_uso,"Dificultad Muy dificil esta en uso");

    pa2m_nuevo_grupo("Separador");
    intento.nivel_adivinado = 67;
    retorno = simulador_simular_evento(s,AdivinarNivelPokemon,&intento);
    pa2m_afirmar(ExitoSimulacion == retorno,"El evento adivinar pokemon devuelve ExitoSimulacion");
    pa2m_afirmar(strcmp(intento.resultado_string,"Me gustan los charmander shiny")==0,"El string resultado de la adivinacion es correcta");
    pa2m_afirmar(!intento.es_correcto,"Todavia el nivel no se adivino");

    intento.nivel_adivinado = 30;
    retorno = simulador_simular_evento(s,AdivinarNivelPokemon,&intento);
    pa2m_afirmar(ExitoSimulacion == retorno,"El evento adivinar pokemon devuelve ExitoSimulacion");
    pa2m_afirmar(strcmp(intento.resultado_string,"Groudon en los volcanes")==0,"El string resultado de la adivinacion es correcta");
    pa2m_afirmar(!intento.es_correcto,"Todavia el nivel no se adivino");

    intento.nivel_adivinado = 20;
    retorno = simulador_simular_evento(s,AdivinarNivelPokemon,&intento);
    pa2m_afirmar(ExitoSimulacion == retorno,"El evento adivinar pokemon devuelve ExitoSimulacion");
    pa2m_afirmar(strcmp(intento.resultado_string,"Bienvenido al salon de la fama")==0,"El string resultado de la adivinacion es correcta");
    pa2m_afirmar(intento.es_correcto,"El nivel se ha adivinado");

    pa2m_afirmar(ExitoSimulacion == simulador_simular_evento(s,ObtenerInformacionPokemonEnTratamiento,&info_poke),"El evento ObtenerInformacionPokemonEnTratamiento devuele ExitoSimulacion");
    pa2m_afirmar(strcmp(info_poke.nombre_pokemon,"charizard")==0,"El siguiente pokemon con menor nivel es charizard");
    pa2m_afirmar(strcmp(info_poke.nombre_entrenador,"lucas")==0,"El pokemon con menor nivel tiene como entrenador a lucas");    

    pa2m_nuevo_grupo("Separador");
    pa2m_afirmar(ExitoSimulacion == simulador_simular_evento(s,ObtenerEstadisticas,&e),"El evento ObtenerEstadisticas devuele ExitoSimulacion");
    pa2m_afirmar(e.entrenadores_atendidos==2,"La cantidad de entrenadores atendidos actualmente es 2");
    pa2m_afirmar(e.pokemon_atendidos==2,"La cantidad de pokemones atendidos actualmente es 2");
    pa2m_afirmar(e.pokemon_en_espera==6,"La cantidad de pokemones en pokemon_en_espera actualmente es 6");


    hospital_destruir(h);
    simulador_destruir(s);
}

int main(){
    
    /*
    pa2m_nuevo_grupo("Pruebas de  creación y destrucción");
    puedoCrearYDestruirUnHospital();

    pa2m_nuevo_grupo("Pruebas con NULL");
    dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema();

    pa2m_nuevo_grupo("Pruebas con un archivo vacío");
    dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con un archivo de un entrenador");
    dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con un archivo de varios entrenadores");
    dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con mas de un archivo");
    dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital();
    */
    
    pa2m_nuevo_grupo("Pruebas con el simulador de pokemones");
    PruebasSimulador();


    return pa2m_mostrar_reporte();
}
