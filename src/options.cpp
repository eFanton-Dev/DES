#include <string>
#include <iostream>

#include "DES.hpp"

void help_menu() {
    std::string usage = "Usage: .\\DES [flags] [options] (-f <path> | -t <text>) -k <key1>[,<key2>]\n";
    std::string flags = "Flags:\n"\
                            "\t(--encrypt | --decrypt)\n"\
                            "\t\tEncrypt/Decrypt text (default --encrypt)\n"\
                            "\t--3DES\n"\
                            "\t\tUse 3DES instead of DES (needs key2 to be specified)\n"
                            "\t--help\n"\
                            "\t\tDisplay informations\n";
    std::string options = "Options:\n"\
                            "\t-f <file>\n"\
                            "\t\tGet plaintext from <file>\n"\
                            "\t-t <text>\n"\
                            "\t\tSpecify plaintext\n"\
                            "\t-k <key1>[,<key2>]\n"\
                            "\t\tKeys to encrypt/decrypt the text (key2 must be specified if --3DES flag is present)\n"\
                            "\t-o <file>\n"\
                            "\tPlace the output into <file>";

    std::cout << usage << flags << options << std::endl;
}

std::string encrypt(const char* key, std::string plaintext) {
    DES des(key);

    std::string padding = util::gen_padding(plaintext, BLOCKSIZE);
    plaintext = plaintext + padding;

    return des.encrypt(plaintext);
}

std::string decrypt(const char* key, std::string chipertext) {
    DES des(key);
    std::string output = des.decrypt(chipertext);

    return util::remove_padding(output);
}

std::string threeDES_encrypt(const char* key1, const char* key2, std::string plaintext) {
    DES des1(key1);
    DES des2(key2);

    std::string padding = util::gen_padding(plaintext, BLOCKSIZE);
    plaintext = plaintext + padding;

    std::string output = des1.encrypt(plaintext);
    output = des2.decrypt(output);
    return des1.encrypt(output);
}

std::string threeDES_decrypt(const char* key1, const char* key2, std::string plaintext) {
    DES des1(key1);
    DES des2(key2);

    std::string output = des1.decrypt(plaintext);
    output = des2.encrypt(output);
    output = des1.decrypt(output);

    return util::remove_padding(output);
}