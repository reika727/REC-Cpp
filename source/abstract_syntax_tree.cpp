#include"parsing/abstract_syntax_tree.hpp"
#include<stdexcept>
using namespace parsing;
using TK=tokenization::TK;
bool abstract_syntax_tree::consume(TK type)
{
    if(pos_now<tk.size()){
	if(auto symp=dynamic_cast<tokenization::symbol*>(tk[pos_now]);symp!=nullptr&&symp->type==type){
	    ++pos_now;
	    return true;
	}
    }
    return false;
}
statement*abstract_syntax_tree::stat()
{
    if(consume(TK::IF)){
	if(!consume(TK::OPARENT))throw std::runtime_error("ifの後ろに括弧がありません");
	single*cond=new single(assign());
	if(!consume(TK::CPARENT))throw std::runtime_error("ifの後ろに括弧がありません");
	statement*st=stat();
	return new _if_(cond,st);
    }else if(consume(TK::ELSE)){
	statement*st=stat();
	return new _else_(st);
    }else if(consume(TK::WHILE)){
	if(!consume(TK::OPARENT))throw std::runtime_error("whileの後ろに括弧がありません");
	single*cond=new single(assign());
	if(!consume(TK::CPARENT))throw std::runtime_error("whileの後ろに括弧がありません");
	statement*st=stat();
	return new _while_(cond,st);
    }else if(consume(TK::FOR)){
	if(!consume(TK::OPARENT))throw std::runtime_error("forの後ろに括弧がありません");
	single*init=emptiable_single();
	if(!consume(TK::SCOLON))throw std::runtime_error("不正な区切り文字です");
	single*cond=emptiable_single();
	if(!consume(TK::SCOLON))throw std::runtime_error("不正な区切り文字です");
	single*reinit=emptiable_single();
	if(!consume(TK::CPARENT))throw std::runtime_error("forの後ろに括弧がありません");
	statement*st=stat();
	return new _for_(init,cond,reinit,st);
    }else if(consume(TK::OBRACE)){
	compound*ret=new compound();
	while(!consume(TK::CBRACE))ret->stats.push_back(stat());
	return ret;
    }else{
	single*ret=emptiable_single();
	if(!consume(TK::SCOLON))throw std::runtime_error("不正な区切り文字です");
	return ret;
    }
}
single*abstract_syntax_tree::emptiable_single()
{
    if(auto syp=dynamic_cast<tokenization::symbol*>(tk[pos_now]);syp!=nullptr&&(syp->type==TK::SCOLON||syp->type==TK::CPARENT)){
	return new single(nullptr);
    }else{
	return new single(assign());
    }
}
node*abstract_syntax_tree::assign() // =, +=, -=, *=, /= right to left
{
    node*ret=equality();
    while(true){
 	     if(consume(TK::EQUAL))ret=new biopr(ret,ND::ASSIGN,assign());
	else if(consume(TK::PLEQ)) ret=new biopr(ret,ND::PLASGN,assign());
	else if(consume(TK::MIEQ)) ret=new biopr(ret,ND::MIASGN,assign());
	else if(consume(TK::ASEQ)) ret=new biopr(ret,ND::MUASGN,assign());
	else if(consume(TK::SLEQ)) ret=new biopr(ret,ND::DIASGN,assign());
	else if(consume(TK::PEEQ)) ret=new biopr(ret,ND::RMASGN,assign());
	else                       return ret;
    }
}
node*abstract_syntax_tree::equality() // ==, != left to right
{
    node*ret=relational();
    while(true){
	     if(consume(TK::EQEQ))ret=new biopr(ret,ND::EQUAL,relational());
	else if(consume(TK::EXEQ))ret=new biopr(ret,ND::NEQUAL,relational());
	else                      return ret;
    }
}
node*abstract_syntax_tree::relational() // <, >, <=, >= left to right
{
    node*ret=add();
    while(true){
	     if(consume(TK::LESS)) ret=new biopr(ret,ND::LESS,add());
	else if(consume(TK::GREAT))ret=new biopr(ret,ND::GREAT,add());
	else if(consume(TK::LEEQ)) ret=new biopr(ret,ND::LEEQ,add());
	else if(consume(TK::GREQ)) ret=new biopr(ret,ND::GREQ,add());
	else                       return ret;
    }
}
node*abstract_syntax_tree::add() // +, - left to right
{
    node*ret=mul();
    while(true){
	     if(consume(TK::PLUS)) ret=new biopr(ret,ND::PLUS,mul());
	else if(consume(TK::MINUS))ret=new biopr(ret,ND::MINUS,mul());
	else                       return ret;
    }
}
node*abstract_syntax_tree::mul() // *, /, % left to right
{
    node*ret=unary();
    while(true){
	     if(consume(TK::ASTER))  ret=new biopr(ret,ND::MULTI,unary());
	else if(consume(TK::SLASH))  ret=new biopr(ret,ND::DIVIDE,unary());
	else if(consume(TK::PERCENT))ret=new biopr(ret,ND::REMAIN,unary());
	else                         return ret;
    }
}
node*abstract_syntax_tree::unary() // +, -, ++, -- right to left
{
         if(consume(TK::PLUS)) return new unopr(ND::UPLUS,unary());
    else if(consume(TK::MINUS))return new unopr(ND::UMINUS,unary());
    else if(consume(TK::PLPL)) return new unopr(ND::PREINC,unary());
    else if(consume(TK::MIMI)) return new unopr(ND::PREDEC,unary());
    else                       return term();
}
node*abstract_syntax_tree::term()
{
    if(consume(TK::OPARENT)){
	node*ret=equality();
	if(!consume(TK::CPARENT))throw std::runtime_error("括弧の対応が正しくありません");
	return ret;
    }else if(auto nup=dynamic_cast<tokenization::numeric*>(tk[pos_now]);nup!=nullptr){
	++pos_now;
	return new numeric(nup->value);
    }else if(auto idp=dynamic_cast<tokenization::ident*>(tk[pos_now]);idp!=nullptr){
	++pos_now;
	if(consume(TK::OPARENT)){
	    fcall*ret=new fcall(idp->name);
	    while(!consume(TK::CPARENT)){
		if(consume(TK::COMMA))ret->args.push_back(equality());
		else                  ret->args.push_back(equality());
	    }
	    return ret;
	}else{
	    return new ident(idp->name);
	}
    }else{
	throw std::runtime_error("構文解析ができませんでした");
    }
}
abstract_syntax_tree::abstract_syntax_tree(const tokenization::tokenizer&_tk):prog(new compound()),tk(_tk),pos_now(0)
{
    while(pos_now!=tk.size()){
	prog->stats.push_back(stat());
    }
}
abstract_syntax_tree::~abstract_syntax_tree()
{
    delete prog;
}
const std::vector<statement*>&abstract_syntax_tree::statements()
{
    return prog->stats;
}
