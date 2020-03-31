#include"lexicon/token_array.hpp"
#include"syntax/node.hpp"
#include"code/generator.hpp"
#include<fstream>
#include<iostream>
#include<stdexcept>
#include<string>
int main(int argc,char**argv)
{
    try{
        if(argc==1)
            throw std::runtime_error("引数が不足しています");
        else if(argc>3)
            throw std::runtime_error("引数が多すぎます");
        std::ifstream ifs(argv[1]);
        lexicon::token_array ta(std::string((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>()));
        code::generator gen;
        while(!ta.is_all_read())syntax::function_difinition::get(ta)->to_asm(gen);
        std::ofstream(argc==2?"tmp.s":argv[2])<<gen.get_asm();
    }catch(const std::exception&e){
        std::cerr<<"\033[1;31m"<<e.what()<<"\033[m"<<std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
