#include"syntax/tree.hpp"
#include<iostream>
#include<string>
#include<stdexcept>
int main(int argc,char**argv)
{
    try{
        if(argc==1)throw std::runtime_error("引数が不足しています");
        else if(argc>3)throw std::runtime_error("引数が多すぎます");
        std::ifstream ifs{argv[1]};
        std::string source_code{std::istreambuf_iterator<char>{ifs},std::istreambuf_iterator<char>{}};
        syntax::tree tr{source_code};
        tr.check();
        tr.to_asm(argc==2?"tmp.s":argv[2]);
    }catch(const std::exception&e){
        std::cout<<e.what()<<std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
