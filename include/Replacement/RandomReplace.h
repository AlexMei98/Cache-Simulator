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
        if (op.w && handler()->writemiss()->writeBack()) {
            handler()->mapping()->setDirty(index, true);
        }
    }

    void init() override {}

    void load(const Op op, const u32 index) override {
        if (op.w && !handler()->writemiss()->writeAllocate()) return;
        handler()->mapping()->setValid(index, true);
        handler()->mapping()->setDirty(index, op.w);
        handler()->mapping()->setTag(index, op.addr);
    }

    void replace(const Op op, const BlockRecord record) override {
        u32 victim_index = record.start + (static_cast<u32>(randint()) % record.n) * record.jump;
        load(op, victim_index);
    }

    const char *me() override {
        return "Random";
    }

private:
    std::default_random_engine randint;

};

#endif //CACHE_SIM_RANDOM_REPLACE_H
