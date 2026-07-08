#include <gtest/gtest.h>

#include "KeySchedule.hpp"

class KeyScheduleTest : public ::testing::Test
{
protected:
    KeySchedule ks{0xa44598e67a54bf1c};
    unsigned long long subkeys[ROUNDNUM] = {
        0xfc1f9c394c57,
        0x335da2f21b34,
        0xf829f5912bba,
        0x95e61d753a11,
        0x4717b673207e,
        0xfe98e525b98e,
        0x9be26a2434f7,
        0xa857be6f88e7,
        0x37f68262e32a,
        0x7a1df2f43d0a,
        0xfce05dec127a,
        0x07c71e55fa6a,
        0x6e19b7349c78,
        0xbfa86989bc76,
        0x8b66ba2deeb0,
        0x35e2cf927a29
    };
};


TEST_F(KeyScheduleTest, subkeyGen_test) {
    ks.gen_subkeys();

    for (size_t i = 0; i < ROUNDNUM; i++)
    {
        std::cout << "Round: " << std::to_string(i) << std::endl;

        std::bitset<48> subkey{subkeys[i]};
        std::bitset<48> res = ks.fetch_round_key(i);
        ASSERT_EQ(res.to_string(), subkey.to_string());
        
        std::cout << "Passed" << std::endl;
    }
    
}