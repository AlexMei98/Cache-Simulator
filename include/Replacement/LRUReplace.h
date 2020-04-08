//
// Created by Randy Mei on 2020/4/7.
//

#ifndef CACHE_SIM_LRU_REPLACE_H
#define CACHE_SIM_LRU_REPLACE_H

#include "ReplacementPolicy.h"
#include "Mapping/MappingPolicy.h"

class LRUReplace : public ReplacementPolicy {

public:
    void init() override {
        _bitWidth = n() * (static_cast<u32>(log2(n())));

        meta = new u8 *[setNum()];
        for (u32 i = 0; i < setNum(); i++) {
            meta[i] = new u8[bitWidthUsed() >> 3u]();
        }

        _shift = static_cast<u32>(log2(n()));
        _index = new u32[n()];
        _start = new u8[n()];
        for (u32 i = 0; i != n(); i++) {
            u32 tmp = i * shift();
            _index[i] = tmp >> 3u;
            _start[i] = static_cast<u8>(tmp & U3A1);
        }
    }

    u8 getIndexInner(const u32 index, const u32 indexInner) const {
        u8 tmp = 0b0u;
        u32 bitIndex = _index[indexInner];
        u8 bitStart = 0b1u << _start[indexInner];
        for (u32 i = 0; i < shift(); i++) {
            tmp |= ((meta[index][bitIndex] & bitStart) ? 0b1u : 0b0u) << i;
            if (bitStart == 0b10000000u) {
                bitIndex += 1;
                bitStart = 0b1u;
            } else {
                bitStart <<= 1u;
            }
        }
        return tmp;
    }

    void setIndexInner(const u32 index, const u32 indexInner, const u8 value) {
        u32 bitIndex = _index[indexInner];
        u8 bitStart = 0b1u << _start[indexInner];
        u8 v = value;
        for (u32 i = 0; i < shift(); i++) {
            if (v & 0b1u) {
                meta[index][bitIndex] |= bitStart;
            } else {
                meta[index][bitIndex] &= static_cast<u8>(~bitStart);
            }
            v >>= 1u;
            if (bitStart == 0b10000000u) {
                bitIndex += 1;
                bitStart = 0b1u;
            } else {
                bitStart <<= 1u;
            }
        }
    }

    void update(const Op op, const u32 index, const BlockRecord record) override {
        if (op.w && handler()->writemiss()->writeBack()) {
            handler()->mapping()->setDirty(index, true);
        }
        u32 indexSet = index >> shift();
        u32 value = index & (n() - 1);
        u8 tmp = value, before;
        for (u32 i = 0; i < n(); i++) {
            before = tmp;
            tmp = getIndexInner(indexSet, i);
            setIndexInner(indexSet, i, before);
            if (tmp == value) break;
        }
    }

    void load(const Op op, const u32 index) override {
        if (op.w && !handler()->writemiss()->writeAllocate()) return;
        handler()->mapping()->setValid(index, true);
        handler()->mapping()->setDirty(index, op.w);
        handler()->mapping()->setTag(index, op.addr);

        u32 indexSet = index >> shift();
        u32 value = index & (n() - 1);
        for (u32 i = n() - 1; i > 0; i--) {
            setIndexInner(indexSet, i, getIndexInner(indexSet, i - 1));
        }
        setIndexInner(indexSet, 0, value);
    }

    void replace(const Op op, const BlockRecord record) override {
        u32 indexSet = record.start >> shift();
        u32 victimIndex = (indexSet << shift()) | getIndexInner(indexSet, n() - 1);
        load(op, victimIndex);
    }

    void print() override {
        printf("Replacement Policy\n");
        printf("\t%s Replace\n", me());
        printf("\tPhysically used bits: %d (%dB)\n", bitWidthUsed(), bitWidthUsed() / 8u);
        printf("\tReal used bits: %d\n", bitWidth());
    }

    const char *me() const override {
        return "LRU";
    }

    u32 n() const {
        return handler()->mapping()->n();
    }

    u32 setNum() const {
        return handler()->block()->blockNum() / handler()->mapping()->n();
    }

    u32 bitWidth() const {
        return _bitWidth;
    }

    u32 bitWidthUsed() const {
        return ((_bitWidth & 0b111u) ? ((_bitWidth >> 3u) + 1u) : (_bitWidth >> 3u)) << 3u;
    }

    u32 shift() const {
        return _shift;
    }

private:
    u8 **meta{};
    u32 _bitWidth{};
    u32 *_index{};
    u8 *_start{};
    u32 _shift{};

};

#endif //CACHE_SIM_LRU_REPLACE_H
