#include"lexicon/token_array.hpp"
#include"syntax/tree.hpp"
#include<iostream>
int main(int argc,char**argv)
{
    try{
	if(argc!=2)throw std::runtime_error("引数の個数が正しくありません");
	lexicon::token_array ta(argv[1]);
	syntax::tree tr(ta);
    }catch(const std::exception&e){
	std::cout<<e.what()<<std::endl;
	return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
