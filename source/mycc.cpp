#include"tokenization/tokenizer.hpp"
#include"abstract_syntax_tree/tree.hpp"
#include"assembly_source/writer.hpp"
#include<iostream>
int main(int argc,char**argv)
{
    try{
	if(argc!=2)throw std::runtime_error("引数の個数が正しくありません");
	tokenization::tokenizer tk(argv[1]);
	abstract_syntax_tree::tree tr(tk);
	assembly_source::writer wr("tmp.s");
	wr.enter("main");
	wr.eval(tr.root());
	wr.leave();
	return EXIT_SUCCESS;
    }
    catch(const std::exception&e){
	std::cout<<e.what()<<std::endl;
	return EXIT_FAILURE;
    }
}
