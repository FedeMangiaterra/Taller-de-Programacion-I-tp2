#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <mutex>

// Documentar la API de la clase.
// Además, para qué esta abstracción en vez de usar std::fstream?

class File{
    // Usar las abstracciones de C++ en vez de las de C
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
