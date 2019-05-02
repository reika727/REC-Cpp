#include"mycc.hpp"
using mycc::abstract_syntax_tree;
using node     =abstract_syntax_tree::node;
using unopr    =abstract_syntax_tree::unopr;
using biopr    =abstract_syntax_tree::biopr;
using numeric  =abstract_syntax_tree::numeric;
using ident    =abstract_syntax_tree::ident;
using statement=abstract_syntax_tree::statement;
using single   =abstract_syntax_tree::single;
using compound =abstract_syntax_tree::compound;
using _if_     =abstract_syntax_tree::_if_;
node     ::~node     ()                                                                          {}
unopr    ::unopr     (ND type,node*const arg)                  :type(type),arg(arg)              {}
biopr    ::biopr     (node*const left,ND type,node*const right):larg(left),type(type),rarg(right){}
numeric  ::numeric   (int value)                               :value(value)                     {}
ident    ::ident     (const std::string&name)                  :name(name)                       {}
statement::~statement()                                                                          {}
single   ::single    (node*const stat)                         :stat(stat)                       {}
_if_     ::_if_      (node*const cond,node*const st)           :cond(cond),st(st)                {}
abstract_syntax_tree::abstract_syntax_tree(const tokenizer&_tk):prog(new compound()),tk(_tk),pos_now(0)
{
    while(pos_now!=tk.size()){
	prog->stats.push_back(stat());
    }
}
bool abstract_syntax_tree::consume(TK type)
{
    if(pos_now<tk.size()){
	if(auto symp=dynamic_cast<tokenizer::symbol*>(tk[pos_now]);symp!=nullptr&&symp->type==type){
	    ++pos_now;
	    return true;
	}
    }
    return false;
}
statement*abstract_syntax_tree::stat()
{
    statement*ret;
    if(consume(TK::OBRACE)){
	ret=new compound();
	auto cop=dynamic_cast<compound*>(ret);
	while(!consume(TK::CBRACE))cop->stats.push_back(stat());
    }else{
	ret=new single(assign());
	if(!consume(TK::SCOLON))throw std::runtime_error("不正な区切り文字です");
    }
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
    }else if(pos_now<tk.size()){
	if(auto nup=dynamic_cast<tokenizer::numeric*>(tk[pos_now]);nup!=nullptr){
	    ++pos_now;
	    return new numeric(nup->value);
	}else if(auto idp=dynamic_cast<tokenizer::ident*>(tk[pos_now]);idp!=nullptr){
	    ++pos_now;
	    return new ident(idp->name);
	}else{
	    throw std::runtime_error("構文解析ができませんでした");
	}
    }else{
	throw std::runtime_error("構文解析ができませんでした");
    }
}
compound*const abstract_syntax_tree::statements()
{
    return prog;
}
