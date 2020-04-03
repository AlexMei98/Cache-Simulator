//
// Created by Randy Mei on 2020/4/3.
//

#ifndef CACHE_SIM_UTILS_H
#define CACHE_SIM_UTILS_H

#include <string>
#include "Op.h"

bool is_blank(char c) {
    return c == ' ' || c == '\t' || c == '\0';
}

bool is_empty(const std::string& s) {
    for (char c : s) {
        if (!is_blank(c)) return false;
    }
    return true;
}

uint64_t str2ull(std::string s, int start, int end) {
    uint64_t x = 0, tmp;
    for (int i = start; i < end; i++) {
        if (s[i] >= 'a' && s[i] <= 'z') tmp = s[i] - 'a' + 10; else tmp = s[i] - '0';
        x = (x << 4u) + tmp;
    }
    return x;
}

Op parse_line(std::string line) {
    Op op;
    for (char & c : line) if (c >= 'A' && c <= 'Z') c += 'a' - 'A';

    // r / w
    bool r = line[0] == 'r' || line[0] == 'l';
    bool w = line[0] == 'w' || line[0] == 's';
    if (w) op.w = true;
    int start = r || w ? 1 : 0;
    int end = line.size();

    // address
    while (is_blank(line[start])) start++;
    while (is_blank(line[end - 1])) end--;
    if (start + 3 < end && (line[start] == '0' && line[start + 1] == 'x')) start += 2;
    op.addr = str2ull(line, start, end);
    return op;
}

#endif //CACHE_SIM_UTILS_H
