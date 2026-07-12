#include <string>

#include "utils.hpp"
#include "common.hpp"

extern void help_menu();
extern std::string encrypt_text(const char* key, std::string plaintext);
extern std::string decrypt_text(const char* key, std::string chipertext);
extern std::string threeDES_encrypt(const char* key1, const char* key2, std::string plaintext);
extern std::string threeDES_decrypt(const char* key1, const char* key2, std::string plaintext);


std::string execute(Command c) {
    if (c.help || c.error) {
        help_menu();
        return "";
    }

    if (!c.filePath.empty())
    {
        std::string content = util::read_file(c.filePath);

        if (c.encrypt)
        {
            if (c.threeDES)
                return threeDES_encrypt(c.keys[0].c_str(), c.keys[1].c_str(), content);
            else
                return encrypt_text(c.keys[0].c_str(), content);
        }
        else
        {
            if (c.threeDES)
                return threeDES_decrypt(c.keys[0].c_str(), c.keys[1].c_str(), content);
            else
                return decrypt_text(c.keys[0].c_str(), content);
        }
    }
    
    if (!c.plaintext.empty())
    {
        if (c.encrypt)
        {
            if (c.threeDES)
                return threeDES_encrypt(c.keys[0].c_str(), c.keys[1].c_str(), c.plaintext);
            else
                return encrypt_text(c.keys[0].c_str(), c.plaintext);
        }
        else
        {
            if (c.threeDES)
                return threeDES_decrypt(c.keys[0].c_str(), c.keys[1].c_str(), c.plaintext);
            else
                return decrypt_text(c.keys[0].c_str(), c.plaintext);
        }
    }
}