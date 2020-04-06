//
// Created by Randy Mei on 2020/4/3.
//

#ifndef CACHE_SIM_WRITER_H
#define CACHE_SIM_WRITER_H

#include <fstream>

#include "defines.h"

#define WRITE 0x1000

class Writer {

public:
    explicit Writer(const char *filepath) : fout(filepath, std::ios::out) {
        printf("Trace log output file set: %s\n", filepath);
    }

    inline void hit() {
        fout << "Hit\n";
    }

    inline void miss() {
        fout << "Miss\n";
    }

    inline void close() {
        fout.flush();
        fout.close();
    }

private:
    std::fstream fout;

};

#endif //CACHE_SIM_WRITER_H
