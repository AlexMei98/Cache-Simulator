//
// Created by Randy Mei on 2020/4/3.
//

#ifndef CACHE_SIM_MAPPING_POLICY_H
#define CACHE_SIM_MAPPING_POLICY_H

#include "defines.h"
#include "Handler.h"

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

    virtual bool valid(u32 index) = 0;

    virtual void setValid(u32 index, bool value) = 0;

    virtual bool dirty(u32 index) = 0;

    virtual void setDirty(u32 index, bool value) = 0;

    virtual bool checkTag(u64 address, u32 index) = 0;

    virtual void setTag(u64 address, u32 index) = 0;

    // getters

    u32 bitWidthUsed() {
        return ((_bitWidth & 0b111u) ? (_bitWidth >> 3u) + 1u : _bitWidth >> 3u) << 3u;
    }

    u32 bitWidth() {
        return _bitWidth;
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

private:
    Handler *_handler{};
    BlockPolicy *_block{};
};

#endif //CACHE_SIM_MAPPING_POLICY_H
