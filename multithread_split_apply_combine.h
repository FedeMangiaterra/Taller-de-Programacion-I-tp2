#ifndef MULTITHREAD_SPLIT_APPLY_COMBINE_H
#define MULTITHREAD_SPLIT_APPLY_COMBINE_H

#include <stdio.h>
#include <stdlib.h>
#include <atomic>
#include "file.h"
#include "command.h"
#include "split_apply_combine.h"

class Multithread_split_apply_combine {
    private:
    int workers_amount;
    std::atomic<bool> no_more_input;
    Split_apply_combine split_apply_combine;

    public:
    Multithread_split_apply_combine(int workers_amount,
                                File& file,
                                int columns_per_row);

    /*Recibe por entrada estandar comandos a realizar sobre el archivo file,
    los cuales son ejecutados concurrentemente por una cantidad de threads 
    determinada por workers amount. Cuando el usuario presiona enter, se 
    terminan de procesar los comandos restantes si los hay, y luego se
    imprimen todos los resultados en el orden en el que fueron */                            
    int run();

    private:
    Multithread_split_apply_combine
    (const Multithread_split_apply_combine& other) = delete;

    Multithread_split_apply_combine& operator=
    (const Multithread_split_apply_combine &other) = delete;
};

#endif
