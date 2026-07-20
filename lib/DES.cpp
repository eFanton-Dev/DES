#include "DES.hpp"

DES::DES(BYTES key) {
    this->key = new KeySchedule(key);

    this->key->gen_subkeys();
}

DES::DES(unsigned long long key) {
    this->key = new KeySchedule(key);

    this->key->gen_subkeys();
}

DES::~DES() {
    delete key;
}

std::bitset<4> DES::S_box(const std::bitset<6> &input, size_t SBox_num) {
    size_t y = (input.to_ulong() & 0b000001) + ((input >> 4).to_ulong() & 0b000010);
    size_t x = (input >> 1).to_ulong() & 0b001111;

    size_t res = S_boxes[SBox_num][y][x];
    return std::bitset<4>(res);
}

std::bitset<32> DES::fistel(const std::bitset<32> &h_block, std::bitset<48> &subkey) {
    std::bitset<48> expanded = util::permutation<32, 48>(h_block, expansion_table); //expantion

    expanded ^= subkey;

    std::string result;
    for (size_t i = 0; i < 8; i++)
    {
        std::bitset<6> chunk = util::copy<48, 6>(expanded, 48 - (i + 1) * 6);
        std::bitset<4> res = S_box(chunk, i);

        result = result + res.to_string();
    }

    std::bitset<32> res(result);
    return util::permutation<32, 32>(res, P_box);
}

std::pair<std::bitset<32>, std::bitset<32>> DES::round(
    std::bitset<32> &left, 
    std::bitset<32> &right, 
    size_t round
) {
    std::bitset<48> subkey = key->fetch_round_key(round);
    std::bitset<32> result = fistel(right, subkey);

    result ^= left;
    return std::make_pair(right, result);
}

std::bitset<64> DES::encrypt_block(std::bitset<64> &block) {
    block = util::permutation<64, 64>(block, perm_table); //initial permutation

    std::bitset<32> right = util::copy<64, 32>(block, 0);
    std::bitset<32> left = util::copy<64, 32>(block, 32);

    for (size_t i = 0; i < ROUNDNUM; i++)
    {
        std::pair<std::bitset<32>, std::bitset<32>> res = round(left, right, i);
        left = res.first;
        right = res.second;
    }

    std::bitset<64> c_block = util::concat<32, 32>(right, left);    

    return util::permutation<64, 64>(c_block, inverse_perm_table);
}


std::string DES::encrypt(std::string msg) {
    std::string chipertext;
    for (size_t i = 0; i < msg.length(); i+=BLOCKSIZE)
    {
        std::string block = msg.substr(i, BLOCKSIZE);

        std::bitset<64> block_bs = util::str_to_bitset<64>(block);

        block_bs = encrypt_block(block_bs);

        block = util::bitset_to_str<64>(block_bs);
        chipertext += block;
    }

    return chipertext;
}

std::bitset<64> DES::decrypt_block(std::bitset<64> &block) {
    block = util::permutation<64, 64>(block, perm_table); //initial permutation

    std::bitset<32> right = util::copy<64, 32>(block, 0);
    std::bitset<32> left = util::copy<64, 32>(block, 32);

    for (int i = ROUNDNUM - 1; i >= 0; i--)    
    {
        std::pair<std::bitset<32>, std::bitset<32>> res = round(left, right, i);
        left = res.first;
        right = res.second;
    }

    std::bitset<64> c_block = util::concat<32, 32>(right, left);    

    return util::permutation<64, 64>(c_block, inverse_perm_table);
}

std::string DES::decrypt(std::string msg) {
    std::string plaintext;

    for (size_t i = 0; i < msg.length(); i+=BLOCKSIZE)
    {
        std::string block = msg.substr(i, BLOCKSIZE);

        std::bitset<64> block_bs = util::str_to_bitset<64>(block);

        block_bs = decrypt_block(block_bs);

        block = util::bitset_to_str<64>(block_bs);
        plaintext += block;
    }

    return plaintext;
}
