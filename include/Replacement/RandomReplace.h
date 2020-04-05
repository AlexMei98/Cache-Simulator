//
// Created by Randy Mei on 2020/4/4.
//

#ifndef CACHE_SIM_RANDOM_REPLACE_H
#define CACHE_SIM_RANDOM_REPLACE_H

#include <random>
#include <ctime>

#include "Handler.h"
#include "Mapping/MappingPolicy.h"
#include "ReplacementPolicy.h"

class RandomReplace : public ReplacementPolicy {

public:
    explicit RandomReplace(u32 seed = 0u, bool rand = false) : randint(rand ? time(nullptr) : seed) {}

    void update(const Op op, const u32 index, const BlockRecord record) override {
        if (op.w) {
            handler()->mapping()->setDirty(index, true);
        }
    }

    void load(const Op op, const BlockRecord record) override {
        u32 victim_index =  record.start + (static_cast<u32>(randint()) % record.n) * record.jump;
        handler()->mapping()->setValid(victim_index, true);
        handler()->mapping()->setDirty(victim_index, false);
        handler()->mapping()->setTag(victim_index, op.addr);
    }

private:
    std::default_random_engine randint;

};

#endif //CACHE_SIM_RANDOM_REPLACE_H
