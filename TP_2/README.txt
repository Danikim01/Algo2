En primer lugar voy a explicar bajo que criterios decidi usar cada estructura de datos presentes en el siguiente trabajo:
El salon contiene entrenadores, que a su vez tambien tienen pokemones y muchas otros datos. En el primer tp como todavia no se habian visto las distintas estructuras de datos, los entrenadores junto a los pokemones se almacenaban en vectores dinamicos. El uso de vectores dinamicos sirve siempre y cuando haya memoria contigua disponible, en caso contrario el vector no se va a poder redimensionar.
Sin embargo, al usar listas para los pokemones de los entrenadores ya no es estrictamente necesario que la memoria sea contigua y por otra parte, ciertas operaciones como el agregado o el borrado de elementos disminuyen considerablemente en complejidad algortmica.
Esto se debe a que por ejemplo para agregar un elemento a un vector dinamico se debe recorrer todo el vector por lo que la complejidad en el peor de los casos es O(n), siendo n la cantidad de elementos a recorrer. Para el caso de la listas no tiene por que pasar esto dado que una lista se trata de un contenedor que no solo va
a contener a un tipo de dato determinado, sino que también podría apuntar al próximo elemento y como la lista tiene un puntero al ultimo elemento y al primer elemento, para agregar un elemento simplemente hay que cambiar el puntero del ultimo elemento anterior al nuevo elemento, por lo que la complejidad del agregado se vuelve O(1).
Ahora bien, lo mismo pasa en caso de que se quiera almacenar entrenadores en vectores dinamicos. En el trabajo se empleo un arbol binario de busqueda de entrenadores que se almacenan dentro de un salon, lo que trae muchas ventajas comparado al uso de vectores dinamicos. 
La complejidad de busqueda en un arbol binario de entrenadores es mucho mas reducida que una busqueda en un vector dinamico de entrenadores. Pasando de un complejidad de O(n) a una complejidad mucho menor de O(log n) en promedio. Esto sucede gracias a que el arbol (binario de busqueda) tiene un comparador de elementos,lo permite distribuir a los entrenadores segun un determinado criterio. Esto fue especialmente util para los comandos que pedian mostrar/devolver a los entrenadores segun un determinado criterio. En el trabajo se nos pedia mostrarlos alfabeticamente. 
A su vez se decidi usar una tabla de hash para almacenar especificamente las reglas dentro de la estructura del salon, de manera que, cada regla tiene asociado univocamente una descripcion. El hecho de tener registrado dentro del salon las reglas me permitio chequear facilmente que la regla que envia como parametro a la funcion ejecutar_comando sea una regla que este efectivamente en el sistema y no sea otra. En otras palabras, si no tuviera una tabla de hash tendria que estar chequeando mediante desprolijos if/elseif/else continuos que la regla que me llega sea la correcta. Por otra parte, tener las reglas en una tabla de hash me facilita mostrar la descripcion asociada a cada regla particular, mediante un simple hash_obtener (ver funcion "reglas_de_batalla"). 
Por su parte, por un motivo bastante similar decidi usar una tabla de hash para comandos, pese a que hubo un pequeño inconveniente dado que inicialmente queria usar una tabla de hash para los comandos para evitar los continuos if/elseif que se realizaban dentro de "salon_ejecutar_comando" para chequear que comando que me llegaba sea correcto, sin embargo, conclui que era de alguna forma muy dificil evitar eso dado que en una tabla de hash todo elemento tiene asociado a una clave unica y en el caso de este trabajo ,por ejemplo,el comando ENTRENADOR puede recibir muchos parametros diferentes como "ENTRENADOR:victorias,n" o "ENTRENADOR:pokemon,nombre" por lo que no pude extraer una clave univoca a partir del comando que me llega por parametro. Entonce opte por asociar a cada comando a una clave univoca a partir del cual, el programa reconozca que hacer cuando le llega esa clave univoca que yo le asigne (esto se ve en la funcion "procesar_comando" y en "salon_crear").

En segundo lugar voy a explicar las decisiones de diseño: 
En el trabajo se van a poder notar algunas funciones que se implementaron en salon.c que no se usaron dentro de salon.c (como 'int victorias_entrenador' o 'char* nombre_entrenador')sino que se usaron para las pruebas de las funciones dentro de salon.c dado que en los test estaba prohibido el aceso directo a las estructuras utilizadas.

Por otra parte, tuve que modificar una pequeña parte de la funcion lista_borrar_en_posicion que se uso para el trabajo dado que previo a la modificacion cuando quitaba pokemones y luego los agregaba me genera lecturas invalidas. 
Asi lucia la funcion previo al cambio:

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
    if(!lista || lista_vacia(lista)){
        return ERROR;
    }
    if (posicion==0){
        nodo_t* nodo_a_eliminar = lista->nodo_inicio;
        lista->nodo_inicio = nodo_a_eliminar->siguiente;
        free(nodo_a_eliminar);
        lista->cantidad--;
        return EJECUCION_CORRECTA;
    }else if((int)posicion>((lista->cantidad)-1) || (int)posicion < 0){
        int eliminacion = lista_borrar(lista);
        return eliminacion;
    } else {
        nodo_t* nodo_aux = lista->nodo_inicio;
        avanzar_lista(&(nodo_aux),(int)posicion,0);
        nodo_t* nodo_anterior = lista->nodo_inicio;
        avanzar_lista(&(nodo_anterior),(int)posicion-1,0);
        nodo_anterior->siguiente = nodo_aux->siguiente;
        free(nodo_aux);
        lista->cantidad--;
    }
   
    return EJECUCION_CORRECTA;
}

y asi despues:

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
    if(!lista || lista_vacia(lista)){
        return ERROR;
    }
    if(posicion>=((lista->cantidad)-1)){
        int eliminacion=lista_borrar(lista);
        return eliminacion;
    }
    nodo_t* aux=lista->nodo_inicio;
    if (posicion==0){
        lista->nodo_inicio=aux->siguiente;
        free(aux);      
    }else{
        avanzar_lista(&aux,(int)posicion-1,0);
        nodo_t* nodo_a_borrar=aux->siguiente;
        aux->siguiente=nodo_a_borrar->siguiente;
        free(nodo_a_borrar);
    }
    (lista->cantidad)--;
    return EJECUCION_CORRECTA;
}

En tercer lugar me gustaria comentar algunas dificultades que tuve en el tp, y que aun no pude resolver:
Hubieron dificultades en el intento de probar ciertas cosas como chequear que un pokemon se haya efectivamente agregado/eliminado de la lista de pokemones de cierto entrenador, para lo cual implemente en salon.c una funcion que me devuelva la lista de pokemones de cierto entrenador. Trate de acceder la lista de pokemones en pruebas.c filtrando los entrenadores para obtener un entrenador mediante salon_filtrar_entrenadores con la funcion es_entrenador y luego usando ese elemento obtenido para hallar su lista de pokemones, pero al tratar de acceder a un pokemon en particular de esta forma desde pruebas.c me generaba lecturas invalidas. Comento este problema por este medio porque considero que no puedo dejar pasar por alto este tipo de errores (todavia me cuesta comprender bien como comprender los errores de lectura invalida)

Otro inconveniente que tuve fue probando el programa con el server. Cuando trato de simular una batalla entre dos entrenadores, se eliminan los pokemones que perdieron la batalla en el respectivo enfrentamiento. No se si esto es normal, dado que el salon debe ser inmutable y yo lo estoy mutando de alguna manera.
