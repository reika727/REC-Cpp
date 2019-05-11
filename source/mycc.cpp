#include"lexicon/token_array.hpp"
#include"syntax/tree.hpp"
#include"code/generator.hpp"
#include<iostream>
int main(int argc,char**argv)
{
    try{
	if(argc!=2)throw std::runtime_error("引数の個数が正しくありません");
	lexicon::token_array ta(argv[1]);
	syntax::tree tr(ta);
	code::generator gen("tmp.s");

	// TODO
	    gen.write("main:");
	    gen.write("push","%rbp");
	    gen.write("mov","%rsp","%rbp");
	tr.root()->eval(gen);
	    gen.write("mov","%rbp","%rsp");
	    gen.write("pop","%rbp");
	    gen.write("ret");

    }catch(const std::exception&e){
	std::cout<<e.what()<<std::endl;
	return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
