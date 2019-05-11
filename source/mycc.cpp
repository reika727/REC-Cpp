#include"lexicon/token_array.hpp"
#include"syntax/tree.hpp"
#include"semantics/analyzer.hpp"
#include"code/generator.hpp"
#include<iostream>
#include<stdexcept>
int main(int argc,char**argv)
{
    try{
	if(argc==1){
	    throw std::runtime_error("引数が不足しています");
	}else{
	    lexicon::token_array ta(argv[1]);
	    syntax::tree tr(ta);
	    //semantics::analyzer(tr);
	    code::generator gen(argc==2?"tmp.s":argv[2]);
	    // TODO
		gen.write("main:");
		gen.write("push","%rbp");
		gen.write("mov","%rsp","%rbp");
	    tr.root()->eval(gen);
		gen.write("mov","%rbp","%rsp");
		gen.write("pop","%rbp");
	    gen.write("ret");
	}
    }catch(const std::exception&e){
	std::cout<<e.what()<<std::endl;
	return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
