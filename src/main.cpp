#include"syntax/tree.hpp"
#include"semantics/analyzer.hpp"
#include"code/cgmanager.hpp"
#include<iostream>
#include<stdexcept>
int main(int argc,char**argv)
{
    try{
        if(argc==1)throw std::runtime_error("引数が不足しています");
        std::ifstream ifs{argv[1]};
        syntax::tree tr{std::string{std::istreambuf_iterator<char>{ifs},std::istreambuf_iterator<char>{}}};
        semantics::analyzer analy;
        tr.get_root().check(analy);
        code::cgmanager cm{std::string{argc==2?"tmp.s":argv[2]}};
        tr.get_root().to_asm(cm);
    }catch(const std::exception&e){
        std::cout<<e.what()<<std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
