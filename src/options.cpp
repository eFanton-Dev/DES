#include <string>
#include <iostream>

#include "DES.hpp"

void help_menu() {
    std::string usage = "Usage: DES.exe [flags] [options] (-f <path> | -t <text>) -k <key1>[,<key2>]\n";
    std::string flags = "Flags:\n"\
                            "\t(--encrypt | --decrypt)\n"\
                            "\t\tEncrypt/Decrypt text (default --encrypt)\n"\
                            "\t--3DES\n"\
                            "\t\tUse 3DES instead of DES (needs key2 to be specified)\n";
    std::string options = "Options:\n"\
                            "\t--help\n"\
                            "\t\tDisplay informations\n"\
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
    return des.encrypt(plaintext);
}

std::string decrypt(const char* key, std::string chipertext) {
    DES des(key);
    return des.decrypt(chipertext);
}

std::string threeDES_encrypt(const char* key1, const char* key2, std::string plaintext) {
    DES des1(key1);
    DES des2(key2);

    std::string res = des1.encrypt(plaintext);
    res = des2.decrypt(res);
    return des1.encrypt(res);
}

std::string threeDES_decrypt(const char* key1, const char* key2, std::string plaintext) {
    DES des1(key1);
    DES des2(key2);

    std::string res = des1.decrypt(plaintext);
    res = des2.encrypt(res);
    return des1.decrypt(res);
}