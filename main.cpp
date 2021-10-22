#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "multithread_split_apply_combine.h"

#define SUCCESS 0
#define ERROR 1

int main(int argc, char* argv[]) {
    if (argc < 4) return ERROR;
    File f(argv[1], "rb");
    int workers_amount = std::atoi(argv[3]);
    Multithread_split_apply_combine mt_split_apply_combine(workers_amount, f,
                                                        std::atoi(argv[2]));
    return mt_split_apply_combine.run();
}
