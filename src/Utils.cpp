//
// Created by Randy Mei on 2020/4/5.
//

#ifndef CACHE_SIM_UTILS_CPP
#define CACHE_SIM_UTILS_CPP


#include <string>

#include "Utils.h"

inline bool isBlank(char c) {
    return c == ' ' || c == '\t' || c == '\0';
}

bool isEmpty(const std::string& s) {
    for (char c : s) {
        if (!isBlank(c)) return false;
    }
    return true;
}

u64 str2ull(std::string s, int start, int end) {
    u64 x = 0, tmp;
    for (int i = start; i < end; i++) {
        if (s[i] >= 'a' && s[i] <= 'z') tmp = s[i] - 'a' + 10; else tmp = s[i] - '0';
        x = (x << 4u) + tmp;
    }
    return x;
}

Op parseLine(std::string line) {
    Op op;
#ifdef _DEBUG
    op.line = line;
#endif
    for (char & c : line) if (c >= 'A' && c <= 'Z') c += 'a' - 'A';

    // r / w
    bool r = line[0] == 'r' || line[0] == 'l';
    bool w = line[0] == 'w' || line[0] == 's';
    if (w) op.w = true;
    int start = (r || w) ? 1 : 0;
    int end = line.size();

    // address
    while (isBlank(line[start])) start++;
    while (isBlank(line[end - 1])) end--;
    if (start + 3 < end && (line[start] == '0' && line[start + 1] == 'x')) start += 2;
    op.addr = str2ull(line, start, end);
    return op;
}

#endif //CACHE_SIM_UTILS_CPP
