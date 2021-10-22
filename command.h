#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>

class Command {
    private:
    int start_range;
    int end_range;
    int total_rows;
    int partition_rows;
    int column;
    int command_number;
    std::string op;

    public:
    Command(char* line, int command_number);

    Command(int start_range, int end_range,
                int total_rows, int partition_rows,
                int column, int command_number,
                const std::string& op);

    /*Develve el valor que indica desde que fila se debe
    ejecutar el comando*/
    int get_start_range();

    /*Develve el valor que indica hasta que fila se debe
    ejecutar el comando*/
    int get_end_range();

    /*Develve el valor que indica sobre cuantas filas se
    debe ejecutar el comando*/
    int get_total_rows();

    /*Develve el valor que indica cuantas filas debe tener
    cada particion sobre la cual se ejectuta el comando*/
    int get_partition_rows();

    /*Develve el valor que indica sobre que columna se
    debe realizar la operacion*/
    int get_column();

    /*Develve el valor que indica que posicion tiene el
    comando en relacion al resto de comandos a ejecutar*/
    int get_command_number();

    /*Develve el string que indica la operacion que se
    debe ejecutar*/
    std::string get_op();

    ~Command();
};

#endif
