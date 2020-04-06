//
// Created by Randy Mei on 2020/4/3.
//

#ifndef CACHE_SIM_MAPPING_POLICY_H
#define CACHE_SIM_MAPPING_POLICY_H

#include "defines.h"
#include "Handler.h"
#include "BlockPolicy.h"
#include "WritemissPolicy.h"

#define U8A1 ((u8)(0xFF))

struct BlockRecord {
    u32 start;
    u32 jump;
    u32 n;

    BlockRecord(u32 start, u32 jump, u32 n) {
        this->start = start;
        this->jump = jump;
        this->n = n;
    }
};

class MappingPolicy {

public:
    void registerHandler(Handler *handler) {
        _handler = handler;
        _block = handler->block();
        init();
    }

    virtual BlockRecord mappingTo(u64 blockIndex) = 0;

    virtual void init() = 0;

    // tag, valid, dirty getters

    bool valid(u32 index) {
        return meta[index][vBit] & V;
    }

    void setValid(const u32 index, const bool value) {
        if (value) {
            meta[index][vBit] |= V;
        } else {
            meta[index][vBit] = meta[index][vBit] & RV;
        }
    }

    bool dirty(const u32 index) {
        return handler()->writemiss()->writeBack() && (meta[index][dBit] & D);
    }

    void setDirty(u32 index, bool value) {
        if (!handler()->writemiss()->writeBack()) return;
        if (value) {
            meta[index][dBit] |= D;
        } else {
            meta[index][dBit] = meta[index][dBit] & RD;
        }
    }

    bool checkTag(u32 index, u64 address) {
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

    void setTag(u32 index, u64 address) {
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
            meta[index][left] |= addr & p;
            p <<= 1u;
            tag--;
        }
    }

    void print() {
        printf("Mapping Policy\n");
        printf("\t%s\n", me());
        printf("\tPhysically used bits: %d (%dB)\n", bitWidthUsed(), bitWidthUsed() / 8u);
        printf("\tReal used bits: %d\n", bitWidth());
    }

    virtual const char *me() = 0;

    // getters

    u32 bitWidthUsed() {
        return ((_bitWidth & 0b111u) ? ((_bitWidth >> 3u) + 1u) : (_bitWidth >> 3u)) << 3u;
    }

    u32 bitWidth() {
        return _bitWidth;
    }

    u32 tagWidth() const {
        return _tagWidth;
    }

    inline Handler* handler() const {
        return _handler;
    }

    inline BlockPolicy* block() const {
        return _block;
    }

protected:
    u8 **meta{};
    u32 _bitWidth{};
    u32 _tagWidth{};

    u8 V{};
    u8 D{};
    u8 RV{};
    u8 RD{};
    u32 dBit{};
    u32 vBit{};

private:
    Handler *_handler{};
    BlockPolicy *_block{};

};

#endif //CACHE_SIM_MAPPING_POLICY_H
