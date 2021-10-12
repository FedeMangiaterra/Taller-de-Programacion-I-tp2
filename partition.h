#ifndef PARTITION_H
#define PARTITION_H

#include <stdio.h>
#include <stdlib.h>

class Partition{
    int rows;
    int columns;
    int starting_row;
    size_t size;
    u_int16_t *data;
    public:
    Partition(int rows, int columns, int starting_row,
             size_t size, u_int16_t *data);

    Partition(File& file, Command& command, int columns, int starting_row);

    u_int16_t get_max(int start, int finish, int current_row, int column);

    u_int16_t get_min(int start, int finish, int current_row, int column);

    u_int16_t sum(int start, int finish, int current_row, int column);
    
    ~Partition();
};

#endif
