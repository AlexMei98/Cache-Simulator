//
// Created by Randy Mei on 2020/4/3.
//

#ifndef CACHE_SIM_REPLACEMENT_POLICY_H
#define CACHE_SIM_REPLACEMENT_POLICY_H

#include "defines.h"
#include "Handler.h"

class ReplacementPolicy {
public:
    void registerHandler(Handler *handler) {
        _handler = handler;
        _writemiss = handler->writemiss();
    }

    inline Handler* handler() const {
        return _handler;
    }

    inline WritemissPolicy* writemiss() const {
        return _writemiss;
    }

    virtual void update(u64 index, BlockRecord record) = 0;

    virtual void load(u64 index, BlockRecord record) = 0;

private:
    Handler *_handler{};
    WritemissPolicy *_writemiss{};
};

#endif //CACHE_SIM_REPLACEMENT_POLICY_H
