//
// Created by Randy Mei on 2020/4/4.
//

#ifndef CACHE_SIM_FULL_CONNECT_MAPPING_H
#define CACHE_SIM_FULL_CONNECT_MAPPING_H

#include "MappingPolicy.h"

class FullConnectMapping : public MappingPolicy {

public:
    BlockRecord mappingTo(u64 blockIndex) override {
        return BlockRecord {0, 1, block()->blockNum()};
    }

    void initTagWidth() override {
        _tagWidth = 64 - handler()->block()->shift(); // tag = 64 - offset_b
    }

    void initOthers() override {
        _n = handler()->block()->blockNum();
    }

    const char *me() override {
        return "Full connected";
    }

};

#endif //CACHE_SIM_FULL_CONNECT_MAPPING_H
