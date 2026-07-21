#include <gtest/gtest.h>

#include "DES.hpp"

class DESTest : public ::testing::Test
{
protected:
    DES des{0xa44598e67a54bf1c};

    std::bitset<4> S_Box(const std::bitset<6> &input, size_t SBox_num) {
        return des.S_box(input, SBox_num);
    }

    std::bitset<32> fistel(const std::bitset<32> &h_block, std::bitset<48> &subkey) {
        return des.fistel(h_block, subkey);
    }

    std::pair<std::bitset<32>, std::bitset<32>> round(std::bitset<32> &left, std::bitset<32> &right) {
        return des.round(left, right, 1);
    }

    std::bitset<64> encrypt_block(std::bitset<64> &block) {
        return des.encrypt_block(block);
    }

    std::bitset<64> decrypt_block(std::bitset<64> &block) {
        return des.decrypt_block(block);
    }

    std::string encrypt_ECB(std::string msg) {
        return des.encrypt_ECB(msg);
    }

    std::string decrypt_ECB(std::string msg) {
        return des.decrypt_ECB(msg);
    }

    std::string encrypt_CBC(std::string msg, std::string iv) {
        return des.encrypt_CBC(msg, iv);
    }

    std::string decrypt_CBC(std::string msg, std::string iv) {
        return des.decrypt_CBC(msg, iv);
    }
};

TEST_F(DESTest, SBox_test) {
    std::bitset<4> res;
    size_t expected;

    res = S_Box(std::bitset<6>("000010"), 2);
    expected = 0;

    ASSERT_EQ(res.to_ulong(), expected);

    res = S_Box(std::bitset<6>("110010"), 0);
    expected = 12;

    ASSERT_EQ(res.to_ulong(), expected);
}

TEST_F(DESTest, fistel_test) {
    std::bitset<32> half_block(0b10010111010110000111010011010111);
    std::bitset<48> subkey(0b110101001010110100001001011111011010100010100101);
    std::bitset<32> res = fistel(half_block, subkey);

    std::bitset<32> expected(0b01111100001001101101100001111000);
    ASSERT_EQ(res.to_string(), expected.to_string());

    half_block = std::bitset<32>(0b10010111010110000111010011010111);
    subkey = std::bitset<48>(0b001100110101110110100010111100100001101100110100);

    res = fistel(half_block, subkey);
    expected = std::bitset<32>(0b00111101010010110110000100100101);
    ASSERT_EQ(res.to_string(), expected.to_string());
}

TEST_F(DESTest, round_test) {
    std::bitset<32> left(0b10110100100000111010011110101010);
    std::bitset<32> right(0b10010111010110000111010011010111);

    std::pair<std::bitset<32>, std::bitset<32>> res = round(left, right);

    std::bitset<32> expected_left(0b10010111010110000111010011010111);
    std::bitset<32> expected_right(0b10001001110010001100011010001111);
    EXPECT_EQ(res.first.to_string(), expected_left.to_string());
    EXPECT_EQ(res.second.to_string(), expected_right.to_string());
}

TEST_F(DESTest, encryptBlock_test) {
    std::bitset<64> block(0x48656c6c6f20576f);
    std::bitset<64> res = encrypt_block(block);

    std::bitset<64> expected(0xb073038f76c1c5e8);
    ASSERT_EQ(res.to_string(), expected.to_string());
}

TEST_F(DESTest, encryptECB_test) {
    std::string plaintext = "Hello World";
    std::string padding = util::gen_padding(plaintext, BLOCKSIZE);
    plaintext = plaintext + padding;

    std::string chipertext = encrypt_ECB(plaintext);
    std::cout << chipertext << std::endl;

    const char data[] =
    "\xb0\x73\x03\x8f\x76\xc1\xc5\xe8\x40\x52\x8d\x35\x00\xff\x1b\x32";

    std::string expected(data, sizeof(data) - 1);

    ASSERT_EQ(chipertext, expected);
}

TEST_F(DESTest, encryptCBC_test) {
    std::string plaintext = "Hello World";
    std::string padding = util::gen_padding(plaintext, BLOCKSIZE);
    plaintext = plaintext + padding;

    std::string chipertext = encrypt_CBC(plaintext, "12345678");
    std::cout << chipertext << std::endl;

    const char data[] =
    "\x12\xa1\x3f\xe8\xa6\x79\x9d\x5a\xe4\x5f\x24\x6e\x89\x55\x3a\x36";

    std::string expected(data, sizeof(data) - 1);

    ASSERT_EQ(chipertext, expected);
}

TEST_F(DESTest, decryptBlock_test) {
    std::bitset<64> block(0xb073038f76c1c5e8);
    std::bitset<64> res = decrypt_block(block);

    std::bitset<64> expected(0x48656c6c6f20576f);
    ASSERT_EQ(res.to_string(), expected.to_string());
}

TEST_F(DESTest, dencryptECB_test) {
    const char data[] =
    "\xb0\x73\x03\x8f\x76\xc1\xc5\xe8\x40\x52\x8d\x35\x00\xff\x1b\x32";

    std::string chipertext(data, sizeof(data) - 1);
    std::string plaintext = decrypt_ECB(chipertext);
    std::cout << plaintext << std::endl;

    std::string expected = "Hello World\x05\x05\x05\x05\x05";

    ASSERT_EQ(plaintext, expected);
}

TEST_F(DESTest, dencryptCBC_test) {
    const char data[] =
    "\x12\xa1\x3f\xe8\xa6\x79\x9d\x5a\xe4\x5f\x24\x6e\x89\x55\x3a\x36";

    std::string chipertext(data, sizeof(data) - 1);
    std::string plaintext = decrypt_CBC(chipertext, "12345678");
    std::cout << plaintext << std::endl;

    std::string expected = "Hello World\x05\x05\x05\x05\x05";

    ASSERT_EQ(plaintext, expected);
}