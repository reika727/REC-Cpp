#include"assembly_source/parser.hpp"
using namespace assembly_source;
using ND=abstract_syntax_tree::ND;
void parser::refer_var(abstract_syntax_tree::node*const node)
{
    if(auto idp=dynamic_cast<abstract_syntax_tree::ident*>(node)){
	if(offset.count(idp->name)){
	    wr.write("mov","rbp","rax");
	    wr.write("sub",offset[idp->name],"rax");
	    wr.write("push","rax");
	}else{
	    throw std::runtime_error("未定義の変数です: "+idp->name);
	}
    }else{
	throw std::runtime_error("右辺値への代入はできません");
    }
}
void parser::RDP(abstract_syntax_tree::node*const node)
{
    if(auto nup=dynamic_cast<abstract_syntax_tree::numeric*>(node)){
	wr.write("push",nup->value);
    }else if(auto idp=dynamic_cast<abstract_syntax_tree::ident*>(node)){
	refer_var(idp);
	wr.write("pop","rax");
	wr.write("push",writer::derefer("rax"));
    }else if(auto cfp=dynamic_cast<abstract_syntax_tree::fcall*>(node)){
	for(;!cfp->args.empty();cfp->args.pop_back()){
	    RDP(cfp->args.back());
	    wr.write("pop","rax");
	    switch(cfp->args.size()){
		case 1 :wr.write("mov","rax","rdi");break;
		case 2 :wr.write("mov","rax","rsi");break;
		case 3 :wr.write("mov","rax","rdx");break;
		case 4 :wr.write("mov","rax","rcx");break;
		case 5 :wr.write("mov","rax","r8"); break;
		case 6 :wr.write("mov","rax","r9"); break;
		default:wr.write("push","rax");     break;
	    }
	}
	wr.write("sub",(16-var_size%16)%16,"rsp");
	wr.write("call "+cfp->name);
    }else if(auto uop=dynamic_cast<abstract_syntax_tree::unopr*>(node)){
	if(uop->type==ND::UPLUS){
	    RDP(uop->arg);
	}else if(uop->type==ND::UMINUS){
	    RDP(uop->arg);
	    wr.write("pop","rax");
	    wr.write("mov","rax","rdi");
	    wr.write("mov",2,"rsi");wr.write("mul","rsi");
	    wr.write("sub","rax","rdi");
	    wr.write("push","rdi");
	}else if(uop->type==ND::PREINC||uop->type==ND::PREDEC){
	    refer_var(uop->arg);
	    wr.write("pop","rax");
	    switch(uop->type){
		case ND::PREINC:
		    wr.write("add",1,writer::derefer("rax"));
		    break;
		case ND::PREDEC:
		    wr.write("sub",1,writer::derefer("rax"));
		    break;
	    }
	    wr.write("push",writer::derefer("rax"));
	}
    }else if(auto bop=dynamic_cast<abstract_syntax_tree::biopr*>(node)){
	if(ND::ASSIGN<=bop->type&&bop->type<=ND::RMASGN){
	    refer_var(bop->larg);
	    RDP(bop->rarg);
	    wr.write("pop","rdi");
	    wr.write("pop","rax");
	    switch(bop->type){
		case ND::ASSIGN:
		    wr.write("mov","rdi",writer::derefer("rax"));
		    break;
		case ND::PLASGN:
		    wr.write("add","rdi",writer::derefer("rax"));
		    break;
		case ND::MIASGN:
		    wr.write("sub","rdi",writer::derefer("rax"));
		    break;
		case ND::MUASGN:
		    wr.write("mov","rax","rsi");
		    wr.write("mov",writer::derefer("rax"),"rax");
		    wr.write("mul","rdi");
		    wr.write("mov","rax",writer::derefer("rsi"));
		    wr.write("mov","rsi","rax");
		    break;
		case ND::DIASGN:
		    wr.write("mov","rax","rsi");
		    wr.write("mov",writer::derefer("rax"),"rax");
		    wr.write("mov",0,"rdx");
		    wr.write("div","rdi");
		    wr.write("mov","rax",writer::derefer("rsi"));
		    wr.write("mov","rsi","rax");
		    break;
		case ND::RMASGN:
		    wr.write("mov","rax","rsi");
		    wr.write("mov",writer::derefer("rax"),"rax");
		    wr.write("mov",0,"rdx");
		    wr.write("div","rdi");
		    wr.write("mov","rdx",writer::derefer("rsi"));
		    wr.write("mov","rsi","rax");
		    break;
	    }
	    wr.write("push",writer::derefer("rax"));
	}else{
	    RDP(bop->larg);
	    RDP(bop->rarg);
	    wr.write("pop","rdi");
	    wr.write("pop","rax");
	    switch(bop->type){
		case ND::PLUS:
		    wr.write("add","rdi","rax");
		    break;
		case ND::MINUS:
		    wr.write("sub","rdi","rax");
		    break;
		case ND::MULTI:
		    wr.write("mul","rdi");
		    break;
		case ND::DIVIDE:
		    wr.write("mov",0,"rdx");
		    wr.write("div","rdi");
		    break;
		case ND::REMAIN:
		    wr.write("mov",0,"rdx");
		    wr.write("div","rdi");
		    wr.write("mov","rdx","rax");
		    break;
		case ND::EQUAL:
		    wr.write("cmp","rdi","rax");
		    wr.write("sete","al");
		    wr.write("movzb","al","rax");
		    break;
		case ND::NEQUAL:
		    wr.write("cmp","rdi","rax");
		    wr.write("setne","al");
		    wr.write("movzb","al","rax");
		    break;
		case ND::LESS:
		    wr.write("cmp","rdi","rax");
		    wr.write("setl","al");
		    wr.write("movzb","al","rax");
		    break;
		case ND::GREAT:
		    wr.write("cmp","rdi","rax");
		    wr.write("setg","al");
		    wr.write("movzb","al","rax");
		    break;
		case ND::LEEQ:
		    wr.write("cmp","rdi","rax");
		    wr.write("setle","al");
		    wr.write("movzb","al","rax");
		    break;
		case ND::GREQ:
		    wr.write("cmp","rdi","rax");
		    wr.write("setge","al");
		    wr.write("movzb","al","rax");
		    break;
	    }
	    wr.write("push","rax");
	}
    }
}
void parser::eval(abstract_syntax_tree::statement*const st)
{
    if(auto sg=dynamic_cast<abstract_syntax_tree::single*>(st)){
	if(!sg->is_nop()){
	    RDP(sg->stat);
	    wr.write("pop","rax");
	}
    }else if(auto com=dynamic_cast<abstract_syntax_tree::compound*>(st)){
	eval(com->stats);
    }
}
void parser::eval(const std::vector<abstract_syntax_tree::statement*>&sv)
{
    for(int i=0;i<sv.size();++i){
	if(auto dep=dynamic_cast<abstract_syntax_tree::declare*>(sv[i])){
	    if(!offset.count(dep->name)){
		wr.write("sub",8,"rsp");
		offset[dep->name]=var_size+=8;
	    }else{
		throw std::runtime_error("同名の変数が再定義されています");
	    }
	}else if(auto cif=dynamic_cast<abstract_syntax_tree::_if_*>(sv[i])){
	    if(auto cel=dynamic_cast<abstract_syntax_tree::_else_*>(i+1<sv.size()?sv[i+1]:nullptr)){
		std::string el=wr.unique_label("Lelse");
		std::string end=wr.unique_label("Lend");
		eval(cif->cond);
		wr.write("cmp",0,"rax");
		wr.write("je",el);
		eval(cif->st);
		wr.write("jmp",end);
		wr.write(el+':');
		eval(cel->st);
		wr.write(end+':');
	    }else{
		std::string end=wr.unique_label("Lend");
		eval(cif->cond);
		wr.write("cmp",0,"rax");
		wr.write("je",end);
		eval(cif->st);
		wr.write(end+':');
	    }
	}else if(auto cwh=dynamic_cast<abstract_syntax_tree::_while_*>(sv[i])){
	    std::string beg=wr.unique_label("Lbegin");
	    std::string end=wr.unique_label("Lend");
	    wr.write(beg+':');
	    eval(cwh->cond);
	    wr.write("cmp",0,"rax");
	    wr.write("je",end);
	    eval(cwh->st);
	    wr.write("jmp",beg);
	    wr.write(end+':');
	}else if(auto cfo=dynamic_cast<abstract_syntax_tree::_for_*>(sv[i])){
	    std::string beg=wr.unique_label("Lbegin");
	    std::string end=wr.unique_label("Lend");
	    eval(cfo->init);
	    wr.write(beg+':');
	    wr.write("mov",1,"rax");
	    eval(cfo->cond);
	    wr.write("cmp",0,"rax");
	    wr.write("je",end);
	    eval(cfo->st);
	    eval(cfo->reinit);
	    wr.write("jmp",beg);
	    wr.write(end+':');
	}else{
	    eval(sv[i]);
	}
    }
}
parser::parser(abstract_syntax_tree::compound*const rt,std::string filename):wr(filename),var_size(0)
{
    //TODO
	wr.write("main:");
	wr.write("push","rbp");
	wr.write("mov","rsp","rbp");

    eval(rt);

    //TODO
	wr.write("mov","rbp","rsp");
	wr.write("pop","rbp");
	wr.write("ret");
}
