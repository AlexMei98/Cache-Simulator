//
// Created by Randy Mei on 2020/4/3.
//

#include <cstdio>
#include <fstream>
#include <string>

#include "Utils.h"

#ifndef CACHE_SIM_TRACE_READER_H
#define CACHE_SIM_TRACE_READER_H

#include "defines.h"

class Reader {

public:
    explicit Reader(const char* filepath) : fin(filepath, std::ios::in) {
        if (!fin) {
            printf("Trace input file `%s` not exist\n", filepath);
            exit(0);
        }
        printf("Trace input file set: %s\n", filepath);
    }

    bool getNextOp(Op &next) {
        std::string temp;
        bool flag;
        while (isEmpty(temp)) if (!getline(this->fin, temp)) return false;
        next = parseLine(temp);
        return true;
    }

    inline void close() {
        fin.close();
    }

private:
    std::fstream fin;

};

#endif //CACHE_SIM_TRACE_READER_H
