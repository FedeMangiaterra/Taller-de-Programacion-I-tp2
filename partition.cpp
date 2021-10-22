#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <vector>
#include "command.h"
#include "file.h"
#include "partition.h"

Partition::Partition(int rows, int columns, int starting_row,
            size_t size, std::vector<u_int16_t> data) 
            : rows(rows),
            columns(columns),
            starting_row(starting_row),
            size(size),
            data() {
                size_t i;
                for (i = 0; i < size; i++) {
                    uint16_t num = htons(data[i]);
                    this->data.push_back(num);
                }
            }

u_int16_t Partition::get_max(int start, int finish,
                            int current_row, int column) {
    uint16_t max = 0;
    size_t i;
    for (i = column;
        i < this->size && current_row < finish;
        i += this->columns, current_row++) {
        if (current_row >= start && this->data[i] > max) {
            max = this->data[i];
        }
    }
    return max;
}

u_int16_t Partition::get_min(int start, int finish,
                            int current_row, int column) {
    u_int16_t min = 65535;
    size_t i;
    for (i = column;
        i < this->size && current_row < finish;
        i += this->columns, current_row++) {
        if (current_row >= start && this->data[i] < min) {
            min = this->data[i];
        }
    }
    return min;
}

u_int16_t Partition::sum(int start, int finish, int current_row, int column) {
    u_int16_t sum = 0;
    size_t i;
    for (i = column;
        i < this->size && current_row < finish;
        i += this->columns, current_row++) {
        if (current_row >= start) {
            sum += this->data[i];
        }
    }
    return sum;
}
    
Partition::~Partition() {}
