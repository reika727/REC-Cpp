#include"mycc.hpp"
using namespace mycc;
using node=abstract_syntax_tree::node;
node::node(node*left,int type,node*right):type(type),lhs(left),rhs(right){}
node::node(int value):type(NUMERIC),value(value),lhs(nullptr),rhs(nullptr){}
node::node(const std::string&name):type(IDENT),name(name),lhs(nullptr),rhs(nullptr){}
bool abstract_syntax_tree::consume(int type)
{
    if(tk(pos_now).type==type){
	++pos_now;
	return true;
    }
    return false;
}
node*abstract_syntax_tree::statement()
{
    node*ret=consume(RETURN)?new node(nullptr,RETURN,assign()):assign();
    if(!consume(SEPARATE))throw std::runtime_error("不正な区切り文字です");
    return ret;
}
node*abstract_syntax_tree::assign()
{
    node*ret=equality();
    while(true){
	if(consume(ASSIGN))ret=new node(ret,ASSIGN,assign());
	else               return ret;
    }
}
node*abstract_syntax_tree::equality()
{
    node*ret=relational();
    while(true){
	     if(consume(EQUAL)) ret=new node(ret,EQUAL,relational());
	else if(consume(NEQUAL))ret=new node(ret,NEQUAL,relational());
	else                    return ret;
    }
}
node*abstract_syntax_tree::relational()
{
    node*ret=add();
    while(true){
	     if(consume(LESS)) ret=new node(ret,LESS,add());
	else if(consume(GREAT))ret=new node(ret,GREAT,add());
	else if(consume(LEEQ)) ret=new node(ret,LEEQ,add());
	else if(consume(GREQ)) ret=new node(ret,GREQ,add());
	else                   return ret;
    }
}
node*abstract_syntax_tree::add()
{
    node*ret=mul();
    while(true){
	     if(consume(PLUS)) ret=new node(ret,PLUS,mul());
	else if(consume(MINUS))ret=new node(ret,MINUS,mul());
	else                   return ret;
    }
}
node*abstract_syntax_tree::mul()
{
    node*ret=unary();
    while(true){
	     if(consume(MULTI)) ret=new node(ret,MULTI,unary());
	else if(consume(DIVIDE))ret=new node(ret,DIVIDE,unary());
	else                    return ret;
    }
}
node*abstract_syntax_tree::unary()
{
         if(consume(PLUS)) return term();
    else if(consume(MINUS))return new node(new node(0),MINUS,term());
    else                   return term();
}
node*abstract_syntax_tree::term()
{
    if(consume(OPARENT)){
	node*ret=equality();
	if(!consume(CPARENT))throw std::runtime_error("括弧の対応が正しくありません");
	return ret;
    }else if(tk(pos_now).type==NUMERIC)return new node(tk(pos_now++).value);
     else if(tk(pos_now).type==IDENT)  return new node(tk(pos_now++).name);
     else                              throw std::runtime_error("構文解析ができませんでした");
}
abstract_syntax_tree::abstract_syntax_tree(tokenizer&_tk):tk(_tk),pos_now(0)
{
    while(tk(pos_now).type!=ENDT)stats.push_back(statement());
}
const std::vector<node*>& abstract_syntax_tree::statements()
{
    return stats;
}
