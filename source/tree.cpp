#include"syntax/tree.hpp"
#include<stdexcept>
using namespace syntax;
using TK=lexicon::TK;
statement*tree::stat()
{
    if(ta.consume(TK::CHAR)){
	declare*ret=new declare;
	while(true){
	    if(auto dep=ta.consume_id()){
		ret->push_back_var(std::make_pair(*dep,ta.consume(TK::EQUAL)?asgn():nullptr));
		if(ta.consume(TK::COMMA))continue;
		else if(ta.consume(TK::SCOLON))break;
		else throw std::runtime_error("不正な区切り文字です");
	    }else{
		throw std::runtime_error("無効な宣言です");
	    }
	}
	return ret;
    }else if(ta.consume(TK::IF)){
	if(!ta.consume(TK::OPARENT))throw std::runtime_error("ifの後ろに括弧がありません");
	single*cond=new single(asgn());
	if(!ta.consume(TK::CPARENT))throw std::runtime_error("ifの後ろに括弧がありません");
	statement*st1=stat();
	statement*st2=ta.consume(TK::ELSE)?stat():new single(nullptr);
	return new _if_else_(cond,st1,st2);
    }else if(ta.consume(TK::WHILE)){
	if(!ta.consume(TK::OPARENT))throw std::runtime_error("whileの後ろに括弧がありません");
	single*cond=new single(asgn());
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
	while(!ta.consume(TK::CBRACE))ret->push_back_stat(stat());
	return ret;
    }else{
	single*ret=emptiable_single();
	if(!ta.consume(TK::SCOLON))throw std::runtime_error("不正な区切り文字です");
	return ret;
    }
}
single*tree::emptiable_single()
{
    if(auto syp=dynamic_cast<lexicon::symbol*>(*ta.pos());syp&&(syp->type==TK::SCOLON||syp->type==TK::CPARENT)){
	return new single(nullptr);
    }else{
	return new single(asgn());
    }
}
node*tree::asgn() // =, +=, -=, *=, /= right to left
{
    node*ret=equality();
    while(true){
 	     if(ta.consume(TK::EQUAL))ret=new assign(ret,asgn());
	else if(ta.consume(TK::PLEQ)) ret=new plasgn(ret,asgn());
	else if(ta.consume(TK::MIEQ)) ret=new miasgn(ret,asgn());
	else if(ta.consume(TK::ASEQ)) ret=new muasgn(ret,asgn());
	else if(ta.consume(TK::SLEQ)) ret=new diasgn(ret,asgn());
	else if(ta.consume(TK::PEEQ)) ret=new rmasgn(ret,asgn());
	else                          return ret;
    }
}
node*tree::equality() // ==, != left to right
{
    node*ret=relational();
    while(true){
	     if(ta.consume(TK::EQEQ))ret=new equal(ret,relational());
	else if(ta.consume(TK::EXEQ))ret=new nequal(ret,relational());
	else                         return ret;
    }
}
node*tree::relational() // <, >, <=, >= left to right
{
    node*ret=add();
    while(true){
	     if(ta.consume(TK::LESS)) ret=new less(ret,add());
	else if(ta.consume(TK::GREAT))ret=new greater(ret,add());
	else if(ta.consume(TK::LEEQ)) ret=new leeq(ret,add());
	else if(ta.consume(TK::GREQ)) ret=new greq(ret,add());
	else                          return ret;
    }
}
node*tree::add() // +, - left to right
{
    node*ret=mul();
    while(true){
	     if(ta.consume(TK::PLUS)) ret=new plus(ret,mul());
	else if(ta.consume(TK::MINUS))ret=new minus(ret,mul());
	else                          return ret;
    }
}
node*tree::mul() // *, /, % left to right
{
    node*ret=unary();
    while(true){
	     if(ta.consume(TK::ASTER))  ret=new multi(ret,unary());
	else if(ta.consume(TK::SLASH))  ret=new divide(ret,unary());
	else if(ta.consume(TK::PERCENT))ret=new remain(ret,unary());
	else                            return ret;
    }
}
node*tree::unary() // +, -, ++, -- right to left
{
         if(ta.consume(TK::PLUS)) return new uplus(unary());
    else if(ta.consume(TK::MINUS))return new uminus(unary());
    else if(ta.consume(TK::PLPL)) return new preinc(unary());
    else if(ta.consume(TK::MIMI)) return new predec(unary());
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
	return new ident(*namep);
    }else{
	throw std::runtime_error("構文解析ができませんでした");
    }
}
tree::tree(lexicon::token_array&ta):ta(ta),rt(new compound())
{
    while(!ta.is_all_read()){
	rt->push_back_stat(stat());
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
