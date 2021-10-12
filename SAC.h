#ifndef SAC_H
#define SAC_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "file.h"
#include "result.h"


class SAC{ //(Split-Apply-Combine)
    private:
    File& file;
    int columns;
    std::mutex m;
    std::vector<Result*>* results;

    public:
    SAC(File& file, int columns);

    int get_max(int start, int finish, int rows, int column,
                     int command_number);

    int get_min(int start, int finish, int rows, int column,
                     int command_number);

    int sum(int start, int finish, int rows, int column,
                     int command_number);

    int mean(int start, int finish, int rows, int column,
            int total_rows, int command_number);

    int process_command(Command& command);

    void print_results();

    ~SAC();

    private:
    SAC(const SAC& other) = delete;
    SAC& operator=(const SAC &other) = delete;
};

#endif
