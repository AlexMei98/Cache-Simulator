//
// Created by Randy Mei on 2020/4/4.
//

#ifndef CACHE_SIM_FULL_CONNECT_MAPPING_H
#define CACHE_SIM_FULL_CONNECT_MAPPING_H

#include "MappingPolicy.h"

class FullConnectMapping : public MappingPolicy {
    BlockRecord mappingTo(u64 blockIndex) override {
        // TODO
        return BlockRecord {0, 0, 0};
    }

    void init() override {
        // TODO
    }

    bool valid(u32 index) override {
        // TODO
        return true;
    }

    void setValid(u32 index, bool value) override {
        // TODO
    }

    bool dirty(u32 index) override {
        // TODO
        return true;
    }

    void setDirty(u32 index, bool value) override {
        // TODO
    }

    bool checkTag(u64 address, u32 index) override {
        // TODO
        return true;
    }

    void setTag(u64 address, u32 index) override {
        // TODO
    }

};

#endif //CACHE_SIM_FULL_CONNECT_MAPPING_H
