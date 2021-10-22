#ifndef PROCESS_COMMANDS_H
#define PROCESS_COMMANDS_H

#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <utility>
#include <atomic>
#include <condition_variable>
#include "split_apply_combine.h"
#include "command_queue.h"

class Process_commands : public Thread{
    private:
    Split_apply_combine& split_apply_combine;
    Command_queue* queue;
    std::atomic<bool>* no_more_input;
    bool finished; 
    public:
    /*Toma un comando de la cola de comandos y lo ejecuta. Se detiene
    cuando se intenta procesar el comando que indica que ya no quedan
    operaciones por realizar.*/
    Process_commands(Split_apply_combine& split_apply_combine,
                    Command_queue* queue,
                    std::atomic<bool>* no_more_input);
    virtual void run() override;
};

#endif
