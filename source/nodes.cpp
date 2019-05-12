#include"syntax/nodes.hpp"
#include"semantics/analyzer.hpp"
#include"code/generator.hpp"
#include"code/gcfuncs.hpp"
#include"code/assembly/instructions.hpp"
#include"code/assembly/registries.hpp"
#include<stdexcept>
#include<typeinfo>
using namespace syntax;
using code::derefer;
void numeric::to_asm(code::generator&gen)const
{
    gen.write(push,value);
}
void ident::to_asm(code::generator&gen)const
{
    gen.write(mov,rbp,rax);
    gen.write(sub,gen.get_offset(name),rax);
    gen.write(push,derefer(rax));
}
void ident::refer(code::generator&gen)const
{
    gen.write(mov,rbp,rax);
    gen.write(sub,gen.get_offset(name),rax);
    gen.write(push,rax);
}
void fcall::to_asm(code::generator&gen)const
{
    int align=(16-gen.get_var_size()%16)%16;
    gen.write(sub,align,rsp);
    for(int i=vars->size()-1;i>=0;--i){
	(*vars)[i]->to_asm(gen);
	gen.write(pop,rax);
	switch(i){
	    case 0 :gen.write(mov,rax,rdi);break;
	    case 1 :gen.write(mov,rax,rsi);break;
	    case 2 :gen.write(mov,rax,rdx);break;
	    case 3 :gen.write(mov,rax,rcx);break;
	    case 4 :gen.write(mov,rax,r8); break;
	    case 5 :gen.write(mov,rax,r9); break;
	    default:gen.write(push,rax);   break;
	}
    }
    gen.write(call,id->name);
    gen.write(add,align,rsp);
    gen.write(push,rax);
}
void uplus::to_asm(code::generator&gen)const
{
    arg->to_asm(gen);
}
void uminus::to_asm(code::generator&gen)const
{
    arg->to_asm(gen);
    gen.write(pop,rax);
    gen.write(mov,rax,rdi);
    gen.write(mov,2,rsi);gen.write(mul,rsi);
    gen.write(sub,rax,rdi);
    gen.write(push,rdi);
}
void preinc::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(arg)->refer(gen);
    gen.write(pop,rax);
    gen.write(add,1,derefer(rax));
    gen.write(push,derefer(rax));
}
void predec::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(arg)->refer(gen);
    gen.write(pop,rax);
    gen.write(sub,1,derefer(rax));
    gen.write(push,derefer(rax));
}
void plus::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(add,rdi,rax);
    gen.write(push,rax);
}
void minus::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(sub,rdi,rax);
    gen.write(push,rax);
}
void multi::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(mul,rdi);
    gen.write(push,rax);
}
void divide::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(mov,0,rdx);
    gen.write(div,rdi);
    gen.write(push,rax);
}
void remain::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(mov,0,rdx);
    gen.write(div,rdi);
    gen.write(push,rdx);
}
void equal::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(cmp,rdi,rax);
    gen.write(sete,al);
    gen.write(movzb,al,rax);
    gen.write(push,rax);
}
void nequal::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(cmp,rdi,rax);
    gen.write(setne,al);
    gen.write(movzb,al,rax);
    gen.write(push,rax);
}
void less::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(cmp,rdi,rax);
    gen.write(setl,al);
    gen.write(movzb,al,rax);
    gen.write(push,rax);
}
void greater::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(cmp,rdi,rax);
    gen.write(setg,al);
    gen.write(movzb,al,rax);
    gen.write(push,rax);
}
void leeq::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(cmp,rdi,rax);
    gen.write(setle,al);
    gen.write(movzb,al,rax);
    gen.write(push,rax);
}
void greq::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(cmp,rdi,rax);
    gen.write(setge,al);
    gen.write(movzb,al,rax);
    gen.write(push,rax);
}
void comma::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    gen.write(pop,rax);
    rarg->to_asm(gen);
}
void assign::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(mov,rdi,derefer(rax));
    gen.write(push,derefer(rax));
}
void plasgn::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(add,rdi,derefer(rax));
    gen.write(push,derefer(rax));
}
void miasgn::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(sub,rdi,derefer(rax));
    gen.write(push,derefer(rax));
}
void muasgn::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(mov,rax,rsi);
    gen.write(mov,derefer(rax),rax);
    gen.write(mul,rdi);
    gen.write(mov,rax,derefer(rsi));
    gen.write(mov,rsi,rax);
    gen.write(push,derefer(rax));
}
void diasgn::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(mov,rax,rsi);
    gen.write(mov,derefer(rax),rax);
    gen.write(mov,0,rdx);
    gen.write(div,rdi);
    gen.write(mov,rax,derefer(rsi));
    gen.write(mov,rsi,rax);
    gen.write(push,derefer(rax));
}
void rmasgn::to_asm(code::generator&gen)const
{
    dynamic_cast<const ident*>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(mov,rax,rsi);
    gen.write(mov,derefer(rax),rax);
    gen.write(mov,0,rdx);
    gen.write(div,rdi);
    gen.write(mov,rdx,derefer(rsi));
    gen.write(mov,rsi,rax);
    gen.write(push,derefer(rax));
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
    id->check(analy);
    for(auto v:*vars)v->check(analy);
}
void unopr::check(semantics::analyzer&analy)const
{
    arg->check(analy);
}
void unopr_l::check(semantics::analyzer&analy)const
{
    unopr::check(analy);
    if(typeid(*arg)!=typeid(ident))throw std::runtime_error("右辺値への操作です");
}
void biopr::check(semantics::analyzer&analy)const
{
    larg->check(analy);
    rarg->check(analy);
}
void biopr_l::check(semantics::analyzer&analy)const
{
    biopr::check(analy);
    if(typeid(*larg)!=typeid(ident))throw std::runtime_error("右辺値への代入です");
}
numeric::numeric(int value)                                         :value(value)         {}
ident  ::ident  (const std::string&name)                            :name(name)           {}
fcall  ::fcall  (const ident*id,const std::vector<const node*>*vars):id(id),vars(vars)    {}
unopr  ::unopr  (const node*arg)                                    :arg(arg)             {}
uplus  ::uplus  (const node*arg)                                    :unopr(arg)           {}
uminus ::uminus (const node*arg)                                    :unopr(arg)           {}
unopr_l::unopr_l(const node*arg)                                    :unopr(arg)           {}
preinc ::preinc (const node*arg)                                    :unopr_l(arg)         {}
predec ::predec (const node*arg)                                    :unopr_l(arg)         {}
biopr  ::biopr  (const node*larg,const node*rarg)                   :larg(larg),rarg(rarg){}
plus   ::plus   (const node*larg,const node*rarg)                   :biopr(larg,rarg)     {}
minus  ::minus  (const node*larg,const node*rarg)                   :biopr(larg,rarg)     {}
multi  ::multi  (const node*larg,const node*rarg)                   :biopr(larg,rarg)     {}
divide ::divide (const node*larg,const node*rarg)                   :biopr(larg,rarg)     {}
remain ::remain (const node*larg,const node*rarg)                   :biopr(larg,rarg)     {}
equal  ::equal  (const node*larg,const node*rarg)                   :biopr(larg,rarg)     {}
nequal ::nequal (const node*larg,const node*rarg)                   :biopr(larg,rarg)     {}
less   ::less   (const node*larg,const node*rarg)                   :biopr(larg,rarg)     {}
greater::greater(const node*larg,const node*rarg)                   :biopr(larg,rarg)     {}
leeq   ::leeq   (const node*larg,const node*rarg)                   :biopr(larg,rarg)     {}
greq   ::greq   (const node*larg,const node*rarg)                   :biopr(larg,rarg)     {}
comma  ::comma  (const node*larg,const node*rarg)                   :biopr(larg,rarg)     {}
biopr_l::biopr_l(const node*larg,const node*rarg)                   :biopr(larg,rarg)     {}
assign ::assign (const node*larg,const node*rarg)                   :biopr_l(larg,rarg)   {}
plasgn ::plasgn (const node*larg,const node*rarg)                   :biopr_l(larg,rarg)   {}
miasgn ::miasgn (const node*larg,const node*rarg)                   :biopr_l(larg,rarg)   {}
muasgn ::muasgn (const node*larg,const node*rarg)                   :biopr_l(larg,rarg)   {}
diasgn ::diasgn (const node*larg,const node*rarg)                   :biopr_l(larg,rarg)   {}
rmasgn ::rmasgn (const node*larg,const node*rarg)                   :biopr_l(larg,rarg)   {}
node   ::~node  ()                                                                        {}
fcall  ::~fcall ()                        {delete id;for(auto v:*vars)delete v;delete vars;}
unopr  ::~unopr ()                                                             {delete arg;}
biopr  ::~biopr ()                                                {delete larg;delete rarg;}
