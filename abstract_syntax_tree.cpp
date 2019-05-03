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
using _else_   =abstract_syntax_tree::_else_;
node     ::~node     ()                                                                          {}
numeric  ::numeric   (int value)                               :value(value)                     {}
numeric  ::~numeric  ()                                                                          {}
ident    ::ident     (const std::string&name)                  :name(name)                       {}
ident    ::~ident    ()                                                                          {}
unopr    ::unopr     (ND type,node*const arg)                  :type(type),arg(arg)              {}
unopr    ::~unopr    ()                                                                          {delete arg;}
biopr    ::biopr     (node*const left,ND type,node*const right):larg(left),type(type),rarg(right){}
biopr    ::~biopr    ()                                                                          {delete larg;delete rarg;}
statement::~statement()                                                                          {}
single   ::single    (node*const stat)                         :stat(stat)                       {}
single   ::~single   ()                                                                          {delete stat;}
compound ::compound  ()                                                                          {stats.push_back(nullptr);}
compound ::~compound ()                                                                          {for(auto s:stats)delete s;}
_if_     ::_if_      (single*const cond,statement*const st)    :cond(cond),st(st)                {}
_if_     ::~_if_     ()                                                                          {delete cond;delete st;}
_else_   ::_else_    (statement*const st)                      :st(st)                           {}
_else_   ::~_else_   ()                                                                          {delete st;}
void compound::push_back(statement*const st)
{
    stats.back()=st;
    stats.push_back(nullptr);
}
abstract_syntax_tree::abstract_syntax_tree(const tokenizer&_tk):prog(new compound()),tk(_tk),pos_now(0)
{
    while(pos_now!=tk.size()){
	prog->push_back(stat());
    }
}
abstract_syntax_tree::~abstract_syntax_tree()
{
    delete prog;
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
    if(consume(TK::IF)){
	if(!consume(TK::OPARENT))throw std::runtime_error("ifの後ろに括弧がありません");
	single*cond=new single(assign());
	if(!consume(TK::CPARENT))throw std::runtime_error("ifの後ろに括弧がありません");
	statement*st=stat();
	ret=new _if_(cond,st);
    }else if(consume(TK::ELSE)){
	statement*st=stat();
	ret=new _else_(st);
    }else if(consume(TK::OBRACE)){
	ret=new compound();
	auto cop=dynamic_cast<compound*>(ret);
	while(!consume(TK::CBRACE))cop->push_back(stat());
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
const std::vector<statement*>&abstract_syntax_tree::statements()
{
    return prog->stats;
}
