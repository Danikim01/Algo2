#include "split.h"
#include <stdlib.h>


size_t contar_ocurrencias(const char* string, char separador){
	size_t contador = 0;
	size_t i = 0;
	while(string[i] != '\0'){
		if(string[i] == separador){
			contador++;
		}
		i++;
	}
	return contador;
}

size_t buscar_prox_separador(const char* string,char separador){
	size_t i = 0;
	while (string[i] != '\0' && string[i] != separador){
		i++;
	}
	return i;
}

char* duplicar_string(const char* string,size_t tamanio_substring){
	char* sub_string = malloc((tamanio_substring+1)*sizeof(char));
	if(!sub_string){
		return NULL;
	}
	for(size_t i = 0; i < tamanio_substring;i++){
		sub_string[i] = string[i];
	}

	sub_string[tamanio_substring] = 0;

	return sub_string;
}

char** split(const char* string, char separador){
	if(!string){
		return NULL;
	}
	size_t cantidad_ocurrencias = contar_ocurrencias(string,separador);
	size_t cantidad_substrings = cantidad_ocurrencias+1;

	char** vector = calloc(cantidad_substrings+1,sizeof(void*));
	if(!vector){
		return NULL;
	}

	for(size_t i = 0; i < cantidad_substrings;i++){
		size_t tamanio_substring = buscar_prox_separador(string,separador);
		char* sub_string = duplicar_string(string,tamanio_substring);
		vector[i] = sub_string;
		string += tamanio_substring+1;
	}
	
    return vector;
}
