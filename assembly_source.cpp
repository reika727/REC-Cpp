#include"mycc.hpp"
using namespace mycc;
assembly_source::assembly_source()
{
    indent=0;
    std::cout<<".global main"<<std::endl;
}
void assembly_source::write(const std::string&str)
{
    std::cout<<std::string(indent,' ')<<str<<std::endl;
}
void assembly_source::write(const std::string&inst,const std::string&reg1,const std::string&reg2)
{
    write(inst+' '+p(reg1)+", "+p(reg2));
}
void assembly_source::write(const std::string&inst,int arg,const std::string&reg)
{
    write(inst+" $"+std::to_string(arg)+", "+p(reg));
}
void assembly_source::write(const std::string&inst,const std::string&reg)
{
    write(inst+" "+p(reg));
}
void assembly_source::write(const std::string&inst,int arg)
{
    write(inst+" $"+std::to_string(arg));
} 
std::string assembly_source::address(int dis,const std::string&base,const std::string&ofs,int scl)
{
    std::stringstream ss;
    if(dis!=0)ss<<dis;
    ss<<'(';
    if(base!="")ss<<'%'<<base;
    if(ofs!="")ss<<",%"<<ofs;
    if(scl!=1)ss<<','<<scl;
    ss<<')';
    return ss.str();
}
std::string assembly_source::address(int dis,const std::string&base,int scl)
{
    return address(dis,base,"",scl);
}
std::string assembly_source::address(const std::string&base,const std::string&ofs,int scl)
{
    return address(0,base,ofs,scl);
}
std::string assembly_source::address(const std::string&base,int scl)
{
    return address(0,base,"",scl);
}
std::string assembly_source::p(const std::string&str)
{
    return (str[0]=='('?"":"%")+str;
}
void assembly_source::generate_lval(const abstract_syntax_tree::node*node)
{
    if(node->type!=ND_IDENT){
	throw std::runtime_error("右辺値への代入はできません");
    }else{
	int offset=('z'-node->name+1)*8;
	write("mov","rbp","rax");
	write("sub",offset,"rax");
	write("push","rax");
    }
}
void assembly_source::generate(const abstract_syntax_tree::node*node)
{
    if(node->type==ND_NUM){
	write("push",node->value);
	return;
    }else if(node->type==ND_RETURN){
	generate(node->rhs);
	write("pop","rax");
	write("mov","rbp","rsp");
	write("pop","rbp");
	write("retq");
    }else if(node->type==ND_IDENT){
	generate_lval(node);
	write("pop","rax");
	write("mov",address("rax"),"rax");
	write("push","rax");
	return;
    }else if(node->type=='='){
	generate_lval(node->lhs);
	generate(node->rhs);
	write("pop","rdi");
	write("pop","rax");
	write("mov","rdi",address("rax"));
	write("push","rdi");
	return;
    }else{
	generate(node->lhs);
	generate(node->rhs);
	write("pop","rdi");
	write("pop","rax");
	switch(node->type){
	    case '+':
		write("add","rdi","rax");
		break;
	    case '-':
		write("sub","rdi","rax");
		break;
	    case '*':
		write("mul","rdi");
		break;
	    case '/':
		write("mov",0,"rdx");
		write("div","rdi");
		break;
	    case TK_EQ:
		write("cmp","rdi","rax");
		write("sete","al");
		write("movzb","al","rax");
		break;
	    case TK_NE:
		write("cmp","rdi","rax");
		write("setne","al");
		write("movzb","al","rax");
		break;
	    case '<':
		write("cmp","rdi","rax");
		write("setl","al");
		write("movzb","al","rax");
		break;
	    case '>':
		write("cmp","rdi","rax");
		write("setg","al");
		write("movzb","al","rax");
		break;
	    case TK_LE:
		write("cmp","rdi","rax");
		write("setle","al");
		write("movzb","al","rax");
		break;
	    case TK_GE:
		write("cmp","rdi","rax");
		write("setge","al");
		write("movzb","al","rax");
		break;
	}
	write("push","rax");
    }
}
void assembly_source::enter(const std::string&func)
{
    write(func+':');
    ++indent;
    write("push","rbp");
    write("mov","rsp","rbp");
}
void assembly_source::leave()
{
    write("mov","rbp","rsp");
    write("pop","rbp");
    write("retq");
    --indent;
}
