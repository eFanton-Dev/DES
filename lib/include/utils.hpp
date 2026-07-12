#pragma once

#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <bitset>

#include <iostream>

namespace util
{
    std::string read_file(std::string path);
    std::string gen_padding(const std::string &msg, size_t blocksize);
    std::string remove_padding(const std::string &msg);

    template<size_t N>
    std::bitset<N> str_to_bitset(const std::string str) {
        size_t length = str.length();

        if (N < length * 8)
            throw "Insufficent space";
    
        std::bitset<N> result;
        for (size_t i = 0; i < length; i++)
        {
            char byte = str[length - 1 - i];
            for (size_t j = 0; j < 8; j++)
            {
                result[(i * 8) + j] = (byte >> j) & 0b1;
            }
        }
        
        return result;
    }

    template<size_t N>
    std::string bitset_to_str(const std::bitset<N> bs) {
        std::string str;
        
        for (size_t i = 0; i < N; i += 8)
        {
            char c = (bs >> i).to_ullong() & 0xFF;
            str = c + str;
        }
        
        return str;
    }

    template<size_t N, size_t M>
    std::bitset<M> copy(const std::bitset<N>& src, const size_t srcPos) {
        std::bitset<M> chunk;

        for (size_t i = 0; i < M; i++)
            chunk[i] = src[srcPos + i];
        
        return chunk;
    }

    template<size_t N, size_t M>
    std::bitset<N + M> concat(const std::bitset<N>& bs1, const std::bitset<N>& bs2) {
        std::string concat = bs1.to_string() + bs2.to_string();

        return std::bitset<N + M>{concat};
    }

    template<size_t N, size_t M>
    std::bitset<M> permutation(const std::bitset<N>& bs, const size_t* perm_table) {
        std::bitset<M> perm;

        for (size_t i = 0; i < M; i++)
        {
            size_t pos = perm_table[i];
            perm[(M - 1) - i] = bs[N - pos];
        }

        return perm;
    }

    template<size_t N>
    std::bitset<N> circular_left_shift(std::bitset<N> &bitset, size_t shift) {
        std::bitset<N> mask;
        std::bitset<N> shifted;

        for (size_t i = 0; i < shift; i++)
            mask[(shift - 1) - i] = bitset[(N - 1) - i];
        
            
        shifted = (bitset << shift) | mask;

        return shifted;
    }
}


