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
#include "process_commands.h"

#define DONE 4

Process_commands::Process_commands(Split_apply_combine& split_apply_combine,
                                    Command_queue* queue,
                                    std::atomic<bool>* no_more_input):
                    split_apply_combine(split_apply_combine), queue(queue), 
                    no_more_input(no_more_input), finished(false) {}

void Process_commands::run() {
    while (!this->finished) {
        Command command = this->queue->wait_front_pop();
        if (this->split_apply_combine.process_command(command) == DONE) {
            this->finished = true;
        }
    }
}
