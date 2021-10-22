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
* Command_queue
* Result
* Thread
* Process_input
* Process_commands
* Partition
* Split_apply_combine
* Multithread_split_apply_combine

## Resolución del trabajo
### Thread
El programa cuenta con N hilos que funcionan como workers, y un hilo más que es el encargado de leer por entrada estandar los comandos que se deben ejecutar. En el archivo thread.c se define la clase Thread, para poder hacer otras clases que hereden de ella, y que funcionen como objetos activos, funciones que se ejecutan sobre su propio hilo mediante el metodo run(). <br>
Los hilos funcionan de manera concurrente usando condition variables y una cola, siguiendo el modelo de productor y consumidor/es. Para llevar a cabo esto se implementa los objetos de Command y Command_queue.
### Command
Por entrada estandar, se indica la fila inicial y final, la columna sobre la cual se opera, la cantidad de filas por partición y la operación a realizar. La clase Command guarda todo eso y también el total de filas sobre las que se opera y el numero de comando (el orden en el cual cada comando se pasó por entrada estandar). Se usa un constructor de command que recibe toda la linea de entrada estandar y el numero de comando, y completa sus atributos. Pero lo que se manda por entrada estandar es la operación a realizar sobre todo el archivo. Para trabajar con las particiones, se "parte" tambien el comando(mediante el objeto activo Process_input, que hereda de Thread), dividiendolo en varios sub-comandos que cumplen que la fila final menos la inicial es menor o igual a la cantidad de filas por partición. Por ejemplo, una suma de la segunda columna entre la fila 1 y la 8, se divide en una suma asi pero entre las filas 1 y 3, 3 y 5, 5 y 7, y 8. <br>
Todos estos mini comandos se guardan en la Command_queue. Cuando ya no se recibe nada por entrada estandar, se guardan en la cola objetos Command pero cuya operacion es "done". Uno por cada worker.
### Command queue
La command queue encapsula una deque de la biblioteca estandar de c++, una condition variable y dos mutex: uno para controlar los accesos a la deque, porque es un objeto al cual acceden tanto el hilo que lee como los workers, y otro para la condition variable. Se hace uso de los std::uniquelock para que las operaciones de la cola sean atómicas y no haya race conditions. Cuando un worker toma un comando de la cola, debe hacer un pop de la cola en la misma operacion atómica, porque si esas dos son atómicas por separado igualmente puede haber una race condition en el medio, y que dos workers agarren el mismo comando porque todavia no se hizo el pop. Para esto se implementa el método front_and_pop().
### Condition variable
Como se mencionó, el procesamiento de la entrada estandar y de los comandos se realiza concurrentemente. Los workers funcionan con el objeto activo Process_commands. Entonces los workers podrían intentar sacar un comando de la cola cuando esta está vacía, cosa que en este caso ni siquiera significa que ya no haya más operaciones que realizar, sino que puede ser que simplemente todavía no se haya pasado el comando a la cola. Para que los workers se queden esperando a que haya algo en la cola se usa uno de los mutex de la cola y su condition variable. Cada vez que el hilo que lee agrega algo a la cola, le avisa a los workers con un notify_all presente en el metodo de push de la cola, y uno de ellos deja de esperar y procesa el comando. Como el que la cola esté vacia no sirve como condición de corte para el procesamiento de los comandos, se hace uso de los comandos con operación "done" mencionado antes. Cuando un worker agarra uno de esos, hace return. El procesamiento de los comandos es un método del objeto Split_apply_combine.
### Split_apply_combine
El objeto Split_apply_combine guarda el archivo a procesar, la cantidad de columnas por fila, un mutex, y un vector con punteros a resultado.<br>
El procesamiento del comando empieza reconociando de que operacion se trata, comparando el atributo op con max, min, sum, mean y done. Dependiendo de de cuál se trate, se llama a la función correspondiente. <br>
En las funciones de max, min, sum y mean el procedimiento es casi igual: Lo primero es comprobar si en el vector de resultados ya hay un resultado correspondiente al número de comando actual. En caso contrario, se crea un objeto Result del tipo correspondiente. Como el vector de resultados es compartido entre todos los threads, toda la operación mencionada se hace atómica con el uso del mutex de Split_apply_combine. <br>
Después de esto lo que se hace es armar la partición. El objeto Partition guarda un arreglo de números, la cantidad de filas y de columnas, la fila actual y el tamaño del arreglo. Para leer el archivo se lo encapsula en el objeto File, que tiene un mutex, y con el metodo seek_and_read() se puede hacer una lectura desde la posicion deseada de manera atómica. La posición desde la cual leer se calcula sabiendo que en el archivo hay enteros de dos bits sin singo y sin padding. La primera posicion de una fila x se calcula como (cantidad de columnas por fila) * (numero de fila x) * 2 bits. <br>
Una vez que se lee se usa htons() para obtener el valor correcto de los numeros, que habían quedado con endianness invertida despues de la lectura. Luego se guardan en la partición. <br>
Se obtiene el maximo/minimo/suma de la partición y se llama al método update_result del resultado correspondiente, al cual se accede con el vector de punteros a Result.
### Result
Result es una clase abstracta con los metodos virtuales puros de print_result y update_result, los cuales se definen en las clases que heredan de Result: Result_max, Result_min, Result_mean y Result_sum. esto es así para poder usar polimorfismo entre estas clases, porque cada una hace algo distinto en sus métodos con igual nombre. Pero en cada caso lo que se hace es recibir un valor, y actualizar el resultado según corresponda. Si es el resultado de una suma o promedio, siempre se suma el numero recibido. Por otro lado, si es una operación de min/max, primero se chequea si es menor o mayor que el resultado actual. Estas operaciones son atómicas porque cada Result tiene su propio método y se hace uso de unique_locks.<br> <br>
![](http://www.plantuml.com/plantuml/png/SoWkIImgAStDuKfCAYufIamkKGXABIxDAU7YWj0ADZMwkb2n-EBIN6p2kQc9ULZ54okm2cVcmKstKYWeoimhqT1429GMf2IMfCQN9UQKfWB57LBpKe3k0m00)<br>
Cuando todos los threads sacaron su Command con operación done, se llama al método print_results() de Split_apply_combine, el cual hace uso nuevamente del polimorfismo entre las clases que heredan de Result, y se imprimen de forma ordenada todos los resultados pedidos.<br>
Toda la logica de lanzar los diferentes hilos, y también la creación del objeto Split_apply_combine, se encapsulan en la función run del objeto Multithread_split_apply_combine, que en su creacion recibe la cantidad de columnas que tendrá cada fila, el archivo a procesar y la cantidad de workers. <br>
Un diagrama de las relaciones entre todas las clases del programa seria algo así: <br> <br>
![](http://www.plantuml.com/plantuml/png/ZP5HJWCX48RVvnHSe1VO-D2aYKzgZ3q0cRAJ50C3mc2YyV2kDBIXejktNVZ_Z_d_sMR19CKx28TNCg9UA1Sdq7xlhQFslO_EYiOOtOSsmHyidw-U5uWL6xWTF2uGAyT4EEiyy3NTBdYFFEktGkL2Iy5GpjfoB79zQ4wE371-LDrzRJQZyTIqIuHmohIc6jnKI-r0JNjIU4W4S3r85VL1gleTZVs7-WipFJZq21sebzO-VdJ4VQXB4zDYZuu40Q2tLU7AMXOsyYSgObLO7QqBVwIzdVDKCNdSmpggBMVGoct6BV5SlFi6)

