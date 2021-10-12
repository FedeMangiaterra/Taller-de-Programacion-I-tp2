#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"


Command::Command(char* line, int command_number):
    command_number(command_number){
    char* args[5];
    int i;
    char* arg = strtok(line, " ");
    for (i = 0; i < 5 && arg != NULL; i++){
        if (i != 4){
            args[i] = arg;
        }else{
            args[i] = strtok(arg, "\n");
        }
        arg = strtok(NULL, " ");
    }
    if (i != 5 && arg == NULL){
        printf("Not enough arguments \n");
        return;
    }
    this->start_range = atoi(args[0]);
    this->end_range = atoi(args[1]);
    this->total_rows = this->end_range - this->start_range;
    this->partition_rows = atoi(args[2]);
    this->column = atoi(args[3]);
    this->op = (char*)malloc(strlen(args[4])+1);
    memcpy(this->op, args[4], strlen(args[4])+1);
}

Command::Command(int start_range, int end_range,
                int total_rows, int partition_rows,
                int column, int command_number,
                char* op):
                start_range(start_range), end_range(end_range),
                total_rows(total_rows), partition_rows(partition_rows),
                column(column), command_number(command_number){
    this->op = (char*)malloc(strlen(op)+1);
    memcpy(this->op, op, strlen(op)+1);
}

Command::Command(Command&& other){
    this->start_range = other.start_range;
    this->end_range = other.end_range;
    this->total_rows = other.total_rows;
    this->partition_rows = other.partition_rows;
    this->column = other.column;
    this->command_number = other.command_number;
    this->op = other.op;
    other.op = nullptr;
}

int Command::get_start_range(){
    return this->start_range;
}

int Command::get_end_range(){
    return this->end_range;
}

int Command::get_total_rows(){
    return this->total_rows;
}

int Command::get_partition_rows(){
    return this->partition_rows;
}

int Command::get_column(){
    return this->column;
}

int Command::get_command_number(){
    return this->command_number;
}

char* Command::get_op(){
    return this->op;
}

Command::~Command(){
    if (this->op != NULL){
        free(this->op);
    }
}

