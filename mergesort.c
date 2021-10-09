#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void merge(void* _v1, size_t c1, void* _v2, size_t c2,size_t tam_elemento,int (*comparador)(void*,void*)){
	char resultado[(c1+c2)*tam_elemento];
	size_t i1 = 0,i2=0,ir=0;

	char* v1 = _v1;
	char* v2 = _v2;
	
	while(i1 < c1 && i2 < c2){
		
		if(comparador(v1+(i1*tam_elemento),v2+(i2*tam_elemento))<0){
			memcpy(resultado+(ir*tam_elemento),v1+(i1*tam_elemento),tam_elemento);
			ir++;
			i1++;
		}else if(comparador(v1+(i1*tam_elemento),v2+(i2*tam_elemento))>0){
			memcpy(resultado+(ir*tam_elemento),v2+(i2*tam_elemento),tam_elemento);
			ir++;
			i2++;
		}else{
			memcpy(resultado+(ir*tam_elemento),v1+(i1*tam_elemento),tam_elemento);
			ir++;
			i2++;
			i1++;
		}

	}
	
	while(i1<c1){
		memcpy(resultado+(ir*tam_elemento),v1+(i1*tam_elemento),tam_elemento);
		ir++;
		i1++;
	}

	while(i2<c2){
		memcpy(resultado+(ir*tam_elemento),v2+(i2*tam_elemento),tam_elemento);
		ir++;
		i2++;
	}

	memcpy(v1,resultado,(c1+c2)*tam_elemento);
		
}



void mergesort(void* vector, size_t cantidad,size_t tam_elemento,int (*comparador)(void*,void*)){
	
	if(cantidad <= 1)
		return;
	
	size_t mitad = cantidad/2;
	
	mergesort(vector,mitad,tam_elemento,comparador);
	mergesort((char*)vector+(mitad*tam_elemento),cantidad-mitad,tam_elemento,comparador);	

	merge(vector,mitad,(char*)vector+(mitad*tam_elemento),cantidad-mitad,tam_elemento,comparador);
}


int comparador_strings(void* _s1, void* _s2){
	char** s1 = _s1;
	char** s2 = _s2;
	return strcmp(*s1,*s2);
}

int comparador_enteros_mayor(void* _i1, void* _i2){
	int* i1 = _i1;
	int* i2 = _i2;	
	return (*i1 - *i2);
}

int main(){

	char* vectors[] = {"Lucas","Mariano","Daniel","Gabriel","Mauro","Leandro"};
	size_t ns = sizeof(vectors)/sizeof(char*);
	
	int vectori[] = {5,7,2,1,8,0,4,3,34,67,12,23};
	size_t ni = sizeof(vectori)/sizeof(int);

	printf("Antes de ordenar\n");
	for(size_t i = 0; i < ns;i++){
		printf("%s ",vectors[i]);
	}
	printf("\n");

	for(size_t i = 0; i < ni;i++){
		printf("%i ",vectori[i]);
	}
	printf("\n");
	
	mergesort(vectors,ns,sizeof(char*),comparador_strings);
	mergesort(vectori,ni,sizeof(int),comparador_enteros_mayor);
	
	printf("Despues de ordenar\n");
	for(size_t i = 0; i < ns;i++){
		printf("%s ",vectors[i]);
	}
	printf("\n");

	for(size_t i = 0; i < ni;i++){
		printf("%i ",vectori[i]);
	}
	printf("\n");


	return 0;
}