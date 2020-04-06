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

    void init() override {
        // set bit width
        _bitWidth = 1; // valid bit
        _tagWidth = 64 - handler()->block()->shift(); // tag = 64 - offset_b
        _bitWidth += _tagWidth;
        if (handler()->writemiss()->writeBack()) _bitWidth++; // dirty bit

        // set V and D
        V = 0b1u;
        u32 wasted = bitWidthUsed() - bitWidth();
        V <<= (7u - wasted);
        RV = ~V;

        // dirty and valid maybe in different u8
        vBit = 0;
        dBit = (handler()->writemiss()->writeBack() && V == 0b1u) ? 1 : 0;

        D = (dBit == 1) ? 0b10000000 : (V >> 1u);
        RD = ~D;
        // meta allocate
        meta = new u8*[block()->blockNum()];
        for (int i = 0, size = block()->blockNum(); i < size; i++) {
            meta[i] = new u8[bitWidthUsed() >> 3u]();
        }
    }

    const char *me() override {
        return "Full connected mapping";
    }

};

#endif //CACHE_SIM_FULL_CONNECT_MAPPING_H
