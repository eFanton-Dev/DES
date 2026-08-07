#include "DES.hpp"

// public:

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

std::string DES::encrypt(
        std::string msg,
        std::variant<std::string, unsigned long> param,
        Modes mode
) {
    switch (mode)
    {
    case Modes::ECB:
        return encrypt_ECB(msg);

    case Modes::CBC: {
        const std::string* iv = std::get_if<std::string>(&param);
        if (!iv) return "";
        if (iv->length() != 8) return "";

        return encrypt_CBC(msg, *iv);
    }

    case Modes::PCBC: {
        const std::string* iv = std::get_if<std::string>(&param);
        if (!iv) return "";
        if (iv->length() != 8) return "";

        return encrypt_PCBC(msg, *iv);
    }

    case Modes::CTR: {
        const unsigned long* ctr = std::get_if<unsigned long>(&param);
        if (!ctr) return "";

        return encrypt_CTR(msg, *ctr);
    }
    
    default:
        return "";
    }

    return "";
}

std::string DES::decrypt(
        std::string msg,
        std::variant<std::string, unsigned long> param,
        Modes mode
) {
    switch (mode)
    {
    case Modes::ECB:
        return decrypt_ECB(msg);

    case Modes::CBC: {
        const std::string* iv = std::get_if<std::string>(&param);
        if (!iv) return "";
        if (iv->length() != 8) return "";

        return decrypt_CBC(msg, *iv);
    }

    case Modes::PCBC: {
        const std::string* iv = std::get_if<std::string>(&param);
        if (!iv) return "";
        if (iv->length() != 8) return "";

        return decrypt_PCBC(msg, *iv);
    }

    case Modes::CTR: {
        const unsigned long* ctr = std::get_if<unsigned long>(&param);
        if (!ctr) return "";

        return decrypt_CTR(msg, *ctr);
    }
    
    default:
        return "";
    }

    return "";
}

//private:

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
    std::bitset<64> pm_block = util::permutation<64, 64>(block, perm_table); //initial permutation

    std::bitset<32> right = util::copy<64, 32>(pm_block, 0);
    std::bitset<32> left = util::copy<64, 32>(pm_block, 32);

    for (size_t i = 0; i < ROUNDNUM; i++)
    {
        std::pair<std::bitset<32>, std::bitset<32>> res = round(left, right, i);
        left = res.first;
        right = res.second;
    }

    std::bitset<64> c_block = util::concat<32, 32>(right, left);    

    return util::permutation<64, 64>(c_block, inverse_perm_table);
}


std::string DES::encrypt_ECB(std::string msg) {
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

std::string DES::encrypt_CBC(std::string msg, std::string iv) {
    std::bitset<64> propagation[(msg.length() / BLOCKSIZE) + 1];
    propagation[0] = util::str_to_bitset<64>(iv);

    std::string chipertext;

    for (size_t i = 0; i < msg.length() / BLOCKSIZE; i++)
    {
        std::string block = msg.substr(i * BLOCKSIZE, BLOCKSIZE);

        std::bitset<64> block_bs = util::str_to_bitset<64>(block);
        block_bs ^= propagation[i];

        block_bs = encrypt_block(block_bs);

        propagation[i + 1] = block_bs;
        chipertext += util::bitset_to_str<64>(block_bs);
    }

    return chipertext;
}

std::string DES::encrypt_PCBC(std::string msg, std::string iv) {
    std::bitset<64> propagation[(msg.length() / BLOCKSIZE) + 1];
    propagation[0] = util::str_to_bitset<64>(iv);

    std::string chipertext;

    for (size_t i = 0; i < msg.length() / BLOCKSIZE; i++)
    {
        std::string block = msg.substr(i * BLOCKSIZE, BLOCKSIZE);

        std::bitset<64> plainblock_bs = util::str_to_bitset<64>(block);
        std::bitset<64> block_bs = plainblock_bs;
        block_bs ^= propagation[i];
        
        block_bs = encrypt_block(block_bs);

        propagation[i + 1] = block_bs ^ plainblock_bs;
        chipertext += util::bitset_to_str<64>(block_bs);
    }

    return chipertext;
}

std::string DES::encrypt_CTR(std::string msg, unsigned long ctr_start) {
    std::string chipertext;
    unsigned long counter = ctr_start;

    for (size_t i = 0; i < msg.length(); i+=BLOCKSIZE)
    {
        std::string block = msg.substr(i, BLOCKSIZE);

        std::bitset<64> block_bs = util::str_to_bitset<64>(block);
        std::bitset<64> counter_bs(counter);

        counter_bs = encrypt_block(counter_bs);

        block_bs ^= counter_bs;

        block = util::bitset_to_str<64>(block_bs);
        chipertext += block;
        counter++;
    }

    return chipertext;
}

std::bitset<64> DES::decrypt_block(std::bitset<64> &block) {
    std::bitset<64> pm_block = util::permutation<64, 64>(block, perm_table); //initial permutation

    std::bitset<32> right = util::copy<64, 32>(pm_block, 0);
    std::bitset<32> left = util::copy<64, 32>(pm_block, 32);

    for (int i = ROUNDNUM - 1; i >= 0; i--)    
    {
        std::pair<std::bitset<32>, std::bitset<32>> res = round(left, right, i);
        left = res.first;
        right = res.second;
    }

    std::bitset<64> c_block = util::concat<32, 32>(right, left);    

    return util::permutation<64, 64>(c_block, inverse_perm_table);
}

std::string DES::decrypt_ECB(std::string msg) {
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

std::string DES::decrypt_CBC(std::string msg, std::string iv) {
    std::bitset<64> propagation[(msg.length() / BLOCKSIZE) + 1];
    propagation[0] = util::str_to_bitset<64>(iv);
    
    std::string plaintext;

    for (size_t i = 0; i < msg.length() / BLOCKSIZE; i++)
    {
        std::string block = msg.substr(i * BLOCKSIZE, BLOCKSIZE);

        std::bitset<64> block_bs = util::str_to_bitset<64>(block);
        propagation[i + 1] = block_bs;

        block_bs = decrypt_block(block_bs);

        block_bs ^= propagation[i];

        block = util::bitset_to_str<64>(block_bs);
        plaintext += block;
    }

    return plaintext;
}

std::string DES::decrypt_PCBC(std::string msg, std::string iv) {
    std::bitset<64> propagation[(msg.length() / BLOCKSIZE) + 1];
    propagation[0] = util::str_to_bitset<64>(iv);
    
    std::string plaintext;

    for (size_t i = 0; i < msg.length() / BLOCKSIZE; i++)
    {
        std::string block = msg.substr(i * BLOCKSIZE, BLOCKSIZE);

        std::bitset<64> chiperblock_bs = util::str_to_bitset<64>(block);
        std::bitset<64> block_bs = decrypt_block(chiperblock_bs);
        block_bs ^= propagation[i];
        

        block = util::bitset_to_str<64>(block_bs);
        plaintext += block;

        propagation[i + 1] = block_bs ^ chiperblock_bs;
    }

    return plaintext;
}

std::string DES::decrypt_CTR(std::string msg, unsigned long ctr_start) {
    std::string plaintext;
    unsigned long counter = ctr_start;

    for (size_t i = 0; i < msg.length(); i+=BLOCKSIZE)
    {
        std::string block = msg.substr(i, BLOCKSIZE);

        std::bitset<64> block_bs = util::str_to_bitset<64>(block);
        std::bitset<64> counter_bs(counter);

        counter_bs = encrypt_block(counter_bs);
        block_bs ^= counter_bs;

        block = util::bitset_to_str<64>(block_bs);
        plaintext += block;

        counter++;
    }

    return plaintext;
}