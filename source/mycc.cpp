#include"tokenization/token_array.hpp"
#include"abstract_syntax_tree/tree.hpp"
#include"assembly_source/parser.hpp"
#include<iostream>
int main(int argc,char**argv)
{
    try{
	if(argc!=2)throw std::runtime_error("引数の個数が正しくありません");
	tokenization::token_array ta(argv[1]);
	abstract_syntax_tree::tree tr(ta);
	assembly_source::parser ps(tr.root(),"tmp.s");
    }catch(const std::exception&e){
	std::cout<<e.what()<<std::endl;
	return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
