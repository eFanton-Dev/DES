#include <iostream>

#include "utils.hpp"
#include "common.hpp"
#include "DES.hpp"

extern Command parse(int argc, char* argv[]);
extern std::string execute(Command &c);

int main(int argc, char *argv[]) {

    if (argc <= 1)
    {
        std::cerr << "Missing arguments" << std::endl;
        return 1;
    }
    

    Command c = parse(argc - 1, argv + 1);

    std::cout << execute(c) << std::endl;

    return 0;
}