#include <iostream>

#include "utils.hpp"
#include "common.hpp"
#include "DES.hpp"
#include "Program.hpp"

extern void parse(int argc, char* argv[]);

int main(int argc, char *argv[]) {

    if (argc <= 1)
    {
        std::cerr << "Missing arguments" << std::endl;
        std::cerr << "See 'DES.exe --help' for more informations" << std::endl;
        return 1;
    }

    Program& app = Program::getInstance();

    parse(argc - 1, argv + 1);

    std::string output = app.execute();

    if (app.outFile.empty())
        std::cout << output << std::endl;
    else
        util::write_file(app.outFile, output);
    

    return 0;
}