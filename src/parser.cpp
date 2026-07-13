#include <iostream>
#include <string>
#include <cstring>

#include "utils.hpp"
#include "common.hpp"


Command command;

Command parse(int argc, char* argv[]);
bool parse_flags(char* flag);
bool parse_options(char* option, char* value);


Command parse(int argc, char* argv[]) {
    if (argc < 1) {
        command.error = true;
        return command;
    }
    
    std::cerr << "Errors:" << std::endl;

    bool res;
    for (size_t i = 0; i < argc;)
    {
        res = parse_flags(argv[i]);
        if (res) {
            i++;
            continue;
        }

        if (i < argc - 1) {
            res = parse_options(argv[i], argv[i + 1]);
            if (res) {
                i += 2;
                continue;
            }
        }
        
        std::cerr << "\tInvalid argument: '" << argv[i] << "'" << std::endl;
        command.error = true;
        i++;
    }  

    return command;
}

bool parse_flags(char* flag) {
    if (std::strcmp(flag, "--encrypt") == 0) {
        command.encrypt = true;
        return 1;
    }
    if (std::strcmp(flag, "--decrypt") == 0) {
        command.encrypt = false;
        return 1;
    }
    if (std::strcmp(flag, "--3DES") == 0) {
        command.threeDES = true;
        return 1;
    }

    return 0;
}

bool parse_options(char* option, char* value) {
    if (std::strcmp(option, "--help") == 0) {
        command.help = true;
        return 1;
    }
    if (std::strcmp(option, "-t") == 0) {
        command.plaintext = value;
        return 1;
    }
    if (std::strcmp(option, "-f") == 0) {
        command.inPath = value;
        return 1;
    }
    if (std::strcmp(option, "-o") == 0) {
        command.outPath = value;
        return 1;
    }
    if (std::strcmp(option, "-k") == 0) {
        std::string v = value;      

        size_t pos = v.find(',');
        
        command.keys[0] = v.substr(0, pos);
        command.keys[1] = pos != std::string::npos ? v.substr(pos + 1) : "";
        return 1;
    }

    return 0;
}