#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include "file.h"

File::File(const char* name, const char* flags) {
    this->fd = fopen(name, flags);
    if (!this->fd) {
        std::cout << "Couldn't open file \n";
    }
}

int File::read(void *ptr, size_t size, size_t nmemb) {
    return fread(ptr, size, nmemb, this->fd);
}

int File::seek(long int offset, int origin){
    return fseek(this->fd, offset, origin);
}

int File::seek_and_read(long int offset, int origin, 
                        void *ptr, size_t size, size_t nmemb) {
    std::unique_lock<std::mutex> lock(this->m);
    this->seek(offset, origin);
    return this->read(ptr, size, nmemb);
}

int File::move_to_start() {
    return fseek(this->fd, 0, SEEK_SET);
}

File::~File() {
    if (this->fd) {
        fclose(this->fd);
    }
}


