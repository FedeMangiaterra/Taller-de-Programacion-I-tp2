#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <mutex>

class File{
    FILE* fd;
    std::mutex m;
    public:
    File(const char* name, const char* flags);

    int read(void *ptr, size_t size, size_t nmemb);

    int seek(long int offset, int origin);

    int seek_and_read(long int offset, int origin, 
                    void *ptr, size_t size, size_t nmemb);

    int move_to_start();

    ~File();

    private:
    File(const File& other) = delete;
    File& operator=(const File &other) = delete;
};

#endif
