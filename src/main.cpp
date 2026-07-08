#include <iostream>

#include "utils.hpp"
#include "DES.hpp"

/**
 * Usage: DES <source file> -o [output file]
 */
int main(int argc, char *argv[]) {
    if (argc < 2)
    {
        std::cout << "Usage: DES <source file> -o [output file]" << std::endl;
        return 1;
    }
    
    const char* message;
    try
    {
        message = util::read_file(argv[1]);
    }
    catch(const std::string error)
    {
        std::cerr << error << '\n';
    }

    std::cout << message << std::endl;

    DES des{0xa44598e67a54bf1c};
    std::string ctext = des.encrypt("Hello World");

    std::cout << ctext << std::endl;

    std::cout << des.decrypt(ctext) << std::endl;

    return 0;
}