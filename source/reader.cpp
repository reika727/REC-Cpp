#include"tokenization/reader.hpp"
using namespace tokenization;
using TK=tokenization::TK;
reader::reader(const tokenization::tokenizer&tk):tk(tk),pos(tk.begin())
{

}
bool reader::consume(TK type)
{
    if(pos!=tk.end()){
	if(auto symp=dynamic_cast<tokenization::symbol*>(*pos);symp!=nullptr&&symp->type==type){
	    ++pos;
	    return true;
	}
    }
    return false;
}

