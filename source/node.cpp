#include"syntax/node.hpp"
#include"code/cgfuncs.hpp"
#include<stdexcept>
#include<typeinfo>
using namespace syntax;
using code::address;
using code::unique_label;
void numeric::to_asm(code::generator&gen)const
{
    gen.write("push",value);
}
void ident::to_asm(code::generator&gen)const
{
    gen.write("push",address(-gen.get_offset(name),"%rbp"));
}
void ident::refer(code::generator&gen)const
{
    gen.write("lea",address(-gen.get_offset(name),"%rbp"),"%rax");
    gen.write("push","%rax");
}
void fcall::to_asm(code::generator&gen)const
{
    int align=(16-gen.get_var_size()%16)%16;
    gen.write("sub",align,"%rsp");
    for(int i=vars->size()-1;i>=0;--i){
	(*vars)[i]->to_asm(gen);
	switch(i){
	    case 0 :gen.write("pop","%rdi");break;
	    case 1 :gen.write("pop","%rsi");break;
	    case 2 :gen.write("pop","%rdx");break;
	    case 3 :gen.write("pop","%rcx");break;
	    case 4 :gen.write("pop","%r8"); break;
	    case 5 :gen.write("pop","%r9"); break;
	}
    }
    gen.write("call",dynamic_cast<const ident*>(func)->name);
    gen.write("add",align,"%rsp");
    gen.write("push","%rax");
}
void uplus::to_asm(code::generator&gen)const
{
    arg->to_asm(gen);
}
void uminus::to_asm(code::generator&gen)const
{
    arg->to_asm(gen);
    gen.write("pop","%rax");
    gen.write("mov","%rax","%rdi");
    gen.write("mov",2,"%rsi");gen.write("mul","%rsi");
    gen.write("sub","%rax","%rdi");
    gen.write("push","%rdi");
}
void preinc::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(arg)->refer(gen);
    gen.write("pop","%rax");
    gen.write("add",1,address("%rax"));
    gen.write("push",address("%rax"));
}
void predec::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(arg)->refer(gen);
    gen.write("pop","%rax");
    gen.write("sub",1,address("%rax"));
    gen.write("push",address("%rax"));
}
void postinc::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(arg)->refer(gen);
    gen.write("pop","%rax");
    gen.write("push",address("%rax"));
    gen.write("add",1,address("%rax"));
}
void postdec::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(arg)->refer(gen);
    gen.write("pop","%rax");
    gen.write("push",address("%rax"));
    gen.write("sub",1,address("%rax"));
}
void plus::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("add","%rdi","%rax");
    gen.write("push","%rax");
}
void minus::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("sub","%rdi","%rax");
    gen.write("push","%rax");
}
void multi::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("mul","%rdi");
    gen.write("push","%rax");
}
void divide::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("mov",0,"%rdx");
    gen.write("div","%rdi");
    gen.write("push","%rax");
}
void remain::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("mov",0,"%rdx");
    gen.write("div","%rdi");
    gen.write("push","%rdx");
}
void equal::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("cmp","%rdi","%rax");
    gen.write("sete","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void nequal::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("cmp","%rdi","%rax");
    gen.write("setne","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void less::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("cmp","%rdi","%rax");
    gen.write("setl","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void greater::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("cmp","%rdi","%rax");
    gen.write("setg","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void leeq::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("cmp","%rdi","%rax");
    gen.write("setle","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void greq::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("cmp","%rdi","%rax");
    gen.write("setge","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void comma::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    gen.write("pop","%rax");
    rarg->to_asm(gen);
}
void assign::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("mov","%rdi",address("%rax"));
    gen.write("push",address("%rax"));
}
void plasgn::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("add","%rdi",address("%rax"));
    gen.write("push",address("%rax"));
}
void miasgn::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("sub","%rdi",address("%rax"));
    gen.write("push",address("%rax"));
}
void muasgn::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("mov","%rax","%rsi");
    gen.write("mov",address("%rax"),"%rax");
    gen.write("mul","%rdi");
    gen.write("mov","%rax",address("%rsi"));
    gen.write("mov","%rsi","%rax");
    gen.write("push",address("%rax"));
}
void diasgn::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("mov","%rax","%rsi");
    gen.write("mov",address("%rax"),"%rax");
    gen.write("mov",0,"%rdx");
    gen.write("div","%rdi");
    gen.write("mov","%rax",address("%rsi"));
    gen.write("mov","%rsi","%rax");
    gen.write("push",address("%rax"));
}
void rmasgn::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("mov","%rax","%rsi");
    gen.write("mov",address("%rax"),"%rax");
    gen.write("mov",0,"%rdx");
    gen.write("div","%rdi");
    gen.write("mov","%rdx",address("%rsi"));
    gen.write("mov","%rsi","%rax");
    gen.write("push",address("%rax"));
}
void single::to_asm(code::generator&gen)const
{
    if(stat){
	stat->to_asm(gen);
	gen.write("pop","%rax");
    }
}
void compound::to_asm(code::generator&gen)const
{
    for(auto s:*stats)s->to_asm(gen);
}
void declare::to_asm(code::generator&gen)const
{
    for(auto v:*vars){
	gen.write("sub",8,"%rsp");
	gen.set_offset(v.first);
	if(v.second){
	    v.second->to_asm(gen);
	    gen.write("pop","%rax");
	    gen.write("mov","%rax",address("%rsp"));
	}
    }
}
void _if_else_::to_asm(code::generator&gen)const
{
    std::string el=unique_label(".Lelse");
    std::string end=unique_label(".Lend");
    cond->to_asm(gen);
    gen.write("cmp",0,"%rax");
    gen.write("je",el);
    st1->to_asm(gen);
    gen.write("jmp",end);
    gen.write(el+':');
    st2->to_asm(gen);
    gen.write(end+':');
}
void _while_::to_asm(code::generator&gen)const
{
    std::string beg=unique_label(".Lbegin");
    std::string end=unique_label(".Lend");
    gen.write(beg+':');
    cond->to_asm(gen);
    gen.write("cmp",0,"%rax");
    gen.write("je",end);
    st->to_asm(gen);
    gen.write("jmp",beg);
    gen.write(end+':');
}
void _for_::to_asm(code::generator&gen)const
{
    std::string beg=unique_label(".Lbegin");
    std::string end=unique_label(".Lend");
    init->to_asm(gen);
    gen.write(beg+':');
    gen.write("mov",1,"%rax");
    cond->to_asm(gen);
    gen.write("cmp",0,"%rax");
    gen.write("je",end);
    st->to_asm(gen);
    reinit->to_asm(gen);
    gen.write("jmp",beg);
    gen.write(end+':');
}
void numeric::check(semantics::analyzer&analy)const
{
    return;
}
void ident::check(semantics::analyzer&analy)const
{
    if(!analy.is_declared(name))throw std::runtime_error("未定義の変数です: "+name);
}
void fcall::check(semantics::analyzer&analy)const
{
    if(typeid(*func)!=typeid(ident))throw std::runtime_error("無効な関数呼び出しです");
    func->check(analy);
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
    for(auto s:*stats)s->check(analy);
}
void declare::check(semantics::analyzer&analy)const
{
    for(auto v:*vars){
	if(analy.is_declared(v.first))throw std::runtime_error("二重定義されました: "+v.first);
	analy.declare(v.first);
	if(v.second)v.second->check(analy);
    }
}
void _if_else_::check(semantics::analyzer&analy)const
{
    cond->check(analy);
    st1->check(analy);
    st2->check(analy);
}
void _while_::check(semantics::analyzer&analy)const
{
    cond->check(analy);
    st->check(analy);
}
void _for_::check(semantics::analyzer&analy)const
{
    init->check(analy);
    cond->check(analy);
    reinit->check(analy);
    st->check(analy);
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
declare   ::declare    (const std::vector<std::pair<std::string,const expression*>>*vars)          :vars(vars)                                 {}
_if_else_ ::_if_else_  (const single*cond,const statement*st1,const statement*st2)                 :cond(cond),st1(st1),st2(st2)               {}
_while_   ::_while_    (const single*cond,const statement*st)                                      :cond(cond),st(st)                          {}
_for_     ::_for_      (const single*init,const single*cond,const single*reinit,const statement*st):init(init),cond(cond),reinit(reinit),st(st){}
fcall     ::~fcall     ()                                                                    {delete func;for(auto v:*vars)delete v;delete vars;}
unopr     ::~unopr     ()                                                                                                           {delete arg;}
biopr     ::~biopr     ()                                                                                              {delete larg;delete rarg;}
single    ::~single    ()                                                                                                          {delete stat;}
compound  ::~compound  ()                                                                              {for(auto s:*stats)delete s;delete stats;}
declare   ::~declare   ()                                                                         {for(auto v:*vars)delete v.second;delete vars;}
_if_else_ ::~_if_else_ ()                                                                                    {delete cond;delete st1;delete st2;}
_while_   ::~_while_   ()                                                                                                {delete cond;delete st;}
_for_     ::~_for_     ()                                                                      {delete init;delete cond;delete reinit;delete st;}
node      ::~node      ()                                                                                                                      {}
expression::~expression()                                                                                                                      {}
statement ::~statement ()                                                                                                                      {}
