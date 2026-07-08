#pragma once

#include <stdlib.h>
#include <memory>
#include <bitset>

#include "utils.hpp"

#ifdef TESTING
    class KeyScheduleTest;
#endif


#define BYTES const char*

#define INITKEYLEN 64
#define KEYLEN 56
#define SUBKEYLEN 48
#define ROUNDNUM 16

class KeySchedule
{
    
#ifdef TESTING
    friend class KeyScheduleTest;
#endif

private:
    std::unique_ptr<std::bitset<64>> key;
    std::bitset<48> subKeys[16];

    const size_t PC1_table[56] = {
        57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
        10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
        14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
    };
    const size_t PC2_table[48] = {
        14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
        23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
        41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
    };
    const size_t shift_table[16] = {
        1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
    };
    
    void gen_round_key(size_t round, std::bitset<28> &left, std::bitset<28> &right);
public:
    KeySchedule(BYTES key);
    KeySchedule(unsigned long long key);

    void gen_subkeys();
    std::bitset<48> fetch_round_key(int round);
};

