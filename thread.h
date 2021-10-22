#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iostream>
#include <thread>
#include <deque>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "command.h"
#include "split_apply_combine.h"

class Thread {
    private:
        std::thread thread;
 
    public:
        Thread();

        /*Lanza el thread. Llama al metodo run del
        objeto activo que funcione en el thread*/
        void start();

        /*Espera a que termine la ejecucion del thread.*/
        void join();

        virtual void run() = 0;
        virtual ~Thread() {}

        Thread(const Thread&) = delete;

        Thread& operator=(const Thread&) = delete;

        Thread(Thread&& other);

        Thread& operator=(Thread&& other);
};

#endif
