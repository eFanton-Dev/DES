#include "KeySchedule.hpp"


KeySchedule::KeySchedule(BYTES key) {
    this->key = std::make_unique<std::bitset<64>>(key);
    
    //this->key(new std::bitset<64>(key));
}

KeySchedule::KeySchedule(unsigned long long key) {
    this->key = std::make_unique<std::bitset<64>>(key);
}


void KeySchedule::gen_round_key(
    size_t round,
    std::bitset<28> &left,
    std::bitset<28> &right
) {
    left = util::circular_left_shift(left, shift_table[round]);
    right = util::circular_left_shift(right, shift_table[round]);

    std::bitset<56> subkey = util::concat<28, 28>(left, right);

    subKeys[round] = util::permutation<56, SUBKEYLEN>(subkey, PC2_table); //PC2
}

void KeySchedule::gen_subkeys() {
    std::bitset<56> perm_key = util::permutation<64, KEYLEN>((*this->key), PC1_table); //PC1

    std::bitset<28> right = util::copy<56, 28>(perm_key, 0);
    std::bitset<28> left = util::copy<56, 28>(perm_key, 28);
    
    for (size_t i = 0; i < ROUNDNUM; i++)
        gen_round_key(i, left, right);
    
}

std::bitset<48> KeySchedule::fetch_round_key(int round) {
    return subKeys[round];
}