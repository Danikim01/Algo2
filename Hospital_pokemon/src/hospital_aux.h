#ifndef HOSPITAL_AUX_H_
#define HOSPITAL_AUX_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "lista.h"
#include "abb.h"
#include "hospital.h"

struct _hospital_pkm_t{
    lista_t* entrenadores_en_sala;
    unsigned entrenadores_totales;
    unsigned pokemon_totales;
    char* texto_leido;
};

struct _pkm_t{
    char* nombre_de_su_entrenador;
    entrenador_t* entrenador_del_pokemon;
    char* nombre;
    size_t nivel;
    bool atendido;
    unsigned cantidad_intentos;
};

struct _entrenador_t{
    int id;
    char* nombre_entrenador;
    abb_t* arbol_pokemones;
    size_t cantidad_pokemones;
    bool entrenador_atendido;
};


#endif // HOSPITAL_AUX_H_
