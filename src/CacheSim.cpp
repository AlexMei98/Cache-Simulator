#include <iostream>

#include "Handler.h"
#include "BlockPolicy.h"
#include "Mapping/DirectMapping.h"
#include "Mapping/FullConnectMapping.h"
#include "Replacement/RandomReplace.h"
#include "WritemissPolicy.h"

void handlerTest() {
    // Block Policy
    u32 blockSize = 8;
    auto *block = new BlockPolicy(blockSize);

    // Mapping Policy
    auto *mapping = new DirectMapping();

    // Replacement Policy
    u32 seed = 0u;
    bool rand = false;
    auto *replacement = new RandomReplace(seed, rand);

    // Writemiss Policy
    bool writeBack = true;
    bool writeAllocate = true;
    auto *writemiss = new WritemissPolicy(writeBack, writeAllocate);

    // Utils
    auto *reader = new Reader("../trace/test.trace");
    auto *writer = new Writer("../log/test.log");

    // Main handler
    auto *handler = new Handler(block, mapping, replacement, writemiss, reader, writer);

    handler->processAll();
    handler->summary();
}

int main() {
    handlerTest();
    return 0;
}
