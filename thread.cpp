#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <utility>
#include <condition_variable>
#include "SAC.h"

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

Command_queue::Command_queue() : queue(new std::deque<Command>){}

void Command_queue::push(Command command){
    std::unique_lock<std::mutex> lock(this->m);
    this->queue->push_back(std::move(command));
}

Command Command_queue::front(){
    std::unique_lock<std::mutex> lock(this->m);
    return std::move(this->queue->front());
}

Command Command_queue::front_and_pop(){
    std::unique_lock<std::mutex> lock(this->m);
    Command command = std::move(this->queue->front());
    this->queue->pop_front();
    return command;
}

size_t Command_queue::size(){
    std::unique_lock<std::mutex> lock(this->m);
    return this->queue->size();
}

bool Command_queue::empty(){
    std::unique_lock<std::mutex> lock(this->m);
    return this->queue->empty();
}

Command_queue::~Command_queue(){
    delete this->queue;
}

Process_input::Process_input(Command_queue* queue,
                            bool* no_more_input,
                            std::condition_variable* c_v,
                            int workers):
                            queue(queue), no_more_input(no_more_input),
                            c_v(c_v), workers(workers){}

void Process_input::run(){
    char* line = NULL;
    size_t size = 40;
    int i;
    int command_number = 0;
    int characters_read = getline(&line, &size, stdin);
    while (characters_read > 1){ 
        Command command(line, command_number);
        int start_range = command.get_start_range();
        int end_range = command.get_end_range();
        int total_rows = command.get_total_rows();
        int partition_rows = command.get_partition_rows();
        int column = command.get_column();
        char* op = command.get_op();
        for (i = start_range; i < end_range; i += partition_rows){
            int new_end = i + partition_rows;
            if (i + partition_rows > end_range){
                new_end = end_range;
            }
            Command sub_command(i, new_end, total_rows, partition_rows,
                                column, command_number, op);
            this->queue->push(std::move(sub_command)); 
            this->c_v->notify_all();
        } 
        command_number++;
        free(line);
        line = NULL;
        characters_read = getline(&line, &size, stdin);
    }
    for (i = 0; i < workers; i++){
        char end_op[5] = "done";
        Command end_command(-1, 0, 0, 0,
                            0, 0, end_op);
        this->queue->push(std::move(end_command));
        this->c_v->notify_all(); 
    }
    *this->no_more_input = true;
    free(line);
}

Process_commands::Process_commands(SAC& split_apply_combine,
                                    Command_queue* queue,
                                    bool* no_more_input,
                                    bool* finished,
                                    std::condition_variable* c_v,
                                    std::mutex* m):
                    split_apply_combine(split_apply_combine), queue(queue), 
                    no_more_input(no_more_input), finished(finished),
                    c_v(c_v), m(m){}


void Process_commands::run(){
    std::unique_lock<std::mutex> lock(*this->m);
    while (!*this->finished){
        while (this->queue->empty()){
            this->c_v->wait(lock);
        }
        while (!this->queue->empty()){
            Command command = std::move(this->queue->front_and_pop());
            if (this->split_apply_combine.process_command(command) == DONE){
                return;
            }
        }
    }
}
