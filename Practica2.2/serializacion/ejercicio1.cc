#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
        //
        _size = sizeof(int16_t) * 2;
        _size += sizeof(char) * 80;
        alloc_data(_size);
        char *  _pointer = _data;
        memcpy(_pointer, name, sizeof(char) * 80);
        _pointer += sizeof(char) * 80;
        memcpy(_pointer, &x, sizeof(int16_t));
        _pointer += sizeof(int16_t);
        memcpy(_pointer, &y, sizeof(int16_t));
    }

    int from_bin(char * data)
    {
        //
        char *  _pointer = data;
        memcpy(name, _pointer, sizeof(char) * 80);
         _pointer += sizeof(char) * 80;
        memcpy(&x, _pointer, sizeof(int16_t));
        _pointer += sizeof(int16_t);
        memcpy(&y, _pointer, sizeof(int16_t));
        return 0;
    }


public:
    char name[80];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 123, 987);

    // 1. Serializar el objeto one_w
    // 2. Escribir la serialización en un fichero
    // 3. Leer el fichero
    // 4. "Deserializar" en one_r
    // 5. Mostrar el contenido de one_r

    one_w.to_bin();

    int fd = open("./jugador.bin", O_CREAT | O_TRUNC | O_RDWR, 0666);

    write(fd, one_w.data(), one_w.size());

    close(fd);

    char buffer[256];
    fd = open("./jugador.bin", O_RDONLY);

    read(fd, buffer, 256);

    close(fd);

    one_r.from_bin(buffer);

    std::cout << one_r.name << " " << one_r.x << " " << one_r.y << "\n";

    return 0;
}

