#include"mycc.hpp"
using mycc::abstract_syntax_tree;
using node   =abstract_syntax_tree::node;
using unopr  =abstract_syntax_tree::unopr;
using biopr  =abstract_syntax_tree::biopr;
using numeric=abstract_syntax_tree::numeric;
using ident  =abstract_syntax_tree::ident;
node   ::~node  ()                                                                          {}
unopr  ::unopr  (ND type,node*const arg)                  :type(type),arg(arg)              {}
biopr  ::biopr  (node*const left,ND type,node*const right):larg(left),type(type),rarg(right){}
numeric::numeric(int value)                               :value(value)                     {}
ident  ::ident  (const std::string&name)                  :name(name)                       {}
abstract_syntax_tree::abstract_syntax_tree(const tokenizer&_tk):tk(_tk),pos_now(0)
{
    while(pos_now!=tk.size())stats.push_back(statement());
}
bool abstract_syntax_tree::consume(TK type)
{
    auto symp=dynamic_cast<tokenizer::symbol*>(tk[pos_now]);
    if(symp!=nullptr&&symp->type==type){
	++pos_now;
	return true;
    }
    return false;
}
node*abstract_syntax_tree::statement()
{
    node*ret=assign();
    if(!consume(TK::SCOLON))throw std::runtime_error("不正な区切り文字です");
    return ret;
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
    }else if(typeid(*tk[pos_now])==typeid(tokenizer::numeric)){
	auto nup=dynamic_cast<tokenizer::numeric*>(tk[pos_now++]);
	return new numeric(nup->value);
    }else if(typeid(*tk[pos_now])==typeid(tokenizer::ident)){
	auto idp=dynamic_cast<tokenizer::ident*>(tk[pos_now++]);
	return new ident(idp->name);
    }else{
	throw  std::runtime_error("構文解析ができませんでした");
    }
}
const std::vector<node*>& abstract_syntax_tree::statements()
{
    return stats;
}
