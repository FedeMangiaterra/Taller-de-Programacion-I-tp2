#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include "command.h"

Command::Command(char* line, int command_number):
    command_number(command_number) {
    std::vector<std::string> args; 
    int i;
    std::string arg = strtok(line, " ");
    for (i = 0; i < 5 && !arg.empty(); i++) {
        if (i != 4) {
            args.push_back(arg);
            arg = strtok(NULL, " ");
        } else {
            arg.erase(arg.length() - 1); //Saco el \n
            args.push_back(arg);
        }
    }
    if (i != 5) { 
        std::cout << "Not enough arguments \n";
        return;
    }
    this->start_range = std::stoi(args[0]);
    this->end_range = std::stoi(args[1]);
    this->total_rows = this->end_range - this->start_range;
    this->partition_rows = std::stoi(args[2]);
    this->column = std::stoi(args[3]);
    this->op = args[4];
}

Command::Command(int start_range, int end_range,
                int total_rows, int partition_rows,
                int column, int command_number,
                const std::string& op):
                start_range(start_range), end_range(end_range),
                total_rows(total_rows), partition_rows(partition_rows),
                column(column), command_number(command_number), op(op) {}

int Command::get_start_range() {
    return this->start_range;
}

int Command::get_end_range() {
    return this->end_range;
}

int Command::get_total_rows() {
    return this->total_rows;
}

int Command::get_partition_rows() {
    return this->partition_rows;
}

int Command::get_column() {
    return this->column;
}

int Command::get_command_number() {
    return this->command_number;
}

std::string Command::get_op() {
    return this->op;
}

Command::~Command() {}

