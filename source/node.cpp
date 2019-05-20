#include"syntax/node.hpp"
#include"code/cgfuncs.hpp"
#include<stdexcept>
#include<typeinfo>
using namespace syntax;
using code::address;
using code::unique_label;
void numeric::to_asm(code::variable_manager&vm)const
{
    vm.write("push",value);
}
void ident::to_asm(code::variable_manager&vm)const
{
    vm.write("push",address(-vm.get_offset(name),"%rbp"));
}
void ident::refer(code::variable_manager&vm)const
{
    vm.write("lea",address(-vm.get_offset(name),"%rbp"),"%rax");
    vm.write("push","%rax");
}
void fcall::to_asm(code::variable_manager&vm)const
{
    int align=(16-vm.get_var_size()%16)%16;
    vm.write("sub",align,"%rsp");
    for(int i=vars->size()-1;i>=0;--i){
	(*vars)[i]->to_asm(vm);
	switch(i){
	    case 0 :vm.write("pop","%rdi");break;
	    case 1 :vm.write("pop","%rsi");break;
	    case 2 :vm.write("pop","%rdx");break;
	    case 3 :vm.write("pop","%rcx");break;
	    case 4 :vm.write("pop","%r8"); break;
	    case 5 :vm.write("pop","%r9"); break;
	}
    }
    vm.write("call",dynamic_cast<const ident*>(func)->name);
    if(vars->size()>6){
	vm.write("add",8*(vars->size()-6),"%rsp");
    }
    vm.write("add",align,"%rsp");
    vm.write("push","%rax");
}
void uplus::to_asm(code::variable_manager&vm)const
{
    arg->to_asm(vm);
}
void uminus::to_asm(code::variable_manager&vm)const
{
    arg->to_asm(vm);
    vm.write("pop","%rax");
    vm.write("mov","%rax","%rdi");
    vm.write("mov",2,"%rsi");vm.write("mul","%rsi");
    vm.write("sub","%rax","%rdi");
    vm.write("push","%rdi");
}
void preinc::to_asm(code::variable_manager&vm)const
{
    dynamic_cast<const ident*>(arg)->refer(vm);
    vm.write("pop","%rax");
    vm.write("add",1,address("%rax"));
    vm.write("push",address("%rax"));
}
void predec::to_asm(code::variable_manager&vm)const
{
    dynamic_cast<const ident*>(arg)->refer(vm);
    vm.write("pop","%rax");
    vm.write("sub",1,address("%rax"));
    vm.write("push",address("%rax"));
}
void postinc::to_asm(code::variable_manager&vm)const
{
    dynamic_cast<const ident*>(arg)->refer(vm);
    vm.write("pop","%rax");
    vm.write("push",address("%rax"));
    vm.write("add",1,address("%rax"));
}
void postdec::to_asm(code::variable_manager&vm)const
{
    dynamic_cast<const ident*>(arg)->refer(vm);
    vm.write("pop","%rax");
    vm.write("push",address("%rax"));
    vm.write("sub",1,address("%rax"));
}
void plus::to_asm(code::variable_manager&vm)const
{
    larg->to_asm(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("add","%rdi","%rax");
    vm.write("push","%rax");
}
void minus::to_asm(code::variable_manager&vm)const
{
    larg->to_asm(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("sub","%rdi","%rax");
    vm.write("push","%rax");
}
void multi::to_asm(code::variable_manager&vm)const
{
    larg->to_asm(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("mul","%rdi");
    vm.write("push","%rax");
}
void divide::to_asm(code::variable_manager&vm)const
{
    larg->to_asm(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("mov",0,"%rdx");
    vm.write("div","%rdi");
    vm.write("push","%rax");
}
void remain::to_asm(code::variable_manager&vm)const
{
    larg->to_asm(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("mov",0,"%rdx");
    vm.write("div","%rdi");
    vm.write("push","%rdx");
}
void equal::to_asm(code::variable_manager&vm)const
{
    larg->to_asm(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("cmp","%rdi","%rax");
    vm.write("sete","%al");
    vm.write("movzb","%al","%rax");
    vm.write("push","%rax");
}
void nequal::to_asm(code::variable_manager&vm)const
{
    larg->to_asm(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("cmp","%rdi","%rax");
    vm.write("setne","%al");
    vm.write("movzb","%al","%rax");
    vm.write("push","%rax");
}
void less::to_asm(code::variable_manager&vm)const
{
    larg->to_asm(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("cmp","%rdi","%rax");
    vm.write("setl","%al");
    vm.write("movzb","%al","%rax");
    vm.write("push","%rax");
}
void greater::to_asm(code::variable_manager&vm)const
{
    larg->to_asm(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("cmp","%rdi","%rax");
    vm.write("setg","%al");
    vm.write("movzb","%al","%rax");
    vm.write("push","%rax");
}
void leeq::to_asm(code::variable_manager&vm)const
{
    larg->to_asm(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("cmp","%rdi","%rax");
    vm.write("setle","%al");
    vm.write("movzb","%al","%rax");
    vm.write("push","%rax");
}
void greq::to_asm(code::variable_manager&vm)const
{
    larg->to_asm(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("cmp","%rdi","%rax");
    vm.write("setge","%al");
    vm.write("movzb","%al","%rax");
    vm.write("push","%rax");
}
void comma::to_asm(code::variable_manager&vm)const
{
    larg->to_asm(vm);
    vm.write("add",8,"%rsp");
    rarg->to_asm(vm);
}
void assign::to_asm(code::variable_manager&vm)const
{
    dynamic_cast<const ident*>(larg)->refer(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("mov","%rdi",address("%rax"));
    vm.write("push",address("%rax"));
}
void plasgn::to_asm(code::variable_manager&vm)const
{
    dynamic_cast<const ident*>(larg)->refer(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("add","%rdi",address("%rax"));
    vm.write("push",address("%rax"));
}
void miasgn::to_asm(code::variable_manager&vm)const
{
    dynamic_cast<const ident*>(larg)->refer(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("sub","%rdi",address("%rax"));
    vm.write("push",address("%rax"));
}
void muasgn::to_asm(code::variable_manager&vm)const
{
    dynamic_cast<const ident*>(larg)->refer(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("mov","%rax","%rsi");
    vm.write("mov",address("%rax"),"%rax");
    vm.write("mul","%rdi");
    vm.write("mov","%rax",address("%rsi"));
    vm.write("push","%rax");
}
void diasgn::to_asm(code::variable_manager&vm)const
{
    dynamic_cast<const ident*>(larg)->refer(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("mov","%rax","%rsi");
    vm.write("mov",address("%rax"),"%rax");
    vm.write("mov",0,"%rdx");
    vm.write("div","%rdi");
    vm.write("mov","%rax",address("%rsi"));
    vm.write("push","%rax");
}
void rmasgn::to_asm(code::variable_manager&vm)const
{
    dynamic_cast<const ident*>(larg)->refer(vm);
    rarg->to_asm(vm);
    vm.write("pop","%rdi");
    vm.write("pop","%rax");
    vm.write("mov","%rax","%rsi");
    vm.write("mov",address("%rax"),"%rax");
    vm.write("mov",0,"%rdx");
    vm.write("div","%rdi");
    vm.write("mov","%rdx",address("%rsi"));
    vm.write("push","%rdx");
}
void single::to_asm(code::variable_manager&vm)const
{
    if(stat){
	stat->to_asm(vm);
	vm.write("pop","%rax");
    }
}
void compound::to_asm(code::variable_manager&vm)const
{
    vm.enter_scope();
    for(auto s:*stats)s->to_asm(vm);
    vm.leave_scope();
}
void define::to_asm(code::variable_manager&vm)const
{
    for(auto v:*vars){
	vm.write("sub",8,"%rsp");
	vm.set_offset(v.first);
	if(v.second){
	    v.second->to_asm(vm);
	    vm.write("pop",address("%rsp"));
	}
    }
}
void _if_else_::to_asm(code::variable_manager&vm)const
{
    std::string el=unique_label(".Lelse");
    std::string end=unique_label(".Lend");
    vm.enter_scope();
    cond->to_asm(vm);
    vm.write("cmp",0,"%rax");
    vm.write("je",el);
    st1->to_asm(vm);
    vm.write("jmp",end);
    vm.write(el+':');
    st2->to_asm(vm);
    vm.leave_scope();
    vm.write(end+':');
}
void _while_::to_asm(code::variable_manager&vm)const
{
    std::string beg=unique_label(".Lbegin");
    std::string end=unique_label(".Lend");
    vm.write(beg+':');
    vm.enter_scope();
    cond->to_asm(vm);
    vm.write("cmp",0,"%rax");
    vm.write("je",end);
    st->to_asm(vm);
    vm.leave_scope();
    vm.write("jmp",beg);
    vm.write(end+':');
}
void _for_::to_asm(code::variable_manager&vm)const
{
    std::string beg=unique_label(".Lbegin");
    std::string end=unique_label(".Lend");
    vm.enter_scope();
    init->to_asm(vm);
    vm.write(beg+':');
    vm.write("mov",1,"%rax");
    cond->to_asm(vm);
    vm.write("cmp",0,"%rax");
    vm.write("je",end);
    st->to_asm(vm);
    reinit->to_asm(vm);
    vm.leave_scope();
    vm.write("jmp",beg);
    vm.write(end+':');
}
void _return_::to_asm(code::variable_manager&vm)const
{
    val->to_asm(vm);
    vm.write("mov","%rbp","%rsp");
    vm.write("pop","%rbp");
    vm.write("ret");
}
void function::to_asm(code::variable_manager&vm)const
{
    vm.write(".globl "+name);
    vm.write(name+':');
    vm.write("push","%rbp");
    vm.write("mov","%rsp","%rbp");
    vm.enter_scope();
    vm.write("sub",8*args->size(),"%rsp");
    for(int i;i<args->size();++i){
	std::string dest=address(i+1-args->size(),"%rsp");
	vm.set_offset((*args)[i]);
	switch(i){
	    case 0 :vm.write("mov","%rdi",dest);break;
	    case 1 :vm.write("mov","%rsi",dest);break;
	    case 2 :vm.write("mov","%rdx",dest);break;
	    case 3 :vm.write("mov","%rcx",dest);break;
	    case 4 :vm.write("mov","%r8" ,dest);break;
	    case 5 :vm.write("mov","%r9" ,dest);break;
	    default:
		    vm.write("mov",address(8*(i-6)+16,"%rbp"),"%rax");
		    vm.write("mov","%rax",dest);
		    break;
	}
    }
    for(auto s:*(com->stats))s->to_asm(vm);
    vm.leave_scope();
    //強制return
    vm.write("mov","%rbp","%rsp");
    vm.write("pop","%rbp");
    vm.write("ret");
}
void numeric::check(semantics::analyzer&analy)const
{
    return;
}
void ident::check(semantics::analyzer&analy)const
{
    if(!analy.is_available(name))throw std::runtime_error("未定義の変数です: "+name);
}
void fcall::check(semantics::analyzer&analy)const
{
    if(typeid(*func)!=typeid(ident))throw std::runtime_error("無効な関数呼び出しです");
    for(auto v:*vars)v->check(analy);
}
void unopr::check(semantics::analyzer&analy)const
{
    arg->check(analy);
}
void unopr_l::check(semantics::analyzer&analy)const
{
    if(typeid(*arg)!=typeid(ident))throw std::runtime_error("右辺値への操作です");
    unopr::check(analy);
}
void biopr::check(semantics::analyzer&analy)const
{
    larg->check(analy);
    rarg->check(analy);
}
void biopr_l::check(semantics::analyzer&analy)const
{
    if(typeid(*larg)!=typeid(ident))throw std::runtime_error("右辺値への代入です");
    biopr::check(analy);
}
void single::check(semantics::analyzer&analy)const
{
    if(stat)stat->check(analy);
}
void compound::check(semantics::analyzer&analy)const
{
    analy.enter_scope();
    for(auto s:*stats)s->check(analy);
    analy.leave_scope();
}
void define::check(semantics::analyzer&analy)const
{
    for(auto v:*vars){
	if(!analy.is_definable(v.first))throw std::runtime_error("二重定義されました: "+v.first);
	analy.define(v.first);
	if(v.second)v.second->check(analy);
    }
}
void _if_else_::check(semantics::analyzer&analy)const
{
    analy.enter_scope();
    cond->check(analy);
    st1->check(analy);
    st2->check(analy);
    analy.leave_scope();
}
void _while_::check(semantics::analyzer&analy)const
{
    analy.enter_scope();
    cond->check(analy);
    st->check(analy);
    analy.leave_scope();
}
void _for_::check(semantics::analyzer&analy)const
{
    analy.enter_scope();
    init->check(analy);
    cond->check(analy);
    reinit->check(analy);
    st->check(analy);
    analy.leave_scope();
}
void _return_::check(semantics::analyzer&analy)const
{
    val->check(analy);
}
void function::check(semantics::analyzer&analy)const
{
    analy.enter_scope();
    for(auto a:*args)analy.define(a);
    for(auto s:*(com->stats))s->check(analy);
    analy.leave_scope();
}
numeric   ::numeric    (int value)                                                                 :value(value)                               {}
ident     ::ident      (const std::string&name)                                                    :name(name)                                 {}
fcall     ::fcall      (const expression*func,const std::vector<const expression*>*vars)           :func(func),vars(vars)                      {}
unopr     ::unopr      (const expression*arg)                                                      :arg(arg)                                   {}
uplus     ::uplus      (const expression*arg)                                                      :unopr(arg)                                 {}
uminus    ::uminus     (const expression*arg)                                                      :unopr(arg)                                 {}
unopr_l   ::unopr_l    (const expression*arg)                                                      :unopr(arg)                                 {}
preinc    ::preinc     (const expression*arg)                                                      :unopr_l(arg)                               {}
predec    ::predec     (const expression*arg)                                                      :unopr_l(arg)                               {}
postinc   ::postinc    (const expression*arg)                                                      :unopr_l(arg)                               {}
postdec   ::postdec    (const expression*arg)                                                      :unopr_l(arg)                               {}
biopr     ::biopr      (const expression*larg,const expression*rarg)                               :larg(larg),rarg(rarg)                      {}
plus      ::plus       (const expression*larg,const expression*rarg)                               :biopr(larg,rarg)                           {}
minus     ::minus      (const expression*larg,const expression*rarg)                               :biopr(larg,rarg)                           {}
multi     ::multi      (const expression*larg,const expression*rarg)                               :biopr(larg,rarg)                           {}
divide    ::divide     (const expression*larg,const expression*rarg)                               :biopr(larg,rarg)                           {}
remain    ::remain     (const expression*larg,const expression*rarg)                               :biopr(larg,rarg)                           {}
equal     ::equal      (const expression*larg,const expression*rarg)                               :biopr(larg,rarg)                           {}
nequal    ::nequal     (const expression*larg,const expression*rarg)                               :biopr(larg,rarg)                           {}
less      ::less       (const expression*larg,const expression*rarg)                               :biopr(larg,rarg)                           {}
greater   ::greater    (const expression*larg,const expression*rarg)                               :biopr(larg,rarg)                           {}
leeq      ::leeq       (const expression*larg,const expression*rarg)                               :biopr(larg,rarg)                           {}
greq      ::greq       (const expression*larg,const expression*rarg)                               :biopr(larg,rarg)                           {}
comma     ::comma      (const expression*larg,const expression*rarg)                               :biopr(larg,rarg)                           {}
biopr_l   ::biopr_l    (const expression*larg,const expression*rarg)                               :biopr(larg,rarg)                           {}
assign    ::assign     (const expression*larg,const expression*rarg)                               :biopr_l(larg,rarg)                         {}
plasgn    ::plasgn     (const expression*larg,const expression*rarg)                               :biopr_l(larg,rarg)                         {}
miasgn    ::miasgn     (const expression*larg,const expression*rarg)                               :biopr_l(larg,rarg)                         {}
muasgn    ::muasgn     (const expression*larg,const expression*rarg)                               :biopr_l(larg,rarg)                         {}
diasgn    ::diasgn     (const expression*larg,const expression*rarg)                               :biopr_l(larg,rarg)                         {}
rmasgn    ::rmasgn     (const expression*larg,const expression*rarg)                               :biopr_l(larg,rarg)                         {}
single    ::single     (const expression*stat)                                                     :stat(stat)                                 {}
compound  ::compound   (const std::vector<const statement*>*stats)                                 :stats(stats)                               {}
define    ::define     (const std::vector<std::pair<std::string,const expression*>>*vars)          :vars(vars)                                 {}
_if_else_ ::_if_else_  (const single*cond,const statement*st1,const statement*st2)                 :cond(cond),st1(st1),st2(st2)               {}
_while_   ::_while_    (const single*cond,const statement*st)                                      :cond(cond),st(st)                          {}
_for_     ::_for_      (const single*init,const single*cond,const single*reinit,const statement*st):init(init),cond(cond),reinit(reinit),st(st){}
_return_  ::_return_   (const single*val)                                                          :val(val)                                   {}
function  ::function   (std::string name,const std::vector<std::string>*args,const compound*com)   :name(name),args(args),com(com)             {}
fcall     ::~fcall     ()                                                                    {delete func;for(auto v:*vars)delete v;delete vars;}
unopr     ::~unopr     ()                                                                                                           {delete arg;}
biopr     ::~biopr     ()                                                                                              {delete larg;delete rarg;}
single    ::~single    ()                                                                                                          {delete stat;}
compound  ::~compound  ()                                                                              {for(auto s:*stats)delete s;delete stats;}
define    ::~define    ()                                                                         {for(auto v:*vars)delete v.second;delete vars;}
_if_else_ ::~_if_else_ ()                                                                                    {delete cond;delete st1;delete st2;}
_while_   ::~_while_   ()                                                                                                {delete cond;delete st;}
_for_     ::~_for_     ()                                                                      {delete init;delete cond;delete reinit;delete st;}
_return_  ::~_return_  ()                                                                                                           {delete val;}
function  ::~function  ()                                                                                               {delete com;delete args;}
