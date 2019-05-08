#include"abstract_syntax_tree/tree.hpp"
#include<stdexcept>
using namespace abstract_syntax_tree;
using TK=tokenization::TK;
statement*tree::stat()
{
    if(ta.consume(TK::CHAR)){
	declare*ret=new declare;
	while(true){
	    if(auto dep=ta.consume_id())ret->vars.push_back(std::make_pair(*dep,nullptr));
	    else throw std::runtime_error("無効な宣言です");
	    if(ta.consume(TK::EQUAL))ret->vars.back().second=assign();
	    if(ta.consume(TK::COMMA))continue;
	    else if(ta.consume(TK::SCOLON))break;
	    else throw std::runtime_error("不正な区切り文字です");
	}
	return ret;
    }else if(ta.consume(TK::IF)){
	if(!ta.consume(TK::OPARENT))throw std::runtime_error("ifの後ろに括弧がありません");
	single*cond=new single(assign());
	if(!ta.consume(TK::CPARENT))throw std::runtime_error("ifの後ろに括弧がありません");
	statement*st=stat();
	return new _if_(cond,st);
    }else if(ta.consume(TK::ELSE)){
	statement*st=stat();
	return new _else_(st);
    }else if(ta.consume(TK::WHILE)){
	if(!ta.consume(TK::OPARENT))throw std::runtime_error("whileの後ろに括弧がありません");
	single*cond=new single(assign());
	if(!ta.consume(TK::CPARENT))throw std::runtime_error("whileの後ろに括弧がありません");
	statement*st=stat();
	return new _while_(cond,st);
    }else if(ta.consume(TK::FOR)){
	if(!ta.consume(TK::OPARENT))throw std::runtime_error("forの後ろに括弧がありません");
	single*init=emptiable_single();
	if(!ta.consume(TK::SCOLON))throw std::runtime_error("不正な区切り文字です");
	single*cond=emptiable_single();
	if(!ta.consume(TK::SCOLON))throw std::runtime_error("不正な区切り文字です");
	single*reinit=emptiable_single();
	if(!ta.consume(TK::CPARENT))throw std::runtime_error("forの後ろに括弧がありません");
	statement*st=stat();
	return new _for_(init,cond,reinit,st);
    }else if(ta.consume(TK::OBRACE)){
	compound*ret=new compound();
	while(!ta.consume(TK::CBRACE))ret->stats.push_back(stat());
	return ret;
    }else{
	single*ret=emptiable_single();
	if(!ta.consume(TK::SCOLON))throw std::runtime_error("不正な区切り文字です");
	return ret;
    }
}
single*tree::emptiable_single()
{
    if(auto syp=dynamic_cast<tokenization::symbol*>(*ta.pos());syp&&(syp->type==TK::SCOLON||syp->type==TK::CPARENT)){
	return new single(nullptr);
    }else{
	return new single(assign());
    }
}
node*tree::assign() // =, +=, -=, *=, /= right to left
{
    node*ret=equality();
    while(true){
 	     if(ta.consume(TK::EQUAL))ret=new biopr(ret,ND::ASSIGN,assign());
	else if(ta.consume(TK::PLEQ)) ret=new biopr(ret,ND::PLASGN,assign());
	else if(ta.consume(TK::MIEQ)) ret=new biopr(ret,ND::MIASGN,assign());
	else if(ta.consume(TK::ASEQ)) ret=new biopr(ret,ND::MUASGN,assign());
	else if(ta.consume(TK::SLEQ)) ret=new biopr(ret,ND::DIASGN,assign());
	else if(ta.consume(TK::PEEQ)) ret=new biopr(ret,ND::RMASGN,assign());
	else                          return ret;
    }
}
node*tree::equality() // ==, != left to right
{
    node*ret=relational();
    while(true){
	     if(ta.consume(TK::EQEQ))ret=new biopr(ret,ND::EQUAL,relational());
	else if(ta.consume(TK::EXEQ))ret=new biopr(ret,ND::NEQUAL,relational());
	else                         return ret;
    }
}
node*tree::relational() // <, >, <=, >= left to right
{
    node*ret=add();
    while(true){
	     if(ta.consume(TK::LESS)) ret=new biopr(ret,ND::LESS,add());
	else if(ta.consume(TK::GREAT))ret=new biopr(ret,ND::GREAT,add());
	else if(ta.consume(TK::LEEQ)) ret=new biopr(ret,ND::LEEQ,add());
	else if(ta.consume(TK::GREQ)) ret=new biopr(ret,ND::GREQ,add());
	else                          return ret;
    }
}
node*tree::add() // +, - left to right
{
    node*ret=mul();
    while(true){
	     if(ta.consume(TK::PLUS)) ret=new biopr(ret,ND::PLUS,mul());
	else if(ta.consume(TK::MINUS))ret=new biopr(ret,ND::MINUS,mul());
	else                          return ret;
    }
}
node*tree::mul() // *, /, % left to right
{
    node*ret=unary();
    while(true){
	     if(ta.consume(TK::ASTER))  ret=new biopr(ret,ND::MULTI,unary());
	else if(ta.consume(TK::SLASH))  ret=new biopr(ret,ND::DIVIDE,unary());
	else if(ta.consume(TK::PERCENT))ret=new biopr(ret,ND::REMAIN,unary());
	else                            return ret;
    }
}
node*tree::unary() // +, -, ++, -- right to left
{
         if(ta.consume(TK::PLUS)) return new unopr(ND::UPLUS,unary());
    else if(ta.consume(TK::MINUS))return new unopr(ND::UMINUS,unary());
    else if(ta.consume(TK::PLPL)) return new unopr(ND::PREINC,unary());
    else if(ta.consume(TK::MIMI)) return new unopr(ND::PREDEC,unary());
    else                          return term();
}
node*tree::term()
{
    if(ta.consume(TK::OPARENT)){
	node*ret=equality();
	if(!ta.consume(TK::CPARENT))throw std::runtime_error("括弧の対応が正しくありません");
	return ret;
    }else if(auto nump=ta.consume_num()){
	return new numeric(*nump);
    }else if(auto namep=ta.consume_id()){
	if(ta.consume(TK::OPARENT)){
	    fcall*ret=new fcall(*namep);
	    while(!ta.consume(TK::CPARENT)){
		if(ta.consume(TK::COMMA))ret->args.push_back(equality());
		else                     ret->args.push_back(equality());
	    }
	    return ret;
	}else{
	    return new ident(*namep);
	}
    }else{
	throw std::runtime_error("構文解析ができませんでした");
    }
}
tree::tree(tokenization::token_array&ta):ta(ta),rt(new compound())
{
    while(!ta.is_all_read()){
	rt->stats.push_back(stat());
    }
}
tree::~tree()
{
    delete rt;
}
compound*tree::root()
{
    return rt;
}
