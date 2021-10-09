#include "util.h"
#include<string.h>

#define TAMANIO_BUFFER 2048
#define STRING_VACIO ""

size_t vtrlen(void* ptr){

	size_t cant_punteros = 0;

	if(ptr == NULL){
	return 0;
	}

	void**ptr_aux = ptr;

	int i = 0;

	while((ptr_aux[i]) != NULL)
	{
	cant_punteros++;
	i++;
	}

	return cant_punteros;
	}

void* vtradd(void* ptr, void* item){


	size_t cant_elementos_actual;

	cant_elementos_actual = vtrlen(ptr);

	void** ptr_aux = realloc(ptr,(2+cant_elementos_actual)*sizeof(item));
	void** otro_ptr = ptr_aux;

	if(!ptr_aux){
		return NULL;
	}

	otro_ptr = otro_ptr + cant_elementos_actual;
	(*otro_ptr) = item;
	(otro_ptr)++;
	(*otro_ptr) = NULL;

    return ptr_aux;
}

void vtrfree(void* ptr){
	if(ptr == NULL){
		return;
	}
	void** aux = ptr;
	size_t cant_punteros = vtrlen(ptr);

	if(cant_punteros == 0){
		return;
	}

	for(int i = 0; i < cant_punteros;i++){
		if((strcmp((aux[i]),STRING_VACIO)) != 0){
			free(aux[i]);
		}
	}
	free(aux);
}

/*
Pre condiciones : La funcion recibe un puntero a un string que es distinto de NULL y fue previamente inciailizado,que en este caso seria el lugar en donde se almacenaria el string, 
un puntero a otro char que en este caso seria una cadena de caracteres, un entero y un caracter tambien inicializados.
Post condiciones : La funcion  devuelve el puntero a un string pero con el primer caracter de la cadena (str) almacenada, formandose asi, una especie de 
subcadena a partir de la cadena oringial.
*/

char* extraer_subtring(const char* str,int i,char separador,char *sub_str){
  	
  	if(!str){
		return NULL;
	}

  char* str_aux = NULL;
  size_t k = 0;
  if(sub_str != NULL){
    k = strlen(sub_str);
  }
  if(str[i] != separador){

    str_aux = realloc(sub_str,(k+2));

    if(!str_aux){
      return NULL;
    }
    sub_str = str_aux;
    sub_str[k] = str[i];
    k++;
    sub_str[k] = '\0';
  }

  return sub_str;
}


char** split(const char* str, char separador){
	char** vector_ptr = NULL;
	char* sub_str = NULL;
  	char* str_aux = NULL;
  	str_aux	= STRING_VACIO;
  	int i = 0;

	if(!str){
		return NULL;
	}
	if(strcmp(str,"\0") == 0){
  		return NULL;
  	}

  	while(str[i] != '\0'){

	    if(str[i] != separador){
	    	sub_str = extraer_subtring(str,i,separador,sub_str);
	    	if(!sub_str){
	    		return NULL;
	    	}
	    }
	    else {

			if(sub_str == NULL){

				vector_ptr = vtradd(vector_ptr,str_aux);
			}
	    	vector_ptr = vtradd(vector_ptr,sub_str);
	       	sub_str = NULL;

	      }

	   	i++;
  	}

  	if(str[i-1] == separador){
        vector_ptr = vtradd(vector_ptr,str_aux);
    }

  	vector_ptr = vtradd(vector_ptr,sub_str);
  	
    return vector_ptr;

}

/**
 * Lee una linea completa de un archivo y devuelve un puntero al string leido.
 *
 * El string devuelto debe ser liberado con malloc.
 */

char* leer_dinamico(FILE *archivo){

    size_t bytes_leidos = 0;
    size_t tamanio = TAMANIO_BUFFER;
    char* buffer = malloc(tamanio);

    if(!buffer)
        return NULL;

    while(fgets(buffer+bytes_leidos,(int)(tamanio+bytes_leidos),archivo)){

        size_t leido = strlen(buffer+bytes_leidos);
        if(leido > 0 && *(buffer+bytes_leidos+leido-1) == '\n'){
            *(buffer+bytes_leidos+leido) = '\0';
            return buffer;
        }else{

            char* aux = realloc(buffer,tamanio*2);
            if(!aux){
                free(buffer);
                return NULL;
            }
            buffer = aux;
            tamanio *= 2;
        }
        bytes_leidos += leido;
    }

    if(bytes_leidos == 0){
        free(buffer);
        return NULL;
    }

    return buffer;
    

}

char* fgets_alloc(FILE* archivo){


	if(!archivo){
		return NULL;
	}

	char* puntero_a_string = leer_dinamico(archivo);
	printf("%s\n",puntero_a_string);

    return puntero_a_string;

    
}

void fclosen(FILE* archivo){

	if(archivo != NULL){
		fclose(archivo);
	}
}


