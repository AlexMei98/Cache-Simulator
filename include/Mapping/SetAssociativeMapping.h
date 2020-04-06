//
// Created by Randy Mei on 2020/4/6.
//

#ifndef CACHE_SIM_SET_ASSOCIATIVE_MAPPING_H
#define CACHE_SIM_SET_ASSOCIATIVE_MAPPING_H

#include "MappingPolicy.h"

class SetAssociativeMapping : public MappingPolicy {

    explicit SetAssociativeMapping(u32 n) : _n(n) {}

    BlockRecord mappingTo(u64 blockIndex) override {
        return BlockRecord {0, 0, 0};
    }

    void init() override {
        _setNum = handler()->block()->blockNum() / n();
        _setCapacity = handler()->capacity()
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
    u32 _setNum;
    u32 _setCapacity;
    u32 _n;
};

#endif //CACHE_SIM_SET_ASSOCIATIVE_MAPPING_H
