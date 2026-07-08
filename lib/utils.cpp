#include "utils.hpp"

const char* util::read_file(std::string path) {
    std::fstream source(path, std::ios::in);
    
    if (!source.is_open()) throw "Failed to open " + path;
    if (source.bad()) throw "Fatal error: badbit is set.";
    if (source.fail()) throw std::string("Error details: ") + std::strerror(errno);
    

    std::stringstream buf;
    buf << source.rdbuf();
    std::string message = buf.str();


    return message.c_str();
}

// PKCS#5 standard
std::string util::gen_padding(const std::string &msg, size_t blocksize) {
    size_t length = msg.length();

    size_t padding_size = blocksize - (length % blocksize);
    if (padding_size == 0) padding_size = blocksize;

    std::string padding(padding_size, padding_size);

    return padding;
}

std::string util::remove_padding(const std::string &msg) {
    size_t length = msg.length();
    size_t padding_size = msg[length - 1];

    return msg.substr(0, length - padding_size);
}
