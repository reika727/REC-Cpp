#include"syntax/node.hpp"
#include"code/cgfuncs.hpp"
#include<stdexcept>
#include<typeinfo>
using namespace syntax;
using code::address;
using code::unique_label;
void numeric::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    wr.write("push",value);
}
void ident::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    wr.write("push",address(-vm.get_offset(name),"%rbp"));
}
void ident::refer(code::variable_manager&vm,code::writer&wr)const
{
    wr.write("lea",address(-vm.get_offset(name),"%rbp"),"%rax");
    wr.write("push","%rax");
}
void fcall::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    int align=(16-vm.get_var_size()%16)%16;
    wr.write("sub",align,"%rsp");
    for(int i=vars->size()-1;i>=0;--i){
	(*vars)[i]->to_asm(vm,wr);
	switch(i){
	    case 0 :wr.write("pop","%rdi");break;
	    case 1 :wr.write("pop","%rsi");break;
	    case 2 :wr.write("pop","%rdx");break;
	    case 3 :wr.write("pop","%rcx");break;
	    case 4 :wr.write("pop","%r8"); break;
	    case 5 :wr.write("pop","%r9"); break;
	}
    }
    wr.write("call",dynamic_cast<const ident*>(func)->name);
    for(int i=0;i<static_cast<signed>(vars->size())-6;++i){
	wr.write("pop","%rax");
    }
    wr.write("add",align,"%rsp");
    wr.write("push","%rax");
}
void uplus::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    arg->to_asm(vm,wr);
}
void uminus::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    arg->to_asm(vm,wr);
    wr.write("pop","%rax");
    wr.write("mov","%rax","%rdi");
    wr.write("mov",2,"%rsi");wr.write("mul","%rsi");
    wr.write("sub","%rax","%rdi");
    wr.write("push","%rdi");
}
void preinc::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    dynamic_cast<const ident*>(arg)->refer(vm,wr);
    wr.write("pop","%rax");
    wr.write("add",1,address("%rax"));
    wr.write("push",address("%rax"));
}
void predec::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    dynamic_cast<const ident*>(arg)->refer(vm,wr);
    wr.write("pop","%rax");
    wr.write("sub",1,address("%rax"));
    wr.write("push",address("%rax"));
}
void postinc::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    dynamic_cast<const ident*>(arg)->refer(vm,wr);
    wr.write("pop","%rax");
    wr.write("push",address("%rax"));
    wr.write("add",1,address("%rax"));
}
void postdec::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    dynamic_cast<const ident*>(arg)->refer(vm,wr);
    wr.write("pop","%rax");
    wr.write("push",address("%rax"));
    wr.write("sub",1,address("%rax"));
}
void plus::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    larg->to_asm(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("add","%rdi","%rax");
    wr.write("push","%rax");
}
void minus::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    larg->to_asm(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("sub","%rdi","%rax");
    wr.write("push","%rax");
}
void multi::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    larg->to_asm(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("mul","%rdi");
    wr.write("push","%rax");
}
void divide::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    larg->to_asm(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("mov",0,"%rdx");
    wr.write("div","%rdi");
    wr.write("push","%rax");
}
void remain::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    larg->to_asm(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("mov",0,"%rdx");
    wr.write("div","%rdi");
    wr.write("push","%rdx");
}
void equal::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    larg->to_asm(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("cmp","%rdi","%rax");
    wr.write("sete","%al");
    wr.write("movzb","%al","%rax");
    wr.write("push","%rax");
}
void nequal::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    larg->to_asm(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("cmp","%rdi","%rax");
    wr.write("setne","%al");
    wr.write("movzb","%al","%rax");
    wr.write("push","%rax");
}
void less::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    larg->to_asm(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("cmp","%rdi","%rax");
    wr.write("setl","%al");
    wr.write("movzb","%al","%rax");
    wr.write("push","%rax");
}
void greater::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    larg->to_asm(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("cmp","%rdi","%rax");
    wr.write("setg","%al");
    wr.write("movzb","%al","%rax");
    wr.write("push","%rax");
}
void leeq::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    larg->to_asm(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("cmp","%rdi","%rax");
    wr.write("setle","%al");
    wr.write("movzb","%al","%rax");
    wr.write("push","%rax");
}
void greq::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    larg->to_asm(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("cmp","%rdi","%rax");
    wr.write("setge","%al");
    wr.write("movzb","%al","%rax");
    wr.write("push","%rax");
}
void comma::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    larg->to_asm(vm,wr);
    wr.write("pop","%rax");
    rarg->to_asm(vm,wr);
}
void assign::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    dynamic_cast<const ident*>(larg)->refer(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("mov","%rdi",address("%rax"));
    wr.write("push",address("%rax"));
}
void plasgn::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    dynamic_cast<const ident*>(larg)->refer(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("add","%rdi",address("%rax"));
    wr.write("push",address("%rax"));
}
void miasgn::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    dynamic_cast<const ident*>(larg)->refer(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("sub","%rdi",address("%rax"));
    wr.write("push",address("%rax"));
}
void muasgn::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    dynamic_cast<const ident*>(larg)->refer(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("mov","%rax","%rsi");
    wr.write("mov",address("%rax"),"%rax");
    wr.write("mul","%rdi");
    wr.write("mov","%rax",address("%rsi"));
    wr.write("push","%rax");
}
void diasgn::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    dynamic_cast<const ident*>(larg)->refer(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("mov","%rax","%rsi");
    wr.write("mov",address("%rax"),"%rax");
    wr.write("mov",0,"%rdx");
    wr.write("div","%rdi");
    wr.write("mov","%rax",address("%rsi"));
    wr.write("push","%rax");
}
void rmasgn::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    dynamic_cast<const ident*>(larg)->refer(vm,wr);
    rarg->to_asm(vm,wr);
    wr.write("pop","%rdi");
    wr.write("pop","%rax");
    wr.write("mov","%rax","%rsi");
    wr.write("mov",address("%rax"),"%rax");
    wr.write("mov",0,"%rdx");
    wr.write("div","%rdi");
    wr.write("mov","%rdx",address("%rsi"));
    wr.write("push","%rdx");
}
void single::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    if(stat){
	stat->to_asm(vm,wr);
	wr.write("pop","%rax");
    }
}
void compound::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    vm.enter_scope();
    for(auto s:*stats)s->to_asm(vm,wr);
    vm.leave_scope();
}
void define::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    for(auto v:*vars){
	wr.write("sub",8,"%rsp");
	vm.set_offset(v.first);
	if(v.second){
	    v.second->to_asm(vm,wr);
	    wr.write("pop",address("%rsp"));
	}
    }
}
void _if_else_::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    vm.enter_scope();
    std::string el=unique_label(".Lelse");
    std::string end=unique_label(".Lend");
    cond->to_asm(vm,wr);
    wr.write("cmp",0,"%rax");
    wr.write("je",el);
    st1->to_asm(vm,wr);
    wr.write("jmp",end);
    wr.write(el+':');
    st2->to_asm(vm,wr);
    wr.write(end+':');
    vm.leave_scope();
}
void _while_::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    vm.enter_scope();
    std::string beg=unique_label(".Lbegin");
    std::string end=unique_label(".Lend");
    wr.write(beg+':');
    cond->to_asm(vm,wr);
    wr.write("cmp",0,"%rax");
    wr.write("je",end);
    st->to_asm(vm,wr);
    wr.write("jmp",beg);
    wr.write(end+':');
    vm.leave_scope();
}
void _for_::to_asm(code::variable_manager&vm,code::writer&wr)const
{
    vm.enter_scope();
    std::string beg=unique_label(".Lbegin");
    std::string end=unique_label(".Lend");
    init->to_asm(vm,wr);
    wr.write(beg+':');
    wr.write("mov",1,"%rax");
    cond->to_asm(vm,wr);
    wr.write("cmp",0,"%rax");
    wr.write("je",end);
    st->to_asm(vm,wr);
    reinit->to_asm(vm,wr);
    wr.write("jmp",beg);
    wr.write(end+':');
    vm.leave_scope();
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
    /*TODO*///func->check(analy);
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
fcall     ::~fcall     ()                                                                    {delete func;for(auto v:*vars)delete v;delete vars;}
unopr     ::~unopr     ()                                                                                                           {delete arg;}
biopr     ::~biopr     ()                                                                                              {delete larg;delete rarg;}
single    ::~single    ()                                                                                                          {delete stat;}
compound  ::~compound  ()                                                                              {for(auto s:*stats)delete s;delete stats;}
define    ::~define    ()                                                                         {for(auto v:*vars)delete v.second;delete vars;}
_if_else_ ::~_if_else_ ()                                                                                    {delete cond;delete st1;delete st2;}
_while_   ::~_while_   ()                                                                                                {delete cond;delete st;}
_for_     ::~_for_     ()                                                                      {delete init;delete cond;delete reinit;delete st;}
node      ::~node      ()                                                                                                                      {}
expression::~expression()                                                                                                                      {}
statement ::~statement ()                                                                                                                      {}
