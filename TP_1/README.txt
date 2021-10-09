1-  Una breve explicación de la solución implementada.

En primer lugar, el trabajo implementa funciones que en conjunto permiten leer y escribir archivos del salón de la fama
pokémon. El trabajo contiene implementaciones de funciones que facilitan el manejo de vectores dinamicos que sirivieron para el desarrollo de otras funciones mas complejas que le dieron un uso util a estas funciones que manejan memoria dinamica. 
Entre ellas se encuentran funciones basicas como vtrlen, vtradd (cuyo desarrollo esta explicado en el trabajo), cuyo uso se da en casi todo el resto de las funciones mas complejas, entre las cuales se encuentran funciones como "split" cuya finalidad consiste en parsear lineas de texto, y funciones como "fgets_alloc" que manejan memoria dinamica integrando conceptos sobre manejo de archivos de texto.
Por otra parte, se da a conocer lo que se conoce como API (Application Programming Interface) que consiste en una interfaz que une nuestra implementación y el programador que la va a utilizar. Con tan solo invocar alguna de nuestras funciones se pueden realizar determinadas operaciones que se engloban en la lectura y escritura de archivos del salon de la fama pokemon. A continuacion se muestran las operaciones que se puedan realizar mediante la implementacion de las funciones mencinoadas y que le dan uso a la mencionada API: 

- Crear un salón usando el archivo 'salon_original.sal'
- Obtener los entrenadores con al menos 3 ligas ganadas y mostrarlos por pantalla.
- Agregar 2 entrenadores al salon con 5 y 7 victorias respectivamente.
- Obtener los entrenadores con al menos 5 ligas ganadas y mostrarlos por pantalla.
- Guardar el salón a un nuevo archivo 'salon_modificado.sal'
- Salir con valor de retorno 0

2- Heap y stack: ¿Para qué y cómo se utiliza cada uno?

El heap permite almacenar variables adquiridas dinámicamente (mediante funciones como malloc o calloc) durante la ejecución de un programa mientras que el stack permite almacenar argumentos y variables locales durante la ejecución de
las funciones definidas en su ambito. A la hora de ejecutar un programa en memoria desde el disco duro unicamente los
datos de la zona estatica son creados, sin embargo, el heap y el stack son creados dinamicamente durante la ejecución del mismo programa.
El heap como esta reservada a la llamda memoria dinamica la misma es memoria que puede estar disponible pra ser utilada por el programador en tiempo de ejecucion. 
En el caso del stack, como se trata de un  un bloque de memoria fija cuyo tamaño no varia a lo largo de 
la ejecucion de nuestro programa, podemos hablar de "pilas de llamadas" para referirnos a estructura dinámica de datos que almacena la información sobre las subrutinas activas de un programa en ejecución. Por ejemplo, al hacer  una llamada a una función, un bloque en la cima de la pila es reservado para guardar las variables locales junto con algunos datos necesarios para asegurar el correcto funcinoamiento de la estructura . Después de retornar, el bloque de la pila que ocupaba el llamado, se libera sola para poder utilizarse en un futuro en caso de ser necesario.

3- Malloc/Realloc/Free: ¿Cómo y para qué se utilizan?

Malloc:

Esta funcion se usa para asignar memoria dinamica del heap a un puntero declarado previamente en el stack, su uso sigue el siguiente comando:

void* puntero = malloc(tamanio_bytes); 

Aca lo que hago es asignar a un puntero declarado como puntero, un puntero a la memoria reservada, para lo cual malloc reserva la cantidad de bytes que se le pasa como parametro (entre este caso "tamanio_bytes"). El malloc basicamente se utiliza para decirle al sistema opeartiva que se desea utilizar memoria del heap.


Realloc:

La funcion realloc() modifica el tamaño del bloque de memoria apuntado por ptr en size bytes, el contenido del bloque de memoria no se modificara desde el inicio del mismo hasta el mìnimo entre el viejo y el nuevo tamaño. En caso de que el tamaño sea mayor a la anterior, la memoria añadida no se va a encontrar incializada, y en caso de que ptr sea NULL entonces la llamada es igual a malloc(size), siendo size cualquier valor.

puntero = realloc(ptr, tamanio_bytes*numero), siendo numero mayor que 0.

Al ejecutarse esto y habiendo hecho previamente un malloc por ejemplo resultara que el puntero de tamaño "tamaño_bytes" pase a tener el tamaño de "numero" veces el tamaño original sin perder el contenido que almacenaba orginalmente. Al agrandar el espacio reservado en memoria (en caso de que el realloc no devuelva NULL), puedo usar ese espacio adicional disponible.

Free:

La funcion free libera espacio de memoria apuntado por el puntero (ptr) que debe haber sido devuelto previamente por una llamda a la funcion malloc(), calloc(), o realloc(). Esto se realiza a fin de evitar llenar nuestra memoria con datos que ya no nos son util. El uso de free sigue el siguiente comando: 

free(puntero);

