#include "utils.hpp"

std::string util::read_file(std::string path) {
    std::fstream source(path, std::ios::binary | std::ios::in);
    
    if (!source.is_open()) throw "Failed to open " + path;
    if (source.bad()) throw "Fatal error: badbit is set.";
    if (source.fail()) throw std::string("Error details: ") + std::strerror(errno);
    

    std::stringstream buf;
    buf << source.rdbuf();

    source.close();

    return buf.str();
}

void util::write_file(std::string path, std::string content) {
    std::fstream output(path, std::ios::binary | std::ios::out);

    if (!output.is_open()) throw "Failed to open " + path;
    if (output.bad()) throw "Fatal error: badbit is set.";
    if (output.fail()) throw std::string("Error details: ") + std::strerror(errno);

    output.write(content.c_str(), content.length());
    output.flush();

    output.close();
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

    if (length == 0)
    {
        std::cerr << "Empty message" << std::endl;
        return "";
    }

    size_t padding_size = msg[length - 1];

    if (padding_size > length)
    {
        std::cerr << "Padding to big" << std::endl;
        return "";
    }
    

    return msg.substr(0, length - padding_size);
}
