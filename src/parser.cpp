#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

#include "utils.hpp"
#include "common.hpp"
#include "Program.hpp"


void parse(int argc, char* argv[]);
bool parse_flags(const char* flag);
bool parse_options(const char* option, const char* value);

Program& app = Program::getInstance();


void parse(int argc, char* argv[]) {
    if (argc < 1) {
        app.error = true;
        return;
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
        app.error = true;
        i++;
    }  

    return;
}

bool parse_flags(const char* flag) {
    if (std::strcmp(flag, "--help") == 0) {
        app.help = true;
        return 1;
    }
    if (std::strcmp(flag, "--encrypt") == 0) {
        app.encryptFlag = true;
        return 1;
    }
    if (std::strcmp(flag, "--decrypt") == 0) {
        app.encryptFlag = false;
        return 1;
    }
    if (std::strcmp(flag, "--3DES") == 0) {
        app.threeDES = true;
        return 1;
    }

    return 0;
}

bool parse_options(const char* option, const char* value) {
    if (std::strcmp(option, "-t") == 0) {
        app.text = value;
        return 1;
    }
    if (std::strcmp(option, "-f") == 0) {
        app.inFile = value;
        return 1;
    }
    if (std::strcmp(option, "-o") == 0) {
        app.outFile = value;
        return 1;
    }
    if (std::strcmp(option, "-k") == 0) {
        std::string v = value;      

        size_t pos = v.find(',');
        
        app.keys[0] = v.substr(0, pos);
        app.keys[1] = pos != std::string::npos ? v.substr(pos + 1) : "";
        return 1;
    }
    if (std::strcmp(option, "-m") == 0) {
        auto iter = ENUMSTRING.find(value);
        if (iter == ENUMSTRING.end()) {
            std::cerr << "\tInvalid mode" << std::endl;
            app.error = true;
            return 1;
        }
        

        app.mode = ENUMSTRING.at(value);
        return 1;
    }
    if (std::strcmp(option, "--iv") == 0) {
        app.param = value;
        return 1;
    }
    if (std::strcmp(option, "--ctr") == 0) {
        unsigned long val = std::strtoul(value, NULL, 10);
        app.param = val;
        return 1;
    }

    return 0;
}