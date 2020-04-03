//
// Created by Randy Mei on 2020/4/3.
//

#include <cstdio>
#include <fstream>
#include <string>

#include "Utils.h"

#ifndef CACHE_SIM_TRACE_READER_H
#define CACHE_SIM_TRACE_READER_H

class Reader {
public:
    explicit Reader(const char* filename) {
        this->filepath = filename;
        printf("trace file set: %s\n", this->filepath);
        this->fin.open(filepath, std::ios::in);
    }

    bool next_op(Op &next) {
        std::string temp;
        bool flag;
        while (is_empty(temp)) if (!getline(this->fin, temp)) return false;
        next = parse_line(temp);
        return true;
    }

private:
    const char* filepath;
    std::fstream fin;
};

#endif //CACHE_SIM_TRACE_READER_H
