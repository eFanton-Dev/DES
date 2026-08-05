#include "Program.hpp"

void Program::help_menu() {
    std::string usage = "Usage: .\\DES [flags] [options] (-f <path> | -t <text>) -k <key1>[,<key2>]\n";
    std::string flags = "Flags:\n"\
                            "\t(--encrypt | --decrypt)\n"\
                            "\t\tEncrypt/Decrypt text (default --encrypt)\n"\
                            "\t--3DES\n"\
                            "\t\tUse 3DES instead of DES (needs key2 to be specified)\n"
                            "\t--help\n"\
                            "\t\tDisplay informations\n";
    std::string options = "Options:\n"\
                            "\t-f <file>\n"\
                            "\t\tGet plaintext from <file>\n"\
                            "\t-t <text>\n"\
                            "\t\tSpecify plaintext\n"\
                            "\t-k <key1>[,<key2>]\n"\
                            "\t\tKeys to encrypt/decrypt the text (key2 must be specified if --3DES flag is present)\n"\
                            "\t-o <file>\n"\
                            "\tPlace the output into <file>\n"\
                            "\t-m <mode>\n"\
                            "\t\tMode of operation for the encryption/decryption: ECB (default), CBC, CTR\n"\
                            "\t--iv <iv>\n"\
                            "\t\tInitialization vector for CBC mode of operation\n"\
                            "\t--ctr <ctr_start>\n"\
                            "\t\tDefine the start value for the counter in CTR mode of operation\n";

    std::cout << usage << flags << options << std::endl;
}

std::string Program::encrypt() {
    des1 = new DES(keys[0].c_str());

    std::string plaintext = text;
    std::string padding = util::gen_padding(plaintext, BLOCKSIZE);
    plaintext = plaintext + padding;

    return des1->encrypt(plaintext, param, mode);
}

std::string Program::decrypt() {
    des1 = new DES(keys[0].c_str());

    std::string output = des1->decrypt(text, param, mode);

    return util::remove_padding(output);
}

std::string Program::threeDES_encrypt() {
    des1 = new DES(keys[0].c_str());
    des2 = new DES(keys[1].c_str());

    std::string plaintext = text;
    std::string padding = util::gen_padding(plaintext, BLOCKSIZE);
    plaintext = plaintext + padding;

    std::string output = des1->encrypt(plaintext, param, mode);
    output = des2->decrypt(output, param, mode);
    return des1->encrypt(output, param, mode);
}

std::string Program::threeDES_decrypt() {
    des1 = new DES(keys[0].c_str());
    des2 = new DES(keys[1].c_str());

    std::string output = des1->decrypt(text, param, mode);
    output = des2->encrypt(output, param, mode);
    output = des1->decrypt(output, param, mode);

    return util::remove_padding(output);
}

void Program::validate_command() {
    if (keys[0].empty()) {
        std::cerr << "\tMissing key" << std::endl;
        error = true;
    }
    else if (keys[0].length() != 8)
    {
        std::cerr << "\tInvalid size for first key(" << keys[0].length() << ")" << std::endl;
        error = true;
    }
    
    if (threeDES && keys[1].empty())
    {
        std::cerr << "\t3DES needs 2 keys" << std::endl;
        error = true;
    }
    else if (threeDES && keys[1].length() != 8)
    {
        std::cerr << "\tInvalid size for second key (" << keys[1].length() << ")" << std::endl;
        error = true;
    }


    if (text.empty() && inFile.empty())
    {
        std::cerr << "\tMissing text or file" << std::endl;
        error = true;
    }

    if (!text.empty() && !inFile.empty())
    {
        std::cerr << "\tBoth a text and a file had been provided" << std::endl;
        error = true;
    }

    /*
    if (iv.length() != 8)
    {
        std::cerr << "\tInvalid size for iv(" << iv.length() << ")" << std::endl;
        error = true;
    }
    */
    

    if (!error)
        std::cerr << "\t-" << std::endl;
    
    std::cerr << std::endl;
}

Program& Program::getInstance() {
    static Program program;
    return program;
}

std::string Program::execute() {
    validate_command();

    if (error)
    {
        std::cerr << "See 'DES.exe --help' for more informations" << std::endl;
        return "";
    }

    if (help) {
        help_menu();
        return "";
    }

    if (!inFile.empty())
    {
        text = util::read_file(inFile);

        if (encryptFlag)
        {
            if (threeDES)
                return threeDES_encrypt();
            
            return encrypt();
        }

        if (threeDES)
            return threeDES_decrypt();
        
        return decrypt();
        
    }
    
    if (!text.empty())
    {
        if (encryptFlag)
        {
            if (threeDES)
                return threeDES_encrypt();
            
            return encrypt();
        }

        if (threeDES)
            return threeDES_decrypt();
        
        return decrypt();
        
    }

    return "";
}