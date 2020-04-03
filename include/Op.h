//
// Created by Randy Mei on 2020/4/3.
//

#ifndef CACHE_SIM_OP_H
#define CACHE_SIM_OP_H

struct Op {
    bool w; // 0 -> read / load / no-mode
            // 1 -> write / store

    uint64_t addr; // operand address

    Op() : w(false), addr(0) {}
};

#endif //CACHE_SIM_OP_H