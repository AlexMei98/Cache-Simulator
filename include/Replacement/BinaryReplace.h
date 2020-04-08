//
// Created by Randy Mei on 2020/4/8.
//

#ifndef CACHE_SIM_BINARY_REPLACE_H
#define CACHE_SIM_BINARY_REPLACE_H

#include "ReplacementPolicy.h"

class BinaryReplace : public ReplacementPolicy {

public:
    void init() override {
        _bitWidth = setNum() * (n() - 1);
        _shift = n() - 1;
        meta = new u8[bitWidthUsed() >> 3u];
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

    u32 shift() const {
        return _shift;
    }

    u32 bitWidthUsed() const {
        return ((_bitWidth & 0b111u) ? ((_bitWidth >> 3u) + 1u) : (_bitWidth >> 3u)) << 3u;
    }

    bool getIndexInner(const u32 index, const u32 indexInner) const {
        u32 bitIndex = (index * shift() + indexInner) >> 3u;
        u32 bitShift = (index * shift() + indexInner) & U3A1;
        return (meta[bitIndex] & (0b1u << bitShift));
    }

    void setIndexInner(const u32 index, const u32 indexInner, const bool value) {
        u32 bitIndex = (index * shift() + indexInner) >> 3u;
        u32 bitShift = (index * shift() + indexInner) & U3A1;
        if (value) {
            meta[bitIndex] |= (0b1u << bitShift);
        } else {
            meta[bitIndex] &= static_cast<u8>(~(0b1u << bitShift));
        }
    }

    void update(const Op op, const u32 index, const BlockRecord record) override {
        if (op.w && handler()->writemiss()->writeBack()) {
            handler()->mapping()->setDirty(index, true);
        }
        u32 indexSet = index / n();
        u32 offset = index & (n() - 1);
        u32 tmp = offset + n();
        bool bit;
        while (tmp > 0b1u) {
            bit = (tmp & 0b1u);
            tmp >>= 1u;
            setIndexInner(indexSet, tmp - 1, bit);
        }
    }

    void load(const Op op, const u32 index) override {
        if (op.w && !handler()->writemiss()->writeAllocate()) return;
        handler()->mapping()->setValid(index, true);
        handler()->mapping()->setDirty(index, op.w);
        handler()->mapping()->setTag(index, op.addr);
        update(op, index, BlockRecord(0, 0, 0));
    }

    u32 getLRU(const u32 index) const {
        u32 lru = 1;
        while (lru < n()) {
            if (getIndexInner(index, lru - 1)) {
                lru = static_cast<u32>(lru << 1u) | 0b1u;
            } else {
                lru = static_cast<u32>(lru << 1u);
            }
        }
        return lru - n();
    }

    void replace(const Op op, const BlockRecord record) override {
        u32 indexSet = record.start >> shift();
        u32 victimIndex = (indexSet << shift()) | getLRU(indexSet);
        load(op, victimIndex);
    }

    void print() override {
        printf("Replacement Policy\n");
        printf("\t%s Replace\n", me());
        printf("\tPhysically used bits (full): %d (%dB)\n", bitWidthUsed(), bitWidthUsed() / 8u);
        printf("\tReal used bits (full): %d\n", bitWidth());
    }

    const char* me() const override {
        return "Binary (PLRU)";
    }


private:
    u8 *meta;
    u32 _shift;
    u32 _bitWidth;

};

#endif //CACHE_SIM_BINARY_REPLACE_H
