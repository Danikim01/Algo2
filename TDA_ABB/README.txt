
Un arbol es una astraccion de una estructura de datos que es capaz de reducir el tiempo de acceso a los mimos, cuyo tiempo de acceso promedio a los datos es de O(logN) y no de O(n) como en el caso del tiempo de acceso de las listas enlazadas, lo cual la hace poco eficiente. Este tipo de dato, se caracteriza principalmente por su naturaleza altamente recursiva ya sea en sus diversas definiciones y en sus implementaciones.

Por dicho motivo, si se requiere por algun motivo trabajar con cantidades grandes o muy grande de datos es recomendable trabajar con un arbol. Por supuesto que existen muchos tipos de variante de esta abstraccion:

- Arbol N-arios o Generales. 
- Arbol Binario
- Arbol Binario de Busqueda
- Arbol AVL 
- Arbol Splay 
- Arbol B 
- Arbol B+ 
- Arbol B* 
- Arbol Rojo Negro

Un arbol ,en si mismo, se trata de un conjunto de nodos.Los nodos son los elementos o vertices del arbol. Por otra parte hay un nodo que se distingue del resto y se hace llamar nodo raiz (nodo r), a su vez un arbol consiste en muchos sub-arboles no vacios conectadas al nodo raiz. El nodo raiz de cada sub-arbol es denominado nodo hijo del nodo r (raiz), y r es el nodo padre de cada sub-arbol de r. 

Un arbol binario esta estrechamente relacionado con las operaciones de busqueda. En otras palabras, los arboles binarios justamente abstraen este comportamiento de busqueda introduciendo la nocion de una busqueda por ramas derecha e izquierda. Un arbol binario puede estar vacio o tener un nodo raiz en conjuncion con otros dos arboles binarios que le sigue : uno llamado derecha y otro llamado izquierda ambos respecto del nodo raiz.

A diferencia de un arbol binario comun existen otro tpo de arboles llamados "Arbol binario de busqueda". Un arbol binario de busqueda es un TDA conformado por un nodo que va a tener un puntero a un nodo a derecha y otro a izquierda, cada nodo teniendo un elemento. Hasta aca se trata de un arbol binario pero la caracteristica que lo diferencian de los arboles binarios es que los nodos hijos respecto a un nodo raiz se encuentran ordenados segun un criterio determiando por el TDA. Esto permite poder realizar la busqueda de elementos de forma eficiente, siendo esta, por lo general, O(log2 N). El criterio que voy a seguir en mi implementacion es que los elementos menores o iguales se encuentran a izquierda y los mayores se van a encontrar a la derecha.

En otras palabras, cada nodo del arbol binario de busqueda contiene un valor clave que satisface las siguientes condiciones:

1. la clave en el nodo izquierdo del hijo ( si existe) es menor que la clave en el nodo padre.
2. la clave en el nodo derecho del hijo (si existe) es mayor que la clave en el nodo padre
3. los arboles derecho e izquierdo son tambien arboles binarios de busqueda


Aclaracion breve el funcionamiento de lista_borrar por si el algoritmo resulta muy complejo sobre todo para el caso en que se quiera borrar un nodo que tiene dos hijos : 

En primer lugar, para el borrado de los nodos hojas, simplemente lo que se hace es destruir el nodo en cuestion y retornar NULL. Con esto se logra que el padre de dicho nodo hoja apunta a nada luego de haber borrado a su hijo nodo.

En tercer lugar, para el borrado de nodos con un hijo, lo que se hizo fue verificar que si el nodo a borrar tiene un hijo izquierdo me guardo en una variable auxiliar dicho nodo para no perder la referencia a dicho nodo, luego ,como lo tengo guardado en auxiliar, destruyo el nodo y retorno el hijo del nodo. Lo mismo para el caso en que el nodo tenga hijo derecho.

En tercer lugar, para la implementacion del borrando de nodos con dos hijos cualesquiera que sean, se penso en extraer al nodo predecesor inorden (el nodo cuya clave es la mayor de los menores) del nodo a eliminar en cuestion, esto significa guardar en una variable auxiliar al nodo predecesor inorden (sacarlo del arbol), dejar a sus hijos acomodados como corresponden. El nodo predecesor inorden siempre va a tener un hijo o 0 hijos, por lo tanto, es importante hacer que el nodo padre de dicho sucesor inorden quede apuntando a los hijos del nodo sucesor inorden. En caso de que dicho sucesor inorden tenga hijos, el padre del sucesor apuntara a los hijos, y en caso contrario, apuntara a NULL.

Luego de haber extraido el predecesor inorden, la idea es reemplazar el nodo a eliminar (que tiene dos hijos) con el nodo extraido, para luego borrar del arbol al nodo a eliminar. Para ello, hay reasignar punteros de forma tal que el predecesor inorden tenga como hijos a los hijos del nodo borrado.

Es pertiente aclarar que para el eliminado de nodos que tienen dos hijos se eligio el criterio de elegir el "predecesor inorden" este termino refiere al nodo con mayor valor de los nodos menores con respecto al nodo que quiero borrar. Dado que dependiendo del criterio con el cual se quiera decidir borrar nodos con dos hijos, la distribucion de nodos del arbol va a cambiar. Por ejemplo, si yo tengo un arbol de tres elementos (la raiz con dos hijos, uno a la derecha y otra a la izquierda) y quiero borrar la raiz, si se decida elegir el sucesor inorden, dicho nodo reemplaza a la raiz y consecuentemente se elimina el nodo raiz, quedando a su izquierda el nodo izquierdo del nodo raiz borrado. Si se decide elegir el "predecesor in orden", es decir, al nodo mayor de los menores, esta distrubucion cambia, dado que el nodo izquierdo a la raiz original reemplazaria su lugar.

