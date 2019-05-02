#include"mycc.hpp"
using mycc::assembly_source;
assembly_source::assembly_source(const std::string&filename):ofs(filename),indent(0),serial(0)
{
    write(".global main");
}
std::string assembly_source::p(const std::string&str)
{
    return (str[0]=='('||str[0]=='.'?"":"%")+str;
}
std::string assembly_source::label(const std::string&base)
{
    return base+std::to_string(serial++);
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
    if(auto bop=dynamic_cast<abstract_syntax_tree::biopr*>(node);bop!=nullptr){
	enumerate_var(bop->larg);
	enumerate_var(bop->rarg);
    }else if(auto uop=dynamic_cast<abstract_syntax_tree::unopr*>(node);uop!=nullptr){
	enumerate_var(uop->arg);
    }else if(auto idp=dynamic_cast<abstract_syntax_tree::ident*>(node);idp!=nullptr){
	if(!offset.count(idp->name)){
	    write("sub",8,"rsp");
	    offset[idp->name]=var_size+=8;
	}
    }
}
void assembly_source::refer_var(abstract_syntax_tree::node*const node)
{
    if(auto idp=dynamic_cast<abstract_syntax_tree::ident*>(node);idp!=nullptr){
	write("mov","rbp","rax");
	write("sub",offset[idp->name],"rax");
	write("push","rax");
    }else{
	throw std::runtime_error("右辺値への代入はできません");
    }
}
void assembly_source::RDP(abstract_syntax_tree::node*const node)
{
    if(auto nup=dynamic_cast<abstract_syntax_tree::numeric*>(node);nup!=nullptr){
	write("push",nup->value);
    }else if(auto idp=dynamic_cast<abstract_syntax_tree::ident*>(node);idp!=nullptr){
	refer_var(idp);
	write("pop","rax");
	write("push",derefer("rax"));
    }else if(auto uop=dynamic_cast<abstract_syntax_tree::unopr*>(node);uop!=nullptr){
	if(uop->type==ND::UPLUS){
	    RDP(uop->arg);
	}else if(uop->type==ND::UMINUS){
	    RDP(uop->arg);
	    write("pop","rax");
	    write("mov","rax","rdi");
	    write("mov",2,"rsi");write("mul","rsi");
	    write("sub","rax","rdi");
	    write("push","rdi");
	}else if(uop->type==ND::PREINC||uop->type==ND::PREDEC){
	    refer_var(uop->arg);
	    write("pop","rax");
	    switch(uop->type){
		case ND::PREINC:
		    write("add",1,derefer("rax"));
		    break;
		case ND::PREDEC:
		    write("sub",1,derefer("rax"));
		    break;
	    }
	    write("push",derefer("rax"));
	}
    }else if(auto bop=dynamic_cast<abstract_syntax_tree::biopr*>(node);bop!=nullptr){
	if(ND::ASSIGN<=bop->type&&bop->type<=ND::RMASGN){
	    refer_var(bop->larg);
	    RDP(bop->rarg);
	    write("pop","rdi");
	    write("pop","rax");
	    switch(bop->type){
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
	    RDP(bop->larg);
	    RDP(bop->rarg);
	    write("pop","rdi");
	    write("pop","rax");
	    switch(bop->type){
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
}
void assembly_source::eval(abstract_syntax_tree::statement*const st)
{
    if(auto sg=dynamic_cast<abstract_syntax_tree::single*>(st);sg!=nullptr){
	eval(sg);
    }else if(auto com=dynamic_cast<abstract_syntax_tree::compound*>(st);com!=nullptr){
	eval(com->stats);
    }
}
void assembly_source::eval(abstract_syntax_tree::single*const sg)
{
    enumerate_var(sg->stat);
    RDP(sg->stat);
    write("pop","rax");
}
void assembly_source::eval(const std::vector<abstract_syntax_tree::statement*>&sv)
{
    for(int i=0;i<sv.size();++i){
	if(auto cif=dynamic_cast<abstract_syntax_tree::_if_*>(sv[i]);cif!=nullptr){
	    if(auto cel=dynamic_cast<abstract_syntax_tree::_else_*>(sv[i+1]);cel!=nullptr){
		eval(cif->cond);
		write("cmp",0,"rax");
		std::string lab1=label(".Lelse");
		write("je",lab1);
		eval(cif->st);
		std::string lab2=label(".Lend");
		write("jmp",lab2);
		write(lab1+':');
		eval(cel->st);
		write(lab2+':');
	    }else{
		eval(cif->cond);
		write("cmp",0,"rax");
		std::string lab=label(".Lend");
		write("je",lab);
		eval(cif->st);
		write(lab+':');
	    }
	}else{
	    eval(sv[i]);
	}
    }
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
