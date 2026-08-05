#pragma once

#include <string>
#include <variant>

#include "DES.hpp"

class Program
{
private:
    DES* des1;
    DES* des2;

    void help_menu();
    std::string encrypt();
    std::string decrypt();
    std::string threeDES_encrypt();
    std::string threeDES_decrypt();
    void validate_command();

    Program() {};

public:

    bool error = false;
    bool help = false;

    bool encryptFlag = true;
    bool threeDES = false;

    Modes mode = Modes::ECB;

    std::string text;
    std::string inFile;

    std::string outFile;

    std::string keys[2] = {"", ""};
    std::variant<std::string, unsigned long> param;
    
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;
    
    static Program& getInstance();
    std::string execute();
};
