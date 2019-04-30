#include"mycc.hpp"
using namespace mycc;
assembly_source::assembly_source(const std::string&filename):ofs(filename),indent(0)
{
    write(".global main");
}
std::string assembly_source::p(const std::string&str)
{
    return (str[0]=='('?"":"%")+str;
}
void assembly_source::write(const std::string&str)
{
    ofs<<std::string(indent,' ')<<str<<std::endl;
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
    write(inst+' '+p(reg));
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
void assembly_source::enumerate_var(const abstract_syntax_tree::node*node)
{
    if(node==nullptr)return;
    if(node->type==IDENT&&!offset.count(node->name)){
	write("sub",8,"rsp");
	offset[node->name]=var_size+=8;
    }
    enumerate_var(node->lhs);
    enumerate_var(node->rhs);
}
void assembly_source::generate_lval(const abstract_syntax_tree::node*node)
{
    if(node->type!=IDENT){
	throw std::runtime_error("右辺値への代入はできません");
    }else{
	write("mov","rbp","rax");
	write("sub",offset[node->name],"rax");
	write("push","rax");
    }
}
void assembly_source::generate_recur(const abstract_syntax_tree::node*node)
{
    if(node->type==NUMERIC){
	write("push",node->value);
	return;
    }else if(node->type==RETURN){
	generate_recur(node->rhs);
	write("pop","rax");
	write("mov","rbp","rsp");
	write("pop","rbp");
	write("retq");
    }else if(node->type==IDENT){
	generate_lval(node);
	write("pop","rax");
	write("mov",address("rax"),"rax");
	write("push","rax");
	return;
    }else if(node->type==ASSIGN){
	generate_lval(node->lhs);
	generate_recur(node->rhs);
	write("pop","rdi");
	write("pop","rax");
	write("mov","rdi",address("rax"));
	write("push","rdi");
	return;
    }else{
	generate_recur(node->lhs);
	generate_recur(node->rhs);
	write("pop","rdi");
	write("pop","rax");
	switch(node->type){
	    case PLUS:
		write("add","rdi","rax");
		break;
	    case MINUS:
		write("sub","rdi","rax");
		break;
	    case MULTI:
		write("mul","rdi");
		break;
	    case DIVIDE:
		write("mov",0,"rdx");
		write("div","rdi");
		break;
	    case EQUAL:
		write("cmp","rdi","rax");
		write("sete","al");
		write("movzb","al","rax");
		break;
	    case NEQUAL:
		write("cmp","rdi","rax");
		write("setne","al");
		write("movzb","al","rax");
		break;
	    case LESS:
		write("cmp","rdi","rax");
		write("setl","al");
		write("movzb","al","rax");
		break;
	    case GREAT:
		write("cmp","rdi","rax");
		write("setg","al");
		write("movzb","al","rax");
		break;
	    case LEEQ:
		write("cmp","rdi","rax");
		write("setle","al");
		write("movzb","al","rax");
		break;
	    case GREQ:
		write("cmp","rdi","rax");
		write("setge","al");
		write("movzb","al","rax");
		break;
	}
	write("push","rax");
    }
}
void assembly_source::generate(const abstract_syntax_tree::node*node)
{
    enumerate_var(node);
    generate_recur(node);
}
void assembly_source::enter(const std::string&func)
{
    write(func+':');
    ++indent;
    write("push","rbp");
    write("mov","rsp","rbp");
    var_size=0;
}
void assembly_source::leave()
{
    write("mov","rbp","rsp");
    write("pop","rbp");
    write("retq");
    --indent;
    offset.clear();
}
