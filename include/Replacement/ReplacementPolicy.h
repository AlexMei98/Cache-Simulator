//
// Created by Randy Mei on 2020/4/3.
//

#ifndef CACHE_SIM_REPLACEMENT_POLICY_H
#define CACHE_SIM_REPLACEMENT_POLICY_H

#include "defines.h"
#include "Handler.h"
#include "Mapping/MappingPolicy.h"

#define U3A1 ((u8)(0b111u))

class ReplacementPolicy {

public:
    void registerHandler(Handler *handler) {
        _handler = handler;
        _writemiss = handler->writemiss();
        init();
    }

    inline Handler *handler() const {
        return _handler;
    }

    inline WritemissPolicy *writemiss() const {
        return _writemiss;
    }

    virtual void init() = 0;

    virtual void update(Op op, u32 index, BlockRecord record) = 0;

    virtual void load(Op op, u32 index) = 0;

    virtual void replace(Op op, BlockRecord record) = 0;

    virtual void print() {
        printf("Replacement Policy\n");
        printf("\t%s Replace\n", me());
    }

    virtual const char* me() const = 0;

private:
    Handler *_handler{};
    WritemissPolicy *_writemiss{};

};

#endif //CACHE_SIM_REPLACEMENT_POLICY_H
