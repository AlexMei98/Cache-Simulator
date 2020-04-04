#include <iostream>

#include "Handler.h"
#include "BlockPolicy.h"
#include "Mapping/FullConnectMapping.h"
#include "Replacement/RandomReplace.h"
#include "WritemissPolicy.h"

void handlerTest() {
    auto *block = new BlockPolicy();
    auto *mapping = new FullConnectMapping();
    auto *replacement = new RandomReplace();
    auto *writemiss = new WritemissPolicy();
    auto *reader = new Reader("../trace/test.trace");
    auto *writer = new Writer("../log/test.log");
    auto *handler = new Handler(block, mapping, replacement, writemiss, reader, writer);
    handler->processAll();
}

int main() {

    return 0;
}
