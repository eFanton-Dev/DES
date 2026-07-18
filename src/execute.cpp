#include <string>

#include "utils.hpp"
#include "common.hpp"

extern void help_menu();
extern std::string encrypt(const char* key, std::string plaintext);
extern std::string decrypt(const char* key, std::string chipertext);
extern std::string threeDES_encrypt(const char* key1, const char* key2, std::string plaintext);
extern std::string threeDES_decrypt(const char* key1, const char* key2, std::string plaintext);

void validate_command(Command &c);

std::string execute(Command &c) {
    validate_command(c);

    if (c.error)
    {
        std::cerr << "See 'DES.exe --help' for more informations" << std::endl;
        return "";
    }

    if (c.help) {
        help_menu();
        return "";
    }

    if (!c.inPath.empty())
    {
        std::string content = util::read_file(c.inPath);

        if (c.encrypt)
        {
            if (c.threeDES)
                return threeDES_encrypt(c.keys[0].c_str(), c.keys[1].c_str(), content);
            
            return encrypt(c.keys[0].c_str(), content);
        }

        if (c.threeDES)
            return threeDES_decrypt(c.keys[0].c_str(), c.keys[1].c_str(), content);
        
        std::cout << content << std::endl;
        return decrypt(c.keys[0].c_str(), content);
        
    }
    
    if (!c.plaintext.empty())
    {
        if (c.encrypt)
        {
            if (c.threeDES)
                return threeDES_encrypt(c.keys[0].c_str(), c.keys[1].c_str(), c.plaintext);
            
            return encrypt(c.keys[0].c_str(), c.plaintext);
        }

        if (c.threeDES)
            return threeDES_decrypt(c.keys[0].c_str(), c.keys[1].c_str(), c.plaintext);
        
        return decrypt(c.keys[0].c_str(), c.plaintext);
        
    }

    return "";
}

void validate_command(Command &c) {

    if (c.keys[0].empty()) {
        std::cerr << "\tMissing key" << std::endl;
        c.error = true;
    }
    else if (c.keys[0].length() != 8)
    {
        std::cerr << "\tInvalid size for first key(" << c.keys[0].length() << ")" << std::endl;
        c.error = true;
    }
    
    if (c.threeDES && c.keys[1].empty())
    {
        std::cerr << "\t3DES needs 2 keys" << std::endl;
        c.error = true;
    }
    else if (c.threeDES && c.keys[1].length() != 8)
    {
        std::cerr << "\tInvalid size for second key (" << c.keys[1].length() << ")" << std::endl;
        c.error = true;
    }


    if (c.plaintext.empty() && c.inPath.empty())
    {
        std::cerr << "\tMissing text or file" << std::endl;
        c.error = true;
    }

    if (!c.plaintext.empty() && !c.inPath.empty())
    {
        std::cerr << "\tBoth a text and a file had been provided" << std::endl;
        c.error = true;
    }

    if (!c.error)
        std::cerr << "\t-" << std::endl;
    
    std::cerr << std::endl;
}