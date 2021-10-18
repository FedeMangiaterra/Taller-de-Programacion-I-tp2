#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "file.h"
#include "command.h"
#include "partition.h"
#include "result.h"
#include "SAC.h"

#define ERROR 1
#define COMPLETED 2
#define UNCOMPLETED 3
#define DONE 4

SAC::SAC(File& file, int columns) :
file(file),
columns(columns),
results(new std::vector<Result*>){}

int SAC::get_max(int start, int finish, int rows, int column,
                     int command_number){
    this->m.lock();
    if (this->results->size() <= (size_t)command_number){
        this->results->resize(command_number + 1);
        this->results->at(command_number) = new Result_max();
    }
    this->m.unlock();
    int size = rows*this->columns;
    int starting_position = start*this->columns*sizeof(u_int16_t);

    // Usar std::vector en vez de un malloc directo (y cuando se quiera usar memoria dinámica, usar new en vez de malloc)
    u_int16_t* nums = (u_int16_t*) malloc(sizeof(u_int16_t)*size);
    size = this->file.seek_and_read(starting_position, SEEK_SET,
                                    nums, sizeof(u_int16_t), size);
    Partition partition(rows, this->columns, start, size, nums);
    uint16_t partition_max = partition.get_max(start, finish, start, column);
    this->results->at(command_number)->update_value(partition_max); 

    free(nums);
    if (size < rows*this->columns){
        this->file.move_to_start();
        return COMPLETED;
    }
    return UNCOMPLETED;
}


int SAC::get_min(int start, int finish, int rows, int column,
                     int command_number){
    this->m.lock();
    if (this->results->size() <= (size_t)command_number){
        this->results->resize(command_number + 1);
        this->results->at(command_number) = new Result_min();
    }
    this->m.unlock();
    int size = rows*this->columns;
    int starting_position = start*this->columns*sizeof(u_int16_t);
    u_int16_t* nums = (u_int16_t*)malloc(sizeof(u_int16_t)*size);
    size = this->file.seek_and_read(starting_position, SEEK_SET,
                            nums, sizeof(u_int16_t), size);
    Partition partition(rows, this->columns,
                            start, size, nums);
    uint16_t partition_min = partition.get_min(start, finish,
                                                start, column);
    this->results->at(command_number)->update_value(partition_min); 

    free(nums);
    if (size < rows*this->columns){
        this->file.move_to_start();
        return COMPLETED;
    }
    return UNCOMPLETED;
}

int SAC::sum(int start, int finish, int rows, int column,
                     int command_number){
    // Usar lock_guard en vez de lock+unlock
    this->m.lock();
    if (this->results->size() <= (size_t)command_number){
        this->results->resize(command_number + 1);
        this->results->at(command_number) = new Result_sum();
    }
    this->m.unlock();
    int size = rows*this->columns;
    int starting_position = start*this->columns*sizeof(u_int16_t);
    u_int16_t* nums = (u_int16_t*)malloc(sizeof(u_int16_t)*size);
    size = this->file.seek_and_read(starting_position, SEEK_SET,
                            nums, sizeof(u_int16_t), size);
    Partition partition(rows, this->columns,
                            start, size, nums);
    uint16_t partition_sum = partition.sum(start, finish,
                                            start, column);
    this->results->at(command_number)->update_value(partition_sum); 

    free(nums);
    if (size < rows*this->columns){
        this->file.move_to_start();
        return COMPLETED;
    }
    return UNCOMPLETED;
}

int SAC::mean(int start, int finish, int rows, int column,
                int total_rows, int command_number){
    this->m.lock();
    if (this->results->size() <= (size_t)command_number) {
        // push_back en vez de resize+at
        this->results->resize(command_number + 1);
        this->results->at(command_number) = new Result_mean(total_rows);
    }
    this->m.unlock();
    return this->sum(start, finish, rows, column,
                             command_number);
}

int SAC::process_command(Command& command){
    int start = command.get_start_range();
    int finish = command.get_end_range();
    int partition_rows = command.get_partition_rows();
    int column = command.get_column();
    int command_number = command.get_command_number();
    char* op = command.get_op();
    if (strcmp(op, "sum") == 0){
        return this->sum(start, finish,
                                partition_rows, column,
                                command_number);
    }else if (strcmp(op, "max") == 0){
        return this->get_max(start, finish,
                                    partition_rows, column,
                                    command_number);
    }else if (strcmp(op, "min") == 0){
        return this->get_min(start, finish,
                                    partition_rows, column,
                                    command_number);
    }else if (strcmp(op, "mean") == 0){
        return this->mean(start, finish,
                    partition_rows, column,
                    command.get_total_rows(),
                    command_number);
    }else if (strcmp(op, "done") == 0){
        return DONE;
    }else{
        printf("Invalid operation \n");
        return ERROR;
    }
    return UNCOMPLETED;
}

void SAC::print_results(){
    size_t i;
    for (i = 0; i < this->results->size(); i++){
        this->results->at(i)->print();
    }
}

SAC::~SAC(){
    size_t i;
    for (i = 0; i < this->results->size(); i++){
        delete this->results->at(i);
    }
    delete (this->results);
}
