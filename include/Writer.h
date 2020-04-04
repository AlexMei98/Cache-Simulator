//
// Created by Randy Mei on 2020/4/3.
//

#ifndef CACHE_SIM_WRITER_H
#define CACHE_SIM_WRITER_H

#include <fstream>

#include "defines.h"

class Writer {
public:
    explicit Writer(const char* filepath) {
        printf("trace log output file set: %s\n", filepath);
        this->fout.open(filepath, std::ios::out);
    }

    inline void hit() {
        fout.write("Hit\n", 4);
    }

    inline void miss() {
        fout.write("Miss\n", 5);
    }

private:
    std::ofstream fout;
};

#endif //CACHE_SIM_WRITER_H
