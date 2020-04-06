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
    const u64 index = block()->getIndex(op.addr); // block index
    BlockRecord record = mapping()->mappingTo(index); // find available place
    for (u32 i = 0; i < record.n; i++) {
        u32 t_index = i * record.jump + record.start;
        if (!mapping()->valid(t_index)) continue; // not valid continue
        if (mapping()->checkTag(t_index, op.addr)) {
            hit();
            replacement()->update(op, t_index, record); // update hit record
            return true;
        }
    }
    miss();
    if (op.w && !writemiss()->writeAllocate()) return true;
    for (u32 i = 0; i < record.n; i++) { // load to invalid cache block
        u32 t_index = i * record.jump + record.start;
        if (mapping()->valid(t_index)) continue;
        replacement()->load(op, t_index);
        return true;
    }
    replacement()->replace(op, record); // cache full (available places), replace used block
    return true;
}

void Handler::summary() {
    printf("---------------- Summary ----------------\n");
    block()->print();
    mapping()->print();
    replacement()->print();
    writemiss()->print();
    printf("------------------ End ------------------\n");
    printf("Miss rate: %0.3f%%\n", 100.0f - 100 * (double(hitNum)) / (double(opNum)));
}
