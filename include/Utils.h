//
// Created by Randy Mei on 2020/4/3.
//

#ifndef CACHE_SIM_UTILS_H
#define CACHE_SIM_UTILS_H

#include <string>

#include "defines.h"
#include "Op.h"

inline bool isBlank(char c);

bool isEmpty(const std::string& s);

u64 str2ull(std::string s, int start, int end);

Op parseLine(std::string line);

#endif //CACHE_SIM_UTILS_CPP
