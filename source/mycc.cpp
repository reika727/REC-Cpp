#include"lexicon/token_array.hpp"
#include"syntax/tree.hpp"
#include"semantics/analyzer.hpp"
#include"code/variable_manager.hpp"
#include"code/writer.hpp"
#include<iostream>
#include<stdexcept>
int main(int argc,char**argv)
{
    try{
	if(argc==1){
	    throw std::runtime_error("引数が不足しています");
	}else{
	    std::ifstream ifs(argv[1]);
	    std::string src((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
	    std::string dest(argc==2?"tmp.s":argv[2]);    
	    lexicon::token_array ta(src);
	    syntax::tree tr(ta);
	    for(auto f:tr.get_root()){
		semantics::analyzer analy;
		f->check(analy);
	    }
	    code::writer wr(dest);
	    for(auto f:tr.get_root()){
		code::variable_manager vm;
		f->to_asm(vm,wr);
	    }
	}
    }catch(const std::exception&e){
	std::cout<<e.what()<<std::endl;
	return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
