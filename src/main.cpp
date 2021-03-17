#include "lexer.hpp"
#include "node.hpp"
#include "writer.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
int main(int argc, char **argv)
{
    try {
        if (argc == 1) {
            throw std::runtime_error("引数が不足しています");
        } else if (argc > 3) {
            throw std::runtime_error("引数が多すぎます");
        }
        std::ifstream ifs(argv[1]);
        lexicon::lexer lx(std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>()));
        code::writer wr;
        syntax::translation_unit(lx).to_asm(wr);
        std::ofstream(argc == 2 ? "tmp.s" : argv[2]) << wr.get_asm();
    } catch (const std::exception &e) {
        std::cerr << "\033[1;31m" << e.what() << "\033[m" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
