#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <deque>
#include <condition_variable>
#include "file.h"
#include "command.h"
#include "SAC.h"
#include "thread.h"

#define SUCCESS 0
#define ERROR 1

// Toda esta lógica del main tiene que ser repartida en las clases de tu diseno, sino es lógica global.
int main(int argc, char* argv[]) {
    if (argc < 4) return ERROR;
    File f(argv[1], "rb");

    // Estos bools son compartido por todos los threads y no están protegidos (std::atomic)
    bool no_more_input = false;
    bool finished = false;

    // Si querés usar atoi, usá el de std, sino usá std::stringstream
    int workers_amount = atoi(argv[3]);

    Command_queue queue;

    // Esta cond var debería ser parte del objeto monitor que protege a la queue
    std::condition_variable queue_condition_variable;
    SAC split_apply_combine(f, atoi(argv[2]));
    std::vector<Thread*> threads;
    threads.push_back(new Process_input(&queue,
                                        &no_more_input,
                                        &queue_condition_variable,
                                        workers_amount));

    // Al igual que la cond var de arriba, este mutex tiene que ser parte de un monitor en vez de estar suelto en el main.
    std::mutex m;
    int i;
    for (i = 0; i < workers_amount; i++){
        threads.push_back(new Process_commands(split_apply_combine, &queue,
                                        &no_more_input, &finished,
                                        &queue_condition_variable,
                                        &m));
    }

    // Estos <= en vez de < son hacks porque tu arreglo de hilos tiene un elemento más que workers_amount. No hagas eso,
    // porque el que lo lee tiene que estar atento para darse cuenta de por qué lo hiciste y le va a resultar raro, este 
    // tipo de prácticas es muy propenso a regresiones.
    for (i = 0; i <= workers_amount; i++) {
        threads[i]->start();
    }
    for (i = 0; i <= workers_amount; i++) {
        threads[i]->join();
        delete(threads[i]);
    }

    split_apply_combine.print_results();
    return SUCCESS;
}
