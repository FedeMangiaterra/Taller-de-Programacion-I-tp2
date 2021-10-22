#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <utility>
#include <atomic>
#include <string>
#include "command_queue.h"
#include "process_input.h"

Process_input::Process_input(Command_queue* queue,
                            std::atomic<bool>* no_more_input,
                            int workers):
                            queue(queue), no_more_input(no_more_input),
                            workers(workers) {}

void Process_input::run() {
    char* line = NULL;
    size_t size = 40;
    int i;
    int command_number = 0;
    int characters_read = getline(&line, &size, stdin);
    while (characters_read > 1) { 
        Command command(line, command_number);
        int start_range = command.get_start_range();
        int end_range = command.get_end_range();
        int total_rows = command.get_total_rows();
        int partition_rows = command.get_partition_rows();
        int column = command.get_column();
        std::string op = command.get_op();
        for (i = start_range; i < end_range; i += partition_rows) {
            int new_end = i + partition_rows;
            if (i + partition_rows > end_range) {
                new_end = end_range;
            }
            Command sub_command(i, new_end, total_rows, partition_rows,
                                column, command_number, op);
            this->queue->push(sub_command); 
        } 
        command_number++;
        free(line);
        line = NULL;
        characters_read = getline(&line, &size, stdin);
    }
    for (i = 0; i < workers; i++) {
        Command end_command(-1, 0, 0, 0,
                            0, 0, "done");
        this->queue->push(end_command);
    }
    *this->no_more_input = true;
    free(line);
}
