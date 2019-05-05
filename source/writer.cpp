#include"assembly_source/writer.hpp"
#include<sstream>
using namespace assembly_source;
using ND=abstract_syntax_tree::ND;
std::string writer::label(const std::string&base)
{
    return '.'+base+std::to_string(serial++);
}
std::string writer::p(const std::string&str)
{
    return (str[0]=='('||str[0]=='.'?"":"%")+str;
}
void writer::write(const std::string&str)
{
    ofs<<std::string(indent,' ')<<str<<std::endl;
}
void writer::write(const std::string&inst,const std::string&reg1,const std::string&reg2)
{
    write(inst+' '+p(reg1)+", "+p(reg2));
}
void writer::write(const std::string&inst,int arg,const std::string&reg)
{
    write(inst+" $"+std::to_string(arg)+", "+p(reg));
}
void writer::write(const std::string&inst,const std::string&reg)
{
    write(inst+' '+p(reg));
}
void writer::write(const std::string&inst,int arg)
{
    write(inst+" $"+std::to_string(arg));
} 
std::string writer::derefer(int dis,const std::string&base,const std::string&ofs,int scl)
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
std::string writer::derefer(int dis,const std::string&base,int scl)
{
    return derefer(dis,base,"",scl);
}
std::string writer::derefer(const std::string&base,const std::string&ofs,int scl)
{
    return derefer(0,base,ofs,scl);
}
std::string writer::derefer(const std::string&base,int scl)
{
    return derefer(0,base,"",scl);
}
void writer::enumerate_var(abstract_syntax_tree::node*const node)
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
    }else if(auto cfp=dynamic_cast<abstract_syntax_tree::fcall*>(node);cfp!=nullptr){
	for(auto a:cfp->args)enumerate_var(a);
    }
}
void writer::refer_var(abstract_syntax_tree::node*const node)
{
    if(auto idp=dynamic_cast<abstract_syntax_tree::ident*>(node);idp!=nullptr){
	write("mov","rbp","rax");
	write("sub",offset[idp->name],"rax");
	write("push","rax");
    }else{
	throw std::runtime_error("右辺値への代入はできません");
    }
}
void writer::RDP(abstract_syntax_tree::node*const node)
{
    if(auto nup=dynamic_cast<abstract_syntax_tree::numeric*>(node);nup!=nullptr){
	write("push",nup->value);
    }else if(auto idp=dynamic_cast<abstract_syntax_tree::ident*>(node);idp!=nullptr){
	refer_var(idp);
	write("pop","rax");
	write("push",derefer("rax"));
    }else if(auto cfp=dynamic_cast<abstract_syntax_tree::fcall*>(node);cfp!=nullptr){
	for(;!cfp->args.empty();cfp->args.pop_back()){
	    RDP(cfp->args.back());
	    write("pop","rax");
	    switch(cfp->args.size()){
		case 1 :write("mov","rax","rdi");break;
		case 2 :write("mov","rax","rsi");break;
		case 3 :write("mov","rax","rdx");break;
		case 4 :write("mov","rax","rcx");break;
		case 5 :write("mov","rax","r8"); break;
		case 6 :write("mov","rax","r9"); break;
		default:write("push","rax");     break;
	    }
	}
	write("sub",(16-var_size%16)%16,"rsp");
	write("call "+cfp->name);
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
void writer::eval(abstract_syntax_tree::statement*const st)
{
    if(auto sg=dynamic_cast<abstract_syntax_tree::single*>(st);sg!=nullptr){
	eval(sg);
    }else if(auto com=dynamic_cast<abstract_syntax_tree::compound*>(st);com!=nullptr){
	eval(com->stats);
    }
}
void writer::eval(abstract_syntax_tree::single*const sg)
{
    if(!sg->is_nop()){
	enumerate_var(sg->stat);
	RDP(sg->stat);
	write("pop","rax");
    }
}
void writer::eval(const std::vector<abstract_syntax_tree::statement*>&sv)
{
    for(int i=0;i<sv.size();++i){
	if(auto cif=dynamic_cast<abstract_syntax_tree::_if_*>(sv[i]);cif!=nullptr){
	    if(auto cel=dynamic_cast<abstract_syntax_tree::_else_*>(i+1<sv.size()?sv[i+1]:nullptr);cel!=nullptr){
		std::string el=label("Lelse");
		std::string end=label("Lend");
		eval(cif->cond);
		write("cmp",0,"rax");
		write("je",el);
		eval(cif->st);
		write("jmp",end);
		write(el+':');
		eval(cel->st);
		write(end+':');
	    }else{
		std::string end=label("Lend");
		eval(cif->cond);
		write("cmp",0,"rax");
		write("je",end);
		eval(cif->st);
		write(end+':');
	    }
	}else if(auto cwh=dynamic_cast<abstract_syntax_tree::_while_*>(sv[i]);cwh!=nullptr){
	    std::string beg=label("Lbegin");
	    std::string end=label("Lend");
	    write(beg+':');
	    eval(cwh->cond);
	    write("cmp",0,"rax");
	    write("je",end);
	    eval(cwh->st);
	    write("jmp",beg);
	    write(end+':');
	}else if(auto cfo=dynamic_cast<abstract_syntax_tree::_for_*>(sv[i]);cfo!=nullptr){
	    std::string beg=label("Lbegin");
	    std::string end=label("Lend");
	    eval(cfo->init);
	    write(beg+':');
	    write("mov",1,"rax");
	    eval(cfo->cond);
	    write("cmp",0,"rax");
	    write("je",end);
	    eval(cfo->st);
	    eval(cfo->reinit);
	    write("jmp",beg);
	    write(end+':');
	}else{
	    eval(sv[i]);
	}
    }
}
writer::writer(const std::string&filename):ofs(filename),indent(0),serial(0)
{
    write(".global main");
}
void writer::enter(const std::string&func)
{
    write(func+':');
    ++indent;
    write("push","rbp");
    write("mov","rsp","rbp");
    var_size=0;
}
void writer::leave()
{
    write("mov","rbp","rsp");
    write("pop","rbp");
    write("ret");
    --indent;
    offset.clear();
}
