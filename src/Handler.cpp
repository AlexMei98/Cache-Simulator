//
// Created by Randy Mei on 2020/4/4.
//

#include "Handler.h"
#include "BlockPolicy.h"
#include "Mapping/MappingPolicy.h"
#include "WritemissPolicy.h"
#include "Replacement/ReplacementPolicy.h"
#include "Reader.h"
#include "Writer.h"
#include "Utils.h"

Handler::Handler(BlockPolicy *block, MappingPolicy *mapping, ReplacementPolicy *replacement, WritemissPolicy *writemiss,
                 Reader *reader, Writer *writer, u32 capacity) :
        _reader(reader), _writer(writer), _block(block), _mapping(mapping), _replacement(replacement),
        _writemiss(writemiss), _capacity(capacity) {
    block->registerHandler(this);
    mapping->registerHandler(this);
    replacement->registerHandler(this);
}

bool Handler::processSingleLine(Op &op) {
    if (!getNextOp(op)) return false;
    const u64 index = block()->getIndex(op.addr);
    BlockRecord record = mapping()->mappingTo(index);
    for (u32 i = 0; i < record.n; i++) {
        u32 t_index = i * record.jump + record.start;
        if (!mapping()->valid(t_index)) continue;
        if (mapping()->checkTag(t_index, op.addr)) {
            hit();
            replacement()->update(op, t_index, record);
            return true;
        }
    }
    miss();
    replacement()->load(op, record);
    return true;
}
