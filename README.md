# Taller de Programación I
## Ejercicio N°2: Split-Apply-Combine
* Nombre: Federico Carlos Mangiaterra 
* Padrón: 105771 
* Link del repositorio de Github: https://github.com/FedeMangiaterra/Taller-de-Programacion-I-tp2
## Introducción
Este trabajo consiste en la implementacion en C++ de un programa capaz de tomar un dataset con numeros de determinado tamaño, dividirlo en una cantidad indicada de particiones, y realizar operaciones con ellas, para luego combinar los resultados y que queden como si las operaciones se hubiesen realizado sobre el archivo sin partir, de ahí el nombre del ejercicio. <br>
En este caso, el programa debe ser multithreading, por lo que las particiones deben ser procesadas de forma concurrente por una cantidad de hilos que se pasa por consola al ejecutar el programa. Para la resolucion del ejercició se usaron diversas clases repartidas en los siguientes archivos:
* File
* Command
* Result
* Thread
* Partition
* SAC(split-apply-combine)
## Resolución del trabajo
### Thread
El programa cuenta con N hilos que funcionan como workers, y un hilo más que es el encargado de leer por entrada estandar los comandos que se deben ejecutar. En el archivo thread.c se define la clase Thread, para poder hacer otras clases que hereden de ella, y que funcionen como objetos activos, funciones que se ejecutan sobre su propio hilo mediante el metodo run(). <br>
Los hilos funcionan de manera concurrente usando condition variables y una cola, siguiendo el modelo de productor y consumidor/es. Para llevar a cabo esto se implementa los objetos de Command y Command_queue.
### Command
Por entrada estandar, se indica la fila inicial y final, la columna sobre la cual se opera, la cantidad de filas por partición y la operación a realizar. La clase Command guarda todo eso y también el total de filas sobre las que se opera y el numero de comando (el orden en el cual cada comando se pasó por entrada estandar). Se usa un constructor de command que recibe toda la linea de entrada estandar y el numero de comando, y completa sus atributos. Pero lo que se manda por entrada estandar es la operación a realizar sobre todo el archivo. Para trabajar con las particiones, se "parte" tambien el comando(mediante el objeto activo Process_input, que hereda de Thread), dividiendolo en varios sub-comandos que cumplen que la fila final menos la inicial es menor o igual a la cantidad de filas por partición. Por ejemplo, una suma de la segunda columna entre la fila 1 y la 8, se divide en una suma asi pero entre las filas 1 y 3, 3 y 5, 5 y 7, y 8. <br>
Todos estos mini comandos se guardan en la Command_queue. Cuando ya no se recibe nada por entrada estandar, se guardan en la cola objetos Command pero cuya operacion es "done". Uno por cada worker.
### Command queue
La command queue encapsula una deque de la biblioteca estandar de c++, y un mutex, porque es un objeto al cual acceden tanto el hilo que lee como los workers. Se hace uso de los std::uniquelock para que las operaciones de la cola sean atómicas y no haya race conditions. Cuando un worker toma un comando de la cola, debe hacer un pop de la cola en la misma operacion atómica, porque si esas dos son atómicas por separado igualmente puede haber una race condition en el medio, y que dos workers agarren el mismo comando porque todavia no se hizo el pop. Para esto se implementa el método front_and_pop().
### Condition variable
Como se mencionó, el procesamiento de la entrada estandar y de los comandos se realiza concurrentemente. Los workers funcionan con el objeto activo Process_commands. Entonces los workers podrían intentar sacar un comando de la cola cuando esta está vacía, cosa que en este caso ni siquiera significa que ya no haya más operaciones que realizar, sino que puede ser que simplemente todavía no se haya pasado el comando a la cola. Para que los workers se queden esperando a que haya algo en la cola se usa un mutex con una condition variable. Cada vez que el hilo que lee agrega algo a la cola, le avisa a los workers, y uno de ellos deja de esperar y procesa el comando. Como el que la cola esté vacia no sirve como condición de corte para el procesamiento de los comandos, se hace uso de los comandos con operación "done" mencionado antes. Cuando un worker agarra uno de esos, hace return. El procesamiento de los comandos es un método del objeto SAC.
### SAC
