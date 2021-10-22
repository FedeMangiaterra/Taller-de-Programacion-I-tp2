#ifndef PROCESS_INPUT_H
#define PROCESS_INPUT_H

#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <utility>
#include <atomic>
#include "command_queue.h"

class Process_input : public Thread{
    private:
    Command_queue* queue;
    std::atomic<bool>* no_more_input;
    int workers;
    public:
    Process_input(Command_queue* queue,
                std::atomic<bool>* no_more_input,
                int workers);
    /*Recibe por entrada estandar los parametros para la creacion
    de un comando, lo crea y lo divide en varios comandos dependiendo
    de la cantidad de particiones necesarias para ejecutarlo. Cada uno de
    esos comandos menores comparte el numero de comando del mayor al
    que pertenecen. Luego se los agrega a una cola de comandos. Se
    detiene cuando recibe un solo byte, esto es, cuando el usuario
    apreta enter.*/
    virtual void run() override;
};

#endif
