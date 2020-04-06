//
// Created by Randy Mei on 2020/4/3.
//

#ifndef CACHE_SIM_OP_H
#define CACHE_SIM_OP_H

#define _DEBUG
#ifdef _DEBUG
#include <string>
#endif

#include "defines.h"

struct Op {
    bool w; // 0 -> read / load / no-mode
            // 1 -> write / store

    u64 addr; // operand address

#ifdef _DEBUG
    std::string line;
    Op() : w(false), addr(0), line() {}
#else
    Op() : w(false), addr(0) {}
#endif

};

#endif //CACHE_SIM_OP_H
