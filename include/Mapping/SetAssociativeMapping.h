//
// Created by Randy Mei on 2020/4/6.
//

#ifndef CACHE_SIM_SET_ASSOCIATIVE_MAPPING_H
#define CACHE_SIM_SET_ASSOCIATIVE_MAPPING_H

#include "MappingPolicy.h"

class SetAssociativeMapping : public MappingPolicy {

public:
    explicit SetAssociativeMapping(u32 n = 8) : _n(n) {}

    BlockRecord mappingTo(u64 blockIndex) override {
        return BlockRecord {n() * (static_cast<u32>(blockIndex & getSetIndex)), 1, n()};
    }

    void initTagWidth() override {
        _tagWidth = 64 - handler()->block()->shift() - static_cast<u32>(log2(setNum()));
    }

    void initOthers() override {
        // other initial
        _setNum = handler()->block()->blockNum() / n();
        _setCapacity = 61 / handler()->block()->blockSize() / setNum();
        getSetIndex = setNum() - 1;
    }

    const char *me() override {
        switch (n()) {
            case  4: return "4-way set associative";
            case  8: return "8-way set associative";
            default: printf("Invalid param: %d", n()); exit(0);
        }
    }

    inline u32 setNum() const {
        return _setNum;
    }

    inline u32 n() {
        return _n;
    }

private:
    u32 _setNum{};
    u32 _setCapacity{};
    u32 _n;
    u32 getSetIndex{};
};

#endif //CACHE_SIM_SET_ASSOCIATIVE_MAPPING_H
