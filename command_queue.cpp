#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <utility>
#include <atomic>
#include <condition_variable>
#include "command.h"
#include "command_queue.h"

Command_queue::Command_queue() : queue(new std::deque<Command>) {}

void Command_queue::push(Command command) {
    std::unique_lock<std::mutex> lock(this->m);
    this->queue->push_back(command);
    this->queue_condition_variable.notify_all();
}

Command Command_queue::front() {
    std::unique_lock<std::mutex> lock(this->m);
    return this->queue->front();
}

Command Command_queue::front_and_pop() {
    std::unique_lock<std::mutex> lock(this->m);
    Command command = this->queue->front();
    this->queue->pop_front();
    return command;
}

Command Command_queue::wait_front_pop() {
    std::unique_lock<std::mutex> lock(this->condition_variable_m);
    while (this->empty()) {
        this->queue_condition_variable.wait(lock);
    }
    return this->front_and_pop();
}

size_t Command_queue::size() {
    std::unique_lock<std::mutex> lock(this->m);
    return this->queue->size();
}

bool Command_queue::empty() {
    std::unique_lock<std::mutex> lock(this->m);
    return this->queue->empty();
}

Command_queue::~Command_queue() {
    delete this->queue;
}
