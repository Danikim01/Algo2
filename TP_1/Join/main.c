#include "strutil.h"
#include "split.h"


void mostrar_elementos_spliteados(char** vector){
	size_t cant_elementos = vtrlen(vector);
	for(size_t i = 0; i < cant_elementos;i++){
		printf("%s\n",vector[i]);
	}
}

int main(){
	char* texto = "Split1;Split2;Split3;Split4;Split5;Split6;Split7";
	char separador = ';';
	printf("El texto original a splitear/joinear es: %s\n",texto);
	printf("Realizo el split\n");
	char** vector = split(texto,separador);
	mostrar_elementos_spliteados(vector);
	printf("Realizo el join\n");
	char* nuevo_texto = join(vector,separador);
	printf("%s\n",nuevo_texto);


	free(nuevo_texto);
	free_strv(vector);
	return 0;
}