#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include "file.h"
#include "command.h"
#include "partition.h"
#include "result.h"
#include "split_apply_combine.h"

#define ERROR 1
#define COMPLETED 2
#define UNCOMPLETED 3
#define DONE 4

Split_apply_combine::Split_apply_combine(File& file, int columns) :
file(file),
columns(columns),
results(){}

void Split_apply_combine::create_result_if_absent(int command_number,
                                                const std::string& op,
                                                int total_rows) {
    std::unique_lock<std::mutex> lock(this->m);
    if (this->results.size() <= (size_t)command_number) {
        if (op == "max") {
            this->results.push_back(new Result_max());
        } else if (op == "min") {
            this->results.push_back(new Result_min());
        } else if (op == "sum") {
            this->results.push_back(new Result_sum());
        } else if (op == "mean") {
            this->results.push_back(new Result_mean(total_rows));
        }
    }
}

int Split_apply_combine::get_max(int start, int finish, int rows, int column,
                     int command_number) {
    this->create_result_if_absent(command_number, "max", 0);
    int size = rows*this->columns;
    int starting_position = start*this->columns*sizeof(u_int16_t);
    std::vector<u_int16_t> nums(size); 
    size = this->file.seek_and_read(starting_position, SEEK_SET,
                            &nums[0], sizeof(u_int16_t), size);
    Partition partition(rows, this->columns,
                            start, size, nums);
    uint16_t partition_max = partition.get_max(start, finish,
                                                start, column);
    this->results.at(command_number)->update_value(partition_max); 
    if (size < rows*this->columns) {
        this->file.move_to_start();
        return COMPLETED;
    }
    return UNCOMPLETED;
}


int Split_apply_combine::get_min(int start, int finish, int rows, int column,
                     int command_number) {
    this->create_result_if_absent(command_number, "min", 0);
    int size = rows*this->columns;
    int starting_position = start*this->columns*sizeof(u_int16_t);
    std::vector<u_int16_t> nums(size);
    size = this->file.seek_and_read(starting_position, SEEK_SET,
                            &nums[0], sizeof(u_int16_t), size);
    Partition partition(rows, this->columns,
                            start, size, nums);
    uint16_t partition_min = partition.get_min(start, finish,
                                                start, column);
    this->results.at(command_number)->update_value(partition_min); 
    if (size < rows*this->columns) {
        this->file.move_to_start();
        return COMPLETED;
    }
    return UNCOMPLETED;
}

int Split_apply_combine::sum(int start, int finish, int rows, int column,
                     int command_number) {
    this->create_result_if_absent(command_number, "sum", 0);
    int size = rows*this->columns;
    int starting_position = start*this->columns*sizeof(u_int16_t);
    std::vector<u_int16_t> nums(size);
    size = this->file.seek_and_read(starting_position, SEEK_SET,
                            &nums[0], sizeof(u_int16_t), size);
    Partition partition(rows, this->columns,
                            start, size, nums);
    uint16_t partition_sum = partition.sum(start, finish,
                                            start, column);
    this->results.at(command_number)->update_value(partition_sum); 
    if (size < rows*this->columns) {
        this->file.move_to_start();
        return COMPLETED;
    }
    return UNCOMPLETED;
}

int Split_apply_combine::mean(int start, int finish, int rows, int column,
                int total_rows, int command_number) {
    this->create_result_if_absent(command_number, "mean", total_rows);
    return this->sum(start, finish, rows, column,
                             command_number);
}

int Split_apply_combine::process_command(Command& command) {
    int start = command.get_start_range();
    int finish = command.get_end_range();
    int partition_rows = command.get_partition_rows();
    int column = command.get_column();
    int command_number = command.get_command_number();
    std::string op = command.get_op();
    if (op == "sum") {
        return this->sum(start, finish,
                                partition_rows, column,
                                command_number);
    } else if (op == "max") {
        return this->get_max(start, finish,
                                    partition_rows, column,
                                    command_number);
    } else if (op == "min") {
        return this->get_min(start, finish,
                                    partition_rows, column,
                                    command_number);
    } else if (op == "mean") {
        return this->mean(start, finish,
                    partition_rows, column,
                    command.get_total_rows(),
                    command_number);
    } else if (op == "done") {
        return DONE;
    } else {
        std::cout << "Invalid operation \n";
        return ERROR;
    }
    return UNCOMPLETED;
}

void Split_apply_combine::print_results() {
    size_t i;
    for (i = 0; i < this->results.size(); i++) {
        this->results.at(i)->print();
    }
}

Split_apply_combine::~Split_apply_combine() {
    size_t i;
    for (i = 0; i < this->results.size(); i++) {
        delete this->results.at(i);
    }
}
