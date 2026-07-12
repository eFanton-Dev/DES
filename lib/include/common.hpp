#pragma once

#include <stdlib.h>

#define BYTES const char*

constexpr size_t INITKEYLEN = 64;
constexpr size_t KEYLEN = 56;
constexpr size_t SUBKEYLEN = 48;
constexpr size_t ROUNDNUM = 16;

constexpr size_t BLOCKSIZE = 64 / 8;            //size of a block in bytes

typedef struct {
    bool error = false;
    bool help = false;

    bool encrypt = true;
    bool threeDES = false;

    std::string plaintext;
    std::string filePath;

    std::string keys[2] = {"", ""};
} Command;