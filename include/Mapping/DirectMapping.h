//
// Created by Randy Mei on 2020/4/3.
//

#ifndef CACHE_SIM_DIRECT_MAPPING_H
#define CACHE_SIM_DIRECT_MAPPING_H

#define U8A1 ((u8)(0xFF))

#include "MappingPolicy.h"
#include "BlockPolicy.h"
#include "WritemissPolicy.h"

class DirectMapping : public MappingPolicy {

public:
    BlockRecord mappingTo(u64 blockIndex) override {
        return BlockRecord{static_cast<u32>(blockIndex % block()->blockNum()), 0, 1};
    }

    void init() override {
        // set bit width
        _bitWidth = 1; // valid bit
        _tagWidth = 47; // tag = 64 - offset_b - (17 - offset_b)
                        // offset_b = log2(block_size)
        _bitWidth += _tagWidth;
        if (handler()->writemiss()->writeBack()) _bitWidth++; // dirty bit

        // set V and D
        V = 0b1u;
        u32 wasted = bitWidthUsed() - bitWidth();
        V <<= (7u - wasted);
        D = V >> 1u;

        RV = ~V;
        RD = ~D;

        // meta allocate
        meta = new u8*[block()->blockNum()];
        for (int i = 0, size = block()->blockNum(); i < size; i++) {
            meta[i] = new u8[bitWidthUsed() >> 3u]();
        }
    }

    bool valid(const u32 index) override {
        return meta[index][0] & V;
    }

    void setValid(const u32 index, const bool value) override {
        if (value) {
            meta[index][0] |= V;
        } else {
            meta[index][0] = meta[index][0] & RV;
        }
    }

    bool dirty(const u32 index) override {
        return handler()->writemiss()->writeBack() && (meta[index][0] & D);
    }

    void setDirty(const u32 index, const bool value) override {
        if (!handler()->writemiss()->writeBack()) return;
        if (value) {
            meta[index][0] |= D;
        } else {
            meta[index][0] = meta[index][0] & RD;
        }
    }

    bool checkTag(const u32 index, const u64 address) override {
        u64 addr = address;
        u32 tag = tagWidth(), i = (bitWidthUsed() >> 3u) - 1;
        u32 left = i - (tag >> 3u);
        // set full used byte (left, i]
        for (; i != left; i--) {
            if (meta[index][i] != (addr & U8A1)) {
                return false;
            }
            addr >>= 8u;
            tag -= 8;
        }
        u32 shift = 8u - tag;
        return (shift == 8u) || ((meta[index][left] << shift) == ((addr << shift) & U8A1));
    }

    void setTag(const u32 index, const u64 address) override {
        // set full used byte
        u64 addr = address;
        u32 tag = tagWidth(), i = (bitWidthUsed() >> 3u) - 1;
        u32 left = i - (tag >> 3u);
        for (; i != left; i--) {
            meta[index][i] = addr & U8A1;
            addr >>= 8u;
            tag -= 8;
        }
        if (tag == 0u) return;

        // reset tag_bit with 0 in meta[index][left]
        u32 shift = 8u - tag;
        meta[index][left] = ((unsigned) meta[index][left] >> shift) << shift;

        // set meta[index][left] using address
        u8 p = 0b1u;
        while (tag) {
            meta[index][0] |= addr & p;
            p <<= 1u;
            tag--;
        }
    }

private:
    u8 V{};
    u8 D{};
    u8 RV{};
    u8 RD{};

};

#endif //CACHE_SIM_DIRECT_MAPPING_H
