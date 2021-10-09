#include "strutil.h"



size_t vtrlen(void* ptr){

	size_t cant_punteros = 0;
	if(ptr == NULL){
		return 0;
	}
	void**ptr_aux = ptr;

	while(*ptr_aux)
	{
		cant_punteros++;
		ptr_aux++;
	}

	return cant_punteros;
}


char* agregar_texto(char* original,char* a_agregar){
	size_t largo = 0;
	if(original != NULL){
		largo = strlen(original);
	}
	char* str = realloc(original,largo+strlen(a_agregar)+1);
	if(!str)
		return NULL;
	original = str;
	strcpy(original+largo,a_agregar);
	return str;
}


char* join(char** strv, char sep){
	if(!strv){
		return NULL;
	}
	size_t cantidad_substrings = vtrlen(strv);
	char* separador = calloc(1,sizeof(sep)+1);
	if(!separador){
		return NULL;
	}
	separador[0] = sep;
	char* string = NULL;
	for(size_t i = 0; i < cantidad_substrings;i++){
		string = agregar_texto(string,strv[i]);
		if(i != cantidad_substrings-1){
			string = agregar_texto(string,separador);
		}
	}
	free(separador);
	return string;
}


/*
void free_strv(void*ptr){
	if(ptr == NULL){
		return;
	}
	void** aux = ptr;
	size_t cant_punteros = vtrlen(ptr);

	if(cant_punteros == 0){
		return;
	}

	for(int i = 0; i < cant_punteros;i++){
		if((strcmp((aux[i]),"")) != 0){
			free(aux[i]);
		}
	}
	free(aux);
}
*/

void free_strv(char** cadena){
	if(!cadena)
		return;
	size_t cant_elementos = vtrlen(cadena);
	for(int i = 0; i < cant_elementos;i++){
		free(cadena[i]);
	}
	free(cadena);
}