#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <atomic>
#include <deque>
#include <condition_variable>
#include <vector>
#include "file.h"
#include "command.h"
#include "split_apply_combine.h"
#include "thread.h"
#include "command_queue.h"
#include "process_input.h"
#include "process_commands.h"
#include "multithread_split_apply_combine.h"

#define SUCCESS 0
#define ERROR 1

Multithread_split_apply_combine::Multithread_split_apply_combine
                                (int workers_amount,
                                File& file,
                                int columns_per_row):
                                workers_amount(workers_amount),
                                no_more_input(false),
                                split_apply_combine(file, columns_per_row) {}

int Multithread_split_apply_combine::run() {
    Command_queue queue;
    std::vector<Thread*> workers;
    Process_input* input_thread = new Process_input(&queue,
                                            &this->no_more_input,
                                            this->workers_amount);
    int i;
    for (i = 0; i < this->workers_amount; i++) {
        workers.push_back(new Process_commands(this->split_apply_combine,
                                                &queue,
                                                &this->no_more_input));
    }
    input_thread->start();
    for (i = 0; i < this->workers_amount; i++) {
        workers[i]->start();
    }
    input_thread->join();
    delete(input_thread);
    for (i = 0; i < this->workers_amount; i++) {
        workers[i]->join();
        delete(workers[i]);
    }
    this->split_apply_combine.print_results();
    return SUCCESS;
}

