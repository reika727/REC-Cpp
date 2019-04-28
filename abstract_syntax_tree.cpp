#include"mycc.hpp"
using namespace mycc;
using node=abstract_syntax_tree::node;
node::node(node*left,int type,node*right):type(type),lhs(left),rhs(right)
{

}
node::node(int value):type(ND_NUM),value(value)
{

}
node::node(char name):type(ND_IDENT),name(name)
{

}
bool abstract_syntax_tree::consume(int type)
{
    if(tk(pos_now).type==type){
	++pos_now;
	return true;
    }else{
	return false;
    }
}
node*abstract_syntax_tree::statement()
{
    node*ret;
    if(consume(TK_RETURN)){
	ret=new node(nullptr,ND_RETURN,assign());
    }else{
	ret=assign();
    }
    if(!consume(';')){
	throw std::runtime_error("不正な区切り文字です");
    }
    return ret;
}
node*abstract_syntax_tree::assign()
{
    node*ret=equality();
    while(true){
	if(consume('=')){
	    ret=new node(ret,'=',assign());
	}else{
	    return ret;
	}
    }
}
node*abstract_syntax_tree::equality()
{
    node*ret=relational();
    while(true){
	if(consume(TK_EQ)){
	    ret=new node(ret,TK_EQ,relational());
	}else if(consume(TK_NE)){
	    ret=new node(ret,TK_NE,relational());
	}else{
	    return ret;
	}
    }
}
node*abstract_syntax_tree::relational()
{
    node*ret=add();
    while(true){
	if(consume('<')){
	    ret=new node(ret,'<',add());
	}else if(consume('>')){
	    ret=new node(ret,'>',add());
	}else if(consume(TK_LE)){
	    ret=new node(ret,TK_LE,add());
	}else if(consume(TK_GE)){
	    ret=new node(ret,TK_GE,add());
	}else{
	    return ret;
	}
    }
}
node*abstract_syntax_tree::add()
{
    node*ret=mul();
    while(true){
	if(consume('+')){
	    ret=new node(ret,'+',mul());
	}else if(consume('-')){
	    ret=new node(ret,'-',mul());
	}else{
	    return ret;
	}
    }
}
node*abstract_syntax_tree::mul()
{
    node*ret=unary();
    while(true){
	if(consume('*')){
	    ret=new node(ret,'*',unary());
	}else if(consume('/')){
	    ret=new node(ret,'/',unary());
	}else{
	    return ret;
	}
    }
}
node*abstract_syntax_tree::unary()
{
    if(consume('+')){
	return term();
    }else if(consume('-')){
	return new node(new node(0),'-',term());
    }else{
	return term();
    }
}
node*abstract_syntax_tree::term()
{
    if(consume('(')){
	node*ret=equality();
	if(!consume(')')){
	    throw std::runtime_error("括弧の対応が正しくありません");
	}
	return ret;
    }else if(tk(pos_now).type==TK_NUM){
	return new node(tk(pos_now++).value);
    }else if(tk(pos_now).type==TK_IDENT){
	return new node(tk(pos_now++).name);
    }else{
	throw std::runtime_error("構文解析ができませんでした");
    }
}
abstract_syntax_tree::abstract_syntax_tree(tokenizer&_tk):tk(_tk),pos_now(0)
{
    while(tk(pos_now).type!=TK_EOF)stats.push_back(statement());
}
const std::vector<node*>& abstract_syntax_tree::statements()const
{
    return stats;
}
