#include <gtest/gtest.h>

#include "utils.hpp"

TEST(readFile_test, correctRead_test) {
    EXPECT_STREQ(util::read_file("./../../test.txt"), "Hello World");
}

TEST(readFile_test, failedOpen_test) {
    try
    {
        util::read_file("test.txt");
    }
    catch(const std::string error)
    {
        EXPECT_EQ(error, "Failed to open test.txt");
    }
    
}


TEST(padding_test, genPadding_test) {
    std::string message = "Hello World";

    std::string padding = util::gen_padding(message, 64 / 8);
    message += padding;

    std::string expected = "\x48\x65\x6C\x6C\x6F\x20\x57\x6F\x72\x6C\x64\x05\x05\x05\x05\x05";

    ASSERT_EQ(message, expected);

    message = "Hello Wo";

    padding = util::gen_padding(message, 64 / 8);
    message += padding;

    expected = "\x48\x65\x6C\x6C\x6F\x20\x57\x6F\x08\x08\x08\x08\x08\x08\x08\x08";

    ASSERT_EQ(message, expected);
}

TEST(padding_test, removePadding_test) {
    std::string message = "\x48\x65\x6C\x6C\x6F\x20\x57\x6F\x72\x6C\x64\x05\x05\x05\x05\x05";

    message = util::remove_padding(message);

    std::string expected = "Hello World";

    ASSERT_EQ(message, expected);

    message = "\x48\x65\x6C\x6C\x6F\x20\x57\x6F\x08\x08\x08\x08\x08\x08\x08\x08";

    message = util::remove_padding(message);

    expected = "Hello Wo";

    ASSERT_EQ(message, expected);
}


TEST(bitsetOp_test, strToBitset_test) {
    std::string s = "Hello";

    std::bitset<40> b = util::str_to_bitset<40>(s);

    std::bitset<40> expected(0x48656C6C6F);
    EXPECT_EQ(b.to_string(), expected.to_string());
}

TEST(bitsetOp_test, bitsetToStr_test) {
    std::bitset<40> bs(0x48656C6C6F);

    std::string str;

    try
    {
        str = util::bitset_to_str<40>(bs);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    

    std::string expected = "Hello";
    EXPECT_EQ(str, expected);
}

TEST(bitsetOp_test, copy_test) {
    std::bitset<16> complete("0100100001101001");

    std::bitset<8> right = util::copy<16, 8>(complete, 0);
    std::bitset<8> left = util::copy<16, 8>(complete, 8);

    EXPECT_EQ(right.to_string(), "01101001");
    EXPECT_EQ(left.to_string(), "01001000");
}

TEST(bitsetOp_test, concat_test) {
    std::bitset<8> left("01001000");
    std::bitset<8> right("01101001");

    std::bitset<16> complete = util::concat<8, 8>(left, right);

    EXPECT_EQ(complete.to_string(), "0100100001101001");
}

TEST(bitsetOp_test, permutation_test) {
    std::bitset<64> init{0xa44598e67a54bf1c};
    const size_t perm_table[56] = {
        57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
        10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
        14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
    };

    std::bitset<56> res = util::permutation<64, 56>(init, perm_table);
    std::bitset<56> expected{0x4d3a59f58ebd44};

    ASSERT_EQ(res.to_string(), expected.to_string());
}

TEST(bitsetOp_test, circularLeftShift_test) {
    std::bitset<8> complete(0b10010011);

    std::bitset<8> shifted1 = util::circular_left_shift<8>(complete, 1);
    std::bitset<8> shifted2 = util::circular_left_shift<8>(complete, 2);

    EXPECT_EQ(shifted1.to_string(), "00100111");
    EXPECT_EQ(shifted2.to_string(), "01001110");
}