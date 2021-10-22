#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <mutex>

class File {
    FILE* fd;
    std::mutex m;
    
    public:
    File(const char* name, const char* flags);

    /*Lee una cantidad de veces determinada por nmemb, y el tamanio de
    cada lectura es de una cantidad de bytes indicada por size. Guarda los
    bytes leidos en la direccion de ptr*/
    int read(void *ptr, size_t size, size_t nmemb);

    /*Situa el puntero de lectura del archivo en una posicion indicada por
    el parametro offset. Desde donde se empieza a contar el offset es determinado
    por el origin*/
    int seek(long int offset, int origin);

    /*Hace tanto un read como un seek, pero de forma atomica. Toma los parametros
    que toman esas dos funciones.*/
    int seek_and_read(long int offset, int origin, 
                    void *ptr, size_t size, size_t nmemb);

    /*Situa el puntero de lectura del archivo en la primera posicion del mismo*/
    int move_to_start();

    ~File();

    private:
    File(const File& other) = delete;
    File& operator=(const File &other) = delete;
};

#endif
