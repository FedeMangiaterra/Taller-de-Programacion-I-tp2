#ifndef COMMAND_H
#define COMMAND_H

class Command{
    private:
    int start_range;
    int end_range;
    int total_rows;
    int partition_rows;
    int column;
    int command_number;
    char* op;

    public:
    Command(char* line, int command_number);

    Command(int start_range, int end_range,
                int total_rows, int partition_rows,
                int column, int command_number,
                char* op);

    Command(Command&& other);

    int get_start_range();

    int get_end_range();

    int get_total_rows();

    int get_partition_rows();

    int get_column();

    int get_command_number();

    char* get_op();

    ~Command();
};

#endif
