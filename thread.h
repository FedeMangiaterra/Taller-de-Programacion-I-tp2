#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iostream>
#include <thread>
#include <deque>
#include <mutex>
#include <condition_variable>
#include "command.h"
#include "SAC.h"

class Thread {
    private:
        std::thread thread;
 
    public:
        Thread();

        void start();

        void join();

        virtual void run() = 0;
        virtual ~Thread() {}

        Thread(const Thread&) = delete;

        Thread& operator=(const Thread&) = delete;

        Thread(Thread&& other);

        Thread& operator=(Thread&& other);
};


// Mover estas clases a sus propios archivos.
class Command_queue{
    private:
    std::deque<Command>* queue;
    std::mutex m;

    public:
    Command_queue();

    void push(Command command);

    Command front();

    Command front_and_pop();

    size_t size();

    bool empty();

    ~Command_queue();

    private:
    Command_queue(const Command_queue& other) = delete;
    Command_queue& operator=(const Command_queue &other) = delete;
}; 

// Fijate que en el informe tenés las flechas de las herencias apuntando para el otro lado.
class Process_input : public Thread {
    private:
    Command_queue* queue;
    bool* no_more_input;
    std::condition_variable* c_v;
    int workers;
    public:
    Process_input(Command_queue* queue,
                bool* no_more_input,
                std::condition_variable* c_v,
                int workers);
    virtual void run() override;
};

class Process_commands : public Thread {
    private:
    SAC& split_apply_combine;
    Command_queue* queue;
    bool* no_more_input;
    bool* finished; 
    std::condition_variable* c_v;
    std::mutex* m;
    public:
    Process_commands(SAC& split_apply_combine, Command_queue* queue,
                    bool* no_more_input, bool* finished, 
                    std::condition_variable* c_v, std::mutex* m);
    virtual void run() override;
};

#endif
