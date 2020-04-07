//
// Created by Randy Mei on 2020/4/3.
//

#ifndef CACHE_SIM_DIRECT_MAPPING_H
#define CACHE_SIM_DIRECT_MAPPING_H

#include "MappingPolicy.h"

class DirectMapping : public MappingPolicy {

public:
    BlockRecord mappingTo(u64 blockIndex) override {
        return BlockRecord{static_cast<u32>(blockIndex % block()->blockNum()), 0, 1};
    }

    void initTagWidth() override {
        _tagWidth = 64 - handler()->bitWidth(); // tag = 64 - offset_b - (20 - offset_b)
                                                // bitWidth() == (log2(blockNum * blockSize) == 20)
    }

    void initOthers() override {
        _n = 1;
    }

    const char *me() override {
        return "Direct";
    }

};

#endif //CACHE_SIM_DIRECT_MAPPING_H
