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

int main(int argc, char* argv[]){
    if (argc < 4) return ERROR;
    File f(argv[1], "rb");
    bool no_more_input = false;
    bool finished = false;
    int workers_amount = atoi(argv[3]);
    Command_queue queue;
    std::condition_variable queue_condition_variable;
    SAC split_apply_combine(f, atoi(argv[2]));
    std::vector<Thread*> threads;
    threads.push_back(new Process_input(&queue,
                                        &no_more_input,
                                        &queue_condition_variable,
                                        workers_amount));
    std::mutex m;
    int i;
    for (i = 0; i < workers_amount; i++){
        threads.push_back(new Process_commands(split_apply_combine, &queue,
                                        &no_more_input, &finished,
                                        &queue_condition_variable,
                                        &m));
    }
    for (i = 0; i <= workers_amount; i++){
        threads[i]->start();
    }
    for (i = 0; i <= workers_amount; i++){
        threads[i]->join();
        delete(threads[i]);
    }

    split_apply_combine.print_results();
    return SUCCESS;
}
