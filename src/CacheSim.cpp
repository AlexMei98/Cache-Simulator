#include <iostream>
#include <string>

#include "Handler.h"
#include "BlockPolicy.h"
#include "Mapping/DirectMapping.h"
#include "Mapping/FullConnectMapping.h"
#include "Mapping/SetAssociativeMapping.h"
#include "Replacement/RandomReplace.h"
#include "Replacement/LRUReplace.h"
#include "WritemissPolicy.h"

void CacheSim(const char *input, const char *output) {
    // Block Policy
    u32 blockSize = 8;
    auto *block = new BlockPolicy(blockSize);

    // Mapping Policy
    u32 n = 8; // Set associative
    auto *mapping = new SetAssociativeMapping(n);
//    auto *mapping = new DirectMapping();
//    auto *mapping = new FullConnectMapping();

    // Replacement Policy
    u32 seed = 0u;
    bool rand = false; // Random
    auto *replacement = new RandomReplace(seed, rand);
//    auto *replacement = new LRUReplace();
    // Writemiss Policy
    bool writeBack = true;
    bool writeAllocate = true;
    auto *writemiss = new WritemissPolicy(writeBack, writeAllocate);

    // Utils
    auto *reader = new Reader(input);
    auto *writer = new Writer(output);

    // Main handler
    auto *handler = new Handler(block, mapping, replacement, writemiss, reader, writer);

    handler->processAll();
    handler->summary();
}

void CacheSimAll() {
    std::string inputL = "../trace/";
    std::string inputR = ".trace";
    std::string outputL = "../log/";
    std::string outputR = ".log";
    std::string cases[10] = {"astar", "bodytrack_1m", "bzip2", "canneal.uniq", "gcc", "mcf", "perlbench",
                             "streamcluster", "swim", "twolf"};
    for (const auto& name : cases) {
        std::string input, output;
        input.append(inputL).append(name).append(inputR);
        output.append(outputL).append(name).append(outputR);
        CacheSim(input.c_str(), output.c_str());
    }
}

int main() {
    CacheSimAll();
    return 0;
}
