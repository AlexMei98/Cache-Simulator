//
// Created by Randy Mei on 2020/4/4.
//

#ifndef CACHE_SIM_FULL_CONNECT_MAPPING_H
#define CACHE_SIM_FULL_CONNECT_MAPPING_H

#include "MappingPolicy.h"

class FullConnectMapping : public MappingPolicy {

public:
    BlockRecord mappingTo(u64 blockIndex) override {
        // TODO
        return BlockRecord {0, 0, 0};
    }

    void init() override {
        // TODO
    }

    bool valid(const u32 index) override {
        // TODO
        return true;
    }

    void setValid(const u32 index, const bool value) override {
        // TODO
    }

    bool dirty(const u32 index) override {
        // TODO
        return true;
    }

    void setDirty(const u32 index, const bool value) override {
        // TODO
    }

    bool checkTag(const u32 index, const u64 address) override {
        // TODO
        return true;
    }

    void setTag(const u32 index, const u64 address) override {
        // TODO
    }

};

#endif //CACHE_SIM_FULL_CONNECT_MAPPING_H
