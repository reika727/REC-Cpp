#include"mycc.hpp"
using namespace mycc;
using node=abstract_syntax_tree::node;
node::node(node*left,ND type,node*right):type(type),lhs(left),rhs(right){}
node::node(int value):type(ND::NUMERIC),value(value),lhs(nullptr),rhs(nullptr){}
node::node(const std::string&name):type(ND::IDENT),name(name),lhs(nullptr),rhs(nullptr){}
abstract_syntax_tree::abstract_syntax_tree(const tokenizer&_tk):tk(_tk),pos_now(0)
{
    while(tk(pos_now).type!=TK::ENDT)stats.push_back(statement());
}
bool abstract_syntax_tree::consume(TK type)
{
    if(tk(pos_now).type==type){
	++pos_now;
	return true;
    }
    return false;
}
node*abstract_syntax_tree::statement()
{
    node*ret=consume(TK::RETURN)?new node(nullptr,ND::RETURN,assign()):assign();
    if(!consume(TK::SCOLON))throw std::runtime_error("不正な区切り文字です");
    return ret;
}
node*abstract_syntax_tree::assign()
{
    node*ret=equality();
    while(true){
 	     if(consume(TK::EQUAL))ret=new node(ret,ND::ASSIGN,assign());
	else if(consume(TK::PLEQ)) ret=new node(ret,ND::PLASGN,assign());
	else if(consume(TK::MIEQ)) ret=new node(ret,ND::MIASGN,assign());
	else if(consume(TK::ASEQ)) ret=new node(ret,ND::MUASGN,assign());
	else if(consume(TK::SLEQ)) ret=new node(ret,ND::DIASGN,assign());
	else if(consume(TK::PEEQ)) ret=new node(ret,ND::RMASGN,assign());
	else                       return ret;
    }
}
node*abstract_syntax_tree::equality()
{
    node*ret=relational();
    while(true){
	     if(consume(TK::EQEQ))ret=new node(ret,ND::EQUAL,relational());
	else if(consume(TK::EXEQ))ret=new node(ret,ND::NEQUAL,relational());
	else                      return ret;
    }
}
node*abstract_syntax_tree::relational()
{
    node*ret=add();
    while(true){
	     if(consume(TK::LESS)) ret=new node(ret,ND::LESS,add());
	else if(consume(TK::GREAT))ret=new node(ret,ND::GREAT,add());
	else if(consume(TK::LEEQ)) ret=new node(ret,ND::LEEQ,add());
	else if(consume(TK::GREQ)) ret=new node(ret,ND::GREQ,add());
	else                       return ret;
    }
}
node*abstract_syntax_tree::add()
{
    node*ret=mul();
    while(true){
	     if(consume(TK::PLUS)) ret=new node(ret,ND::PLUS,mul());
	else if(consume(TK::MINUS))ret=new node(ret,ND::MINUS,mul());
	else                       return ret;
    }
}
node*abstract_syntax_tree::mul()
{
    node*ret=unary();
    while(true){
	     if(consume(TK::ASTER))  ret=new node(ret,ND::MULTI,unary());
	else if(consume(TK::SLASH))  ret=new node(ret,ND::DIVIDE,unary());
	else if(consume(TK::PERCENT))ret=new node(ret,ND::REMAIN,unary());
	else                    return ret;
    }
}
node*abstract_syntax_tree::unary()
{
         if(consume(TK::PLUS)) return new node(nullptr,ND::UPLUS,term());
    else if(consume(TK::MINUS))return new node(nullptr,ND::UMINUS,term());
    else                       return term();
}
node*abstract_syntax_tree::term()
{
    if(consume(TK::OPARENT)){
	node*ret=equality();
	if(!consume(TK::CPARENT))throw std::runtime_error("括弧の対応が正しくありません");
	return ret;
    }else if(tk(pos_now).type==TK::NUMERIC)return new node(tk(pos_now++).value);
     else if(tk(pos_now).type==TK::IDENT)  return new node(tk(pos_now++).name);
     else                                  throw std::runtime_error("構文解析ができませんでした");
}
const std::vector<node*>& abstract_syntax_tree::statements()
{
    return stats;
}
