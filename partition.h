#ifndef PARTITION_H
#define PARTITION_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>

class Partition {
    private:
    int rows;
    int columns;
    int starting_row;
    size_t size;
    std::vector<u_int16_t> data;

    public:
    Partition(int rows, int columns, int starting_row,
             size_t size, std::vector<u_int16_t> data);

    Partition(File& file, Command& command, int columns, int starting_row);

    /*Devuelve el mayor valor de la particion teniendo en cuenta sobre
    que columna, desde que fila y hasta que fila operar. Esto se determina
    con los parametros column, start y finish respectivamente*/
    u_int16_t get_max(int start, int finish, int current_row, int column);

    /*Devuelve el menor valor de la particion teniendo en cuenta sobre
    que columna, desde que fila y hasta que fila operar. Esto se determina
    con los parametros column, start y finish respectivamente*/
    u_int16_t get_min(int start, int finish, int current_row, int column);

    /*Devuelve la suma de los valores de la particion teniendo en cuenta sobre
    que columna, desde que fila y hasta que fila operar. Esto se determina
    con los parametros column, start y finish respectivamente*/
    u_int16_t sum(int start, int finish, int current_row, int column);
    
    ~Partition();
};

#endif
