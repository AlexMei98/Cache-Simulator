//
// Created by Randy Mei on 2020/4/4.
//

#ifndef CACHE_SIM_RANDOM_REPLACE_H
#define CACHE_SIM_RANDOM_REPLACE_H

#include <Mapping/MappingPolicy.h>
#include "ReplacementPolicy.h"

class RandomReplace : public ReplacementPolicy {
    void update(u64 index, BlockRecord record) override {
        // TODO
    }

    void load(u64 index, BlockRecord record) override {
        // TODO
    }
};

#endif //CACHE_SIM_RANDOM_REPLACE_H
