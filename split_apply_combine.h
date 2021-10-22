#ifndef SPLIT_APPLY_COMBINE_H
#define SPLIT_APPLY_COMBINE_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include "file.h"
#include "result.h"


class Split_apply_combine { 
    private:
    File& file;
    int columns;
    std::mutex m;
    std::vector<Result*> results;

    public:
    Split_apply_combine(File& file, int columns);

    /*Crea una particion del archivo segun los parametros de start,
    finish y rows. Luego busca el maximo de esa particion y actualiza
    el resultado del comando correspondiente si es necesario.*/
    int get_max(int start, int finish, int rows, int column,
                int command_number);

    /*Crea una particion del archivo segun los parametros de start,
    finish y rows. Luego busca el minimo de esa particion y actualiza
    el resultado del comando correspondiente si es necesario.*/
    int get_min(int start, int finish, int rows, int column,
                int command_number);

    /*Crea una particion del archivo segun los parametros de start,
    finish y rows. Luego calcula la suma de la columna indicada de
    esa particion y actualiza el resultado del comando correspondiente.*/
    int sum(int start, int finish, int rows, int column,
            int command_number);

    /*Hace lo mismo que la funcion sum, pero sobre un resultado del
    tipo Result_mean*/
    int mean(int start, int finish, int rows, int column,
            int total_rows, int command_number);

    /*Recibe un comando y llama al metodo que corresponde.
    Devuelve 4 si se recibe el comando que indica que no
    hay mas operaciones por realizar, y devuelve 1 en caso
    de error.*/
    int process_command(Command& command);

    /*Imprime todos los resultados de los comandos ejecutados,
    en el orden que fueron pasados por entrada estandar.*/
    void print_results();

    ~Split_apply_combine();

    private:
    Split_apply_combine(const Split_apply_combine& other) = delete;
    Split_apply_combine& operator=(const Split_apply_combine &other) = delete;

    void create_result_if_absent(int command_number,
                                const std::string& op,
                                int total_rows);
};

#endif
