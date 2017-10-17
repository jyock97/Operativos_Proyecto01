

# Documentación
# Proyecto 1
# Principios de Sistemas Operativos
## Prof. Kevin Moraga
## Julio Rojas 2015090806
## Jose Paulo Yock 2015

## Introducción:
En este proyecto, se nos pide implementar la biblioteca pthreads,con el nombre de my_thread, además de esto, hay que implementar tres tipos de schedulers(RoundRobin, sorteo, tiempo real).

Utilizando esta biblioteca, se debe crear un tecRoyale, el cuál es un juego basado en Clash Royale pero para terminal.
El juego consta de dos pantallas, con la mitad de la arena en cada pantalla.
Deben de haber al menos diez guerreros, con stats como tipo, nivel, ataque, vida.
Para generar los guerreros de la mano, se debe utilizar un algoritmo genético.

## Ambiente de desarrollo:
Para realizar este proyecto se utilizó el lenguaje de programación C.
Entre las estructuras de datos más importantes se encuentra un arreglo para almacenar los hilos, un arreglo de booleanos para saber el estado de cada hilo, un arreglo para saber la prioridad de los hilos y un último para conocer la cantidad de tiquetes que tiene cada hilo.

Entre las funciones se encuentran:

- my_thread_create: La cual recibe como parámetros, función a ejecutar, parámetros necesarios, tiquetes iniciales y prioridad. Esta función crea un hilo y lo agrega a la cola del scheduler.

- my_thread_end: La cúal se encarga de finalizar un hilo.

- my_thread_yield: Esta función activa el scheduler y cede el procesador

- run_threads: Esta función activa el scheduler y pone a correr los hilos.

- my_mutex_init: Esta función inicializa el mutex, recibe como parametro el puntero a la variable mutex.

- my_mutex_lock: Esta función recibe como parámetro elmutex a bloquear y se encarga de bloquear el recurso siguiente en el texto

- my_mutex_destroy: Esta función finaliza el mutex.

- my_mutex_unlock: Esta función desbloquea el recurso y locolocacomo disponible

- my_mutex_trylock: Esta función bloquea el recurso, pero si este no está disponible no se bloquea.

- my_th_chsched: Función que se encarga de cambiar el scheduler activo.

## Instrucciones para ejecutar el programa:

Para ejecutar el código se presenta un archivo makefile, por o quese debe ejecutar:
```
$make all
$make run
```
O también, despes de hacer el primer comando, se puede iniciar con:
```
$./tecRoyale <archivo ini>
```

## Actividades realizadas por estudiante:

#### Julio Rojas:

- 24 de setiembre: Se comienza a leer sobre la implementación de los hilos. Horas: 7

- 25 de setiembre: Se hacen pruebas con setjmp y longjmp. Horas: 4

- 30 de setiembre: Se Hacen pruebas con context. Horas: 4

- 30 de setiembre: Se hacen pruebas con Clone. Horas: 4

- 1 de octubre: Se realiza un código base con context. Horas: 3

- 2  de octubre: Se agregan las primeras funciones de la biblioteca. Horas: 6

- 6 de octubre: Se investiga funcionamiento del scheduler. Horas: 4

- 7 de octubre: Se realiza el scheduler RoundRobin. Horas: 4

- 8 de octubre: Se realizan los schedulers de tiempo real y sorteo. Horas: 7

- 9 de octubre: Se investiga sobre el funcionamiento de los mutex. Horas: 6

- 12 de octubre: Se implementan los mutex. Horas: 5

- 13 de octubre: Se resuelven errores en el código. Horas: 3

- 13 de octubre: Se implementa algoritmo genético del juego. Horas: 3

- 14 y 15 de octubre: Haciendo debug al código buscando un error de memoria. Horas: 10

- 16 de octubre: Se realiza de nuevo el algoritmo genético y se corrige el error. Horas: 2

- 16  de octubre: Se incorpora el código del juego con los hilos propios. Horas: 2

Horas totales: 79


## Comentarios finales (estado del programa):

El proyecto se logra finalizar casi en su totalidad, con excepciones en algunos casos.
- No se implementan los métodos: my_thread_join y detach, principalmente por que no son necesarios para la implementación del juego.

El juego presenta un error cuando se envian varios mensajes, lo cúal no se pudo corregir.

## Conclusiones y Recomendaciones del proyecto.

En este proyecto, la parte de la implementación de los hilos fue muy pesada, por lo que se recomienda empezar por ahi. El tema es complicado para encontrar ejemplos sencillos, por lo que se recomienda tener paciencia, leer mucho, ver muchos ejemplos y tratar de entender todos estos.

## Bibliografı́a
- Multitasking using setjmp, l. (2017). Multitasking using setjmp, longjmp. Stackoverflow.com.  17 de octubre 2017, obtenido de  https://stackoverflow.com/questions/2560792/multitasking-using-setjmp-longjmp

- mfiber: Simple User Level Thread Library.. (2017). LID: Lost In Droid. 17 de octubre 2017, obtenido de https://machiry.wordpress.com/2012/01/02/mfiber-simple-user-land-thread-library/

- Implementing a Thread Library on Linux (evanjones.ca). (2017). Evanjones.ca. 17 de octubre 2017, obtenido de http://www.evanjones.ca/software/threading.html

- Implementing a Thread Library on Linux (evanjones.ca). (2017). Evanjones.ca. 17 de octubre 2017, obtenido de http://www.evanjones.ca/software/threading.html

- INI file. (2017, October 11). In Wikipedia, The Free Encyclopedia. 17 de octubre 2017, obtenido de https://en.wikipedia.org/w/index.php?title=INI_file&oldid=804815470

- Manual de linux:
```
$man longjmp
$man context
$man sched.h
```

- GitHub:

https://github.com/brianwatling/libfiber
https://gist.github.com/DanGe42/7148946
