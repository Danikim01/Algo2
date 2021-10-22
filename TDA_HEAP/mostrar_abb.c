#include "mostrar_abb.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void mostrar_arbol(int* buffer, int n){
  printf("\n");
  int niveles = 0;
  for(int restantes=n;restantes>0;restantes>>=1)
    niveles++;

  int n_nodo=0;
  for(int nivel=0;nivel<niveles;nivel++){
    int posicion = 1<<(niveles-nivel-1);

    for(int nodo=0;nodo<(1<<nivel);nodo++){
      for(int i=1;i<posicion;i++){
        if(i<=posicion/2-1 || buffer[2*n_nodo+1]==0)
          printf("  ");
        else{
          if(i == posicion/2)
            printf(" ┌");
          else printf("――");
        }
      }

      if(buffer[n_nodo]!=0){
        int valor = buffer[n_nodo]<100?buffer[n_nodo]:99;
        printf("%2d",valor);
      }
      else{
        printf("  ");
      }

      for(int i=0;i<posicion;i++){
        if(i>=posicion/2 || buffer[2*n_nodo+2]==0)
          printf("  ");
        else{
          if(i == posicion/2-1)
            printf("―┐");
          else printf("――");
        }
      }
      n_nodo++;
    }
    printf("\n");
  }
}

