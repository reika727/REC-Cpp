#include"syntax/tree.hpp"
#include<iostream>
#include<stdexcept>
int main(int argc,char**argv)
{
    try{
        if(argc==1)throw std::runtime_error("引数が不足しています");
        std::ifstream ifs{argv[1]};
        syntax::tree tr{std::string{std::istreambuf_iterator<char>{ifs},std::istreambuf_iterator<char>{}}};
        tr.check();
        tr.to_asm(std::string{argc==2?"tmp.s":argv[2]});
    }catch(const std::exception&e){
        std::cout<<e.what()<<std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
