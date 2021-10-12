#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "command.h"
#include "file.h"
#include "partition.h"

Partition::Partition(int rows, int columns, int starting_row,
            size_t size, u_int16_t *data) 
            : rows(rows),
            columns(columns),
            starting_row(starting_row),
            size(size),
            data((u_int16_t*)malloc(rows*columns*sizeof(u_int16_t))){
                size_t i;
                for (i = 0; i < size; i++){
                    uint16_t num = htons(data[i]);
                    this->data[i] = num;
                }
            }

u_int16_t Partition::get_max(int start, int finish,
                            int current_row, int column){
    uint16_t max = 0;
    size_t i;
    for (i = column;
        i < this->size && current_row < finish;
        i += this->columns){
        if (current_row >= start && this->data[i] > max){
            max = this->data[i];
        }
        current_row++;
    }
    return max;
}

u_int16_t Partition::get_min(int start, int finish,
                            int current_row, int column){
    u_int16_t min = 65535;
    size_t i;
    for (i = column;
        i < this->size && current_row < finish;
        i += this->columns){
        if (current_row >= start && this->data[i] < min){
            min = this->data[i];
        }
        current_row++;
    }
    return min;
}

u_int16_t Partition::sum(int start, int finish, int current_row, int column){
    u_int16_t sum = 0;
    size_t i;
    for (i = column;
        i < this->size && current_row < finish;
        i += this->columns){
        if (current_row >= start){
            sum += this->data[i];
        }
        current_row++;
    }
    return sum;
}
    
Partition::~Partition(){
    free(this->data);
}
