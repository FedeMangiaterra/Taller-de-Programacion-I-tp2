#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <utility>
#include <atomic>
#include <condition_variable>
#include <deque>
#include "command.h"

class Command_queue{
    private:
    std::deque<Command>* queue;
    std::mutex m;
    std::condition_variable queue_condition_variable;
    std::mutex condition_variable_m;

    public:
    Command_queue();

    /*Agrega el comando pasado por parametro al final
    de la cola. Esta operacion es atomica*/
    void push(Command command);

    /*Devuelve el comando que esta al principio
    de la cola. Esta operacion es atomica*/
    Command front();

    /*Devuelve el comando que esta al principio
    de la cola y ademas lo saca de la misma, todo
    de forma atomica*/
    Command front_and_pop();

    /*Si no hay nada en la cola, espera a que haya
    algo. Luego devuelve el comando que esta al principio
    de la cola y ademas lo saca de la misma de forma atomica*/
    Command wait_front_pop();

    /*Devuelve la cantidad de comandos presentes
    en la cola, de forma atomica.*/
    size_t size();

    /*Devuelve un booleano que indica si la cola esta
    vacia o no.*/
    bool empty();

    ~Command_queue();

    private:
    Command_queue(const Command_queue& other) = delete;
    Command_queue& operator=(const Command_queue &other) = delete;
};
#endif
