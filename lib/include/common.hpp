#pragma once

#include <stdlib.h>
#include <map>

#define BYTES const char*

constexpr size_t INITKEYLEN = 64;
constexpr size_t KEYLEN = 56;
constexpr size_t SUBKEYLEN = 48;
constexpr size_t ROUNDNUM = 16;

constexpr size_t BLOCKSIZE = 64 / 8;            //size of a block in bytes

enum class Modes:char {
    ECB,
    CBC,
    PCBC,
    OFB,
    CTR
};

const std::map<std::string, Modes> ENUMSTRING = {
    {"ECB", Modes::ECB},
    {"CBC", Modes::CBC},
    {"PCBC", Modes::PCBC},
    {"OFB", Modes::OFB},
    {"CTR", Modes::CTR}
};