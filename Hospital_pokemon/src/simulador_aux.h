#ifndef SIMULADOR_AUX_H_
#define SIMULADOR_AUX_H_

#include "simulador.h"
#include "hospital_aux.h"
#include "lista.h"
#include "abb.h"
#include "hash.h"
#include "math.h"
#include <string.h>

typedef struct{
	int id_dificultad;
    DatosDificultad* nueva_dificultad;
}DatosDificultadAux;

struct  _simulador_t{
	hospital_t* hospital_pokemon;
	unsigned cantidad_entrenadores_atendidos;
	unsigned pokemon_atendidos;
	unsigned puntos;
    unsigned cantidad_eventos_simulados;
	abb_t* pokemones_en_espera;
	pokemon_t* pokemon_en_tratamiento;
	unsigned pokemon_en_espera;
	abb_t* arbol_niveles;
	int cantidad_niveles;
	DatosDificultadAux* dificultad_actual;
	bool simulacion_finalizada;
};

#endif // SIMULADOR_AUX_H_