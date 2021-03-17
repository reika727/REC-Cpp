#include "common.hpp"
#include "lexer.hpp"
#include "node.hpp"
#include "writer.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
int main(int argc, char *argv[])
{
    try {
        const char *const default_name = "tmp.s";
        if (argc != 2 && argc != 3) {
            throw std::runtime_error(format("usage: rec INPUT [OUTPUT=%s]", default_name));
        }
        std::ifstream ifs(argv[1]);
        if (!ifs) {
            throw std::runtime_error(format("file %s does not exist", argv[1]));
        }
        std::string source{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
        lexicon::lexer lx(source);
        code::writer wr;
        syntax::translation_unit(lx).to_asm(wr);
        std::ofstream(argc == 2 ? default_name : argv[2]) << wr.get_asm();
    } catch (const std::exception &e) {
        std::cerr << "\033[1;31m" << e.what() << "\033[m" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
