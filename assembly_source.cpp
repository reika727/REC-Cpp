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
std::string assembly_source::derefer(int dis,const std::string&base,const std::string&ofs,int scl)
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
std::string assembly_source::derefer(int dis,const std::string&base,int scl)
{
    return derefer(dis,base,"",scl);
}
std::string assembly_source::derefer(const std::string&base,const std::string&ofs,int scl)
{
    return derefer(0,base,ofs,scl);
}
std::string assembly_source::derefer(const std::string&base,int scl)
{
    return derefer(0,base,"",scl);
}
void assembly_source::enumerate_var(abstract_syntax_tree::node*const node)
{
    if(node==nullptr)return;
    if(node->type==ND::IDENT&&!offset.count(node->name)){
	write("sub",8,"rsp");
	offset[node->name]=var_size+=8;
    }
    enumerate_var(node->lhs);
    enumerate_var(node->rhs);
}
void assembly_source::refer_var(abstract_syntax_tree::node*const node)
{
    if(node->type!=ND::IDENT){
	throw std::runtime_error("右辺値への代入はできません");
    }else{
	write("mov","rbp","rax");
	write("sub",offset[node->name],"rax");
	write("push","rax");
    }
}
void assembly_source::RDP(abstract_syntax_tree::node*const node)
{
    if(node->type==ND::NUMERIC){
	write("push",node->value);
    }else if(node->type==ND::IDENT){
	refer_var(node);
	write("pop","rax");
	write("push",derefer("rax"));
    }else if(node->type==ND::RETURN){
	RDP(node->rhs);
	write("pop","rax");
	write("mov","rbp","rsp");
	write("pop","rbp");
	write("retq");
    }else if(node->type==ND::UPLUS){
	RDP(node->rhs);
    }else if(node->type==ND::UMINUS){
	RDP(node->rhs);
	write("pop","rax");
	write("mov","rax","rdi");
	write("mov",2,"rsi");write("mul","rsi");
	write("sub","rax","rdi");
	write("push","rdi");
    }else if(ND::ASSIGN<=node->type&&node->type<=ND::RMASGN){
	refer_var(node->lhs);
	RDP(node->rhs);
	write("pop","rdi");
	write("pop","rax");
	switch(node->type){
	    case ND::ASSIGN:
		write("mov","rdi",derefer("rax"));
		break;
	    case ND::PLASGN:
		write("add","rdi",derefer("rax"));
		break;
	    case ND::MIASGN:
		write("sub","rdi",derefer("rax"));
		break;
	    case ND::MUASGN:
		write("mov","rax","rsi");
		write("mov",derefer("rax"),"rax");
		write("mul","rdi");
		write("mov","rax",derefer("rsi"));
		write("mov","rsi","rax");
		break;
	    case ND::DIASGN:
		write("mov","rax","rsi");
		write("mov",derefer("rax"),"rax");
		write("mov",0,"rdx");
		write("div","rdi");
		write("mov","rax",derefer("rsi"));
		write("mov","rsi","rax");
		break;
	    case ND::RMASGN:
		write("mov","rax","rsi");
		write("mov",derefer("rax"),"rax");
		write("mov",0,"rdx");
		write("div","rdi");
		write("mov","rdx",derefer("rsi"));
		write("mov","rsi","rax");
		break;
	}
	write("push",derefer("rax"));
    }else{
	RDP(node->lhs);
	RDP(node->rhs);
	write("pop","rdi");
	write("pop","rax");
	switch(node->type){
	    case ND::PLUS:
		write("add","rdi","rax");
		break;
	    case ND::MINUS:
		write("sub","rdi","rax");
		break;
	    case ND::MULTI:
		write("mul","rdi");
		break;
	    case ND::DIVIDE:
		write("mov",0,"rdx");
		write("div","rdi");
		break;
	    case ND::REMAIN:
		write("mov",0,"rdx");
		write("div","rdi");
		write("mov","rdx","rax");
		break;
	    case ND::EQUAL:
		write("cmp","rdi","rax");
		write("sete","al");
		write("movzb","al","rax");
		break;
	    case ND::NEQUAL:
		write("cmp","rdi","rax");
		write("setne","al");
		write("movzb","al","rax");
		break;
	    case ND::LESS:
		write("cmp","rdi","rax");
		write("setl","al");
		write("movzb","al","rax");
		break;
	    case ND::GREAT:
		write("cmp","rdi","rax");
		write("setg","al");
		write("movzb","al","rax");
		break;
	    case ND::LEEQ:
		write("cmp","rdi","rax");
		write("setle","al");
		write("movzb","al","rax");
		break;
	    case ND::GREQ:
		write("cmp","rdi","rax");
		write("setge","al");
		write("movzb","al","rax");
		break;
	}
	write("push","rax");
    }
}
void assembly_source::eval(abstract_syntax_tree::node*const node)
{
    enumerate_var(node);
    RDP(node);
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
