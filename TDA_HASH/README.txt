1) Una tabla de hash es una estructura que contiene valores, los cuales estan identificados univocamente mediante claves que las distinguen de otros valores. La particularidad de una tabla de hash es que como los elementos estan vinculados a sus respectivas claves, la ubicacion de los elementos dentro de la tabla esta dada por una funcion de hash. Dicha funcion transforma claves en un numero asociado dentro del tamaño de la tabla. Cabe destacar que pueden haber mas claves que espacios en la tabla de hash y como consecuencia de esto, claves distintas pueden dar como resultado un mismo valor en el hash.
Las tablas se hash se dividen en dos grandes grupos: por un lado se encuentra el hash abierto, tambien conocido como hash con direccionamiento cerrado, y por el otro se encuentra el hash cerrado, tambien conocido como hash con direccionamiento abierto.
En los hash abiertos, existe lo que se llama "chaining" o "encadenamiento" en donde las claves (con su respectivo valor) se almacenan en listas enlazadas unidas a celdas de una tabla hash, por lo que en caso de que colisionen dos claves distintas lo que se hace simplemente es encadenar el par clave-valor a la lista correspondiente a la posicion dada por la funcion de hash.
En los hash cerrados, todos los valores se almacenan dentro de la misma tabla, y a diferencia de los hash abiertos en caso de que haya colision se sigue recorriendo la tabla hasta encontrar el proximo espacio libre en el cual insertar el valor. Por dicho motivo el direccionamiento es abierto.

2)

En este trabajo se implemento un hash cerrado con direccionamiento abierto y se aplicaron los siguientes criterios:

Resolucion de colisiones: Se determino que hay colision dentro de una tabla de hash cerrada si la funcion de hash me devuelve una posicion en la tabla en la que este ocupada, es decir, ya hay un elemento almacenado en dicha posicion y que la clave de la misma no coincida con la clave del elemento a insertar.

En caso de que haya colision lo que se hace es un probing lineal. Este procedimiento busca una posicion libre dentro de la tabla de hash buscando el siguiente espacio inmediato. Si la busqueda llega al final se resetea la busqueda desde el principio de la tabla para seguir buscando linealmente una posicion libre.

Criterio de redimensionamiento: Lo que tuve en cuenta para resolver las colisiones fue la relacion entre la cantidad de elementos que hay en la tabla con el tamaño de la misma, para lo cual uso la funcion "calcular_factor_de_carga" para ir continuamente chequeando si hay que rehashear la tabla. En caso de que se
dicha funcion de mayor o igual a FACTOR_DE_CARGA, se agranda la tabla para rehashearla.

Para el criterio del borrado se eligio optar por reemplazar en los lugares borrados un flag que indique que en dicha posicion hubo un elemento y se borro.

Para el iteardor de hash se decidio crear una estructura aparte llamada "hash_iter_t" que apunte al primer elemento valido de la tabla de hash y que vaya iterando sobre los elementos de la misma. Para implementar esto me base en el iterador de TDA_lista implementado anteriormente. 
