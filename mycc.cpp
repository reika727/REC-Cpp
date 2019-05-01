#include"mycc.hpp"
int main(int argc,char**argv)
{
    try{
	if(argc!=2)throw std::runtime_error("引数の個数が正しくありません");
	mycc::tokenizer tk(argv[1]);
	mycc::abstract_syntax_tree ast(tk);
	mycc::assembly_source as("tmp.s");
	as.enter("main");
	for(mycc::abstract_syntax_tree::node*const s:ast.statements()){
	    as.eval(s);
	    as.write("pop","rax");
	}
	as.leave();
	return EXIT_SUCCESS;
    }
    catch(const std::exception&e){
	std::cout<<e.what()<<std::endl;
	return EXIT_FAILURE;
    }
}
