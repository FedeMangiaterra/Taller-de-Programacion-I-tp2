#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <utility>

#define DONE 4

Thread::Thread() {}

void Thread::start() {
    thread = std::thread(&Thread::run, this);
}

void Thread::join() {
    thread.join();
}

Thread::Thread(Thread&& other) {
    this->thread = std::move(other.thread);
}

Thread& Thread::operator=(Thread&& other) {
    this->thread = std::move(other.thread);
    return *this;
}
