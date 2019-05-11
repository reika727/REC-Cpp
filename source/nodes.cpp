#include"syntax/nodes.hpp"
#include"code/generator.hpp"
#include"code/gcfuncs.hpp"
#include"code/assembly/instructions.hpp"
#include"code/assembly/registries.hpp"
using namespace syntax;
using namespace code::assembly;
using code::derefer;
void numeric::to_asm(const code::generator&gen)const
{
    gen.write(push,value);
}
void ident::to_asm(const code::generator&gen)const
{
    gen.write(mov,rbp,rax);
    gen.write(sub,gen.offset(name),rax);
    gen.write(push,derefer(rax));
}
void ident::refer(const code::generator&gen)const
{
    gen.write(mov,rbp,rax);
    gen.write(sub,gen.offset(name),rax);
    gen.write(push,rax);
}
void uplus::to_asm(const code::generator&gen)const
{
    arg->to_asm(gen);
}
void uminus::to_asm(const code::generator&gen)const
{
    arg->to_asm(gen);
    gen.write(pop,rax);
    gen.write(mov,rax,rdi);
    gen.write(mov,2,rsi);gen.write(mul,rsi);
    gen.write(sub,rax,rdi);
    gen.write(push,rdi);
}
void preinc::to_asm(const code::generator&gen)const
{
    dynamic_cast<const ident*const>(arg)->refer(gen);
    gen.write(pop,rax);
    gen.write(add,1,derefer(rax));
    gen.write(push,derefer(rax));
}
void predec::to_asm(const code::generator&gen)const
{
    dynamic_cast<const ident*const>(arg)->refer(gen);
    gen.write(pop,rax);
    gen.write(sub,1,derefer(rax));
    gen.write(push,derefer(rax));
}
void plus::to_asm(const code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(add,rdi,rax);
    gen.write(push,rax);
}
void minus::to_asm(const code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(sub,rdi,rax);
    gen.write(push,rax);
}
void multi::to_asm(const code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(mul,rdi);
    gen.write(push,rax);
}
void divide::to_asm(const code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(mov,0,rdx);
    gen.write(div,rdi);
    gen.write(push,rax);
}
void remain::to_asm(const code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(mov,0,rdx);
    gen.write(div,rdi);
    gen.write(push,rdx);
}
void equal::to_asm(const code::generator&gen)const
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
void nequal::to_asm(const code::generator&gen)const
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
void less::to_asm(const code::generator&gen)const
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
void greater::to_asm(const code::generator&gen)const
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
void leeq::to_asm(const code::generator&gen)const
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
void greq::to_asm(const code::generator&gen)const
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
void assign::to_asm(const code::generator&gen)const
{
    dynamic_cast<const ident*const>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(mov,rdi,derefer(rax));
    gen.write(push,derefer(rax));
}
void plasgn::to_asm(const code::generator&gen)const
{
    dynamic_cast<const ident*const>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(add,rdi,derefer(rax));
    gen.write(push,derefer(rax));
}
void miasgn::to_asm(const code::generator&gen)const
{
    dynamic_cast<const ident*const>(larg)->refer(gen);
    rarg->to_asm(gen);
    gen.write(pop,rdi);
    gen.write(pop,rax);
    gen.write(sub,rdi,derefer(rax));
    gen.write(push,derefer(rax));
}
void muasgn::to_asm(const code::generator&gen)const
{
    dynamic_cast<const ident*const>(larg)->refer(gen);
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
void diasgn::to_asm(const code::generator&gen)const
{
    dynamic_cast<const ident*const>(larg)->refer(gen);
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
void rmasgn::to_asm(const code::generator&gen)const
{
    dynamic_cast<const ident*const>(larg)->refer(gen);
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
numeric::numeric(int value)             :value(value)         {}
ident  ::ident  (const std::string&name):name(name)           {}
unopr  ::unopr  (node*arg)              :arg(arg)             {}
uplus  ::uplus  (node*arg):unopr(arg)                         {}
uminus ::uminus (node*arg)              :unopr(arg)           {}
preinc ::preinc (node*arg)              :unopr(arg)           {}
predec ::predec (node*arg)              :unopr(arg)           {}
biopr  ::biopr  (node*larg,node*rarg)   :larg(larg),rarg(rarg){}
plus   ::plus   (node*larg,node*rarg)   :biopr(larg,rarg)     {}
minus  ::minus  (node*larg,node*rarg)   :biopr(larg,rarg)     {}
multi  ::multi  (node*larg,node*rarg)   :biopr(larg,rarg)     {}
divide ::divide (node*larg,node*rarg)   :biopr(larg,rarg)     {}
remain ::remain (node*larg,node*rarg)   :biopr(larg,rarg)     {}
equal  ::equal  (node*larg,node*rarg)   :biopr(larg,rarg)     {}
nequal ::nequal (node*larg,node*rarg)   :biopr(larg,rarg)     {}
less   ::less   (node*larg,node*rarg)   :biopr(larg,rarg)     {}
greater::greater(node*larg,node*rarg)   :biopr(larg,rarg)     {}
leeq   ::leeq   (node*larg,node*rarg)   :biopr(larg,rarg)     {}
greq   ::greq   (node*larg,node*rarg)   :biopr(larg,rarg)     {}
assign ::assign (node*larg,node*rarg)   :biopr(larg,rarg)     {}
plasgn ::plasgn (node*larg,node*rarg)   :biopr(larg,rarg)     {}
miasgn ::miasgn (node*larg,node*rarg)   :biopr(larg,rarg)     {}
muasgn ::muasgn (node*larg,node*rarg)   :biopr(larg,rarg)     {}
diasgn ::diasgn (node*larg,node*rarg)   :biopr(larg,rarg)     {}
rmasgn ::rmasgn (node*larg,node*rarg)   :biopr(larg,rarg)     {}
node   ::~node  ()                                            {}
unopr  ::~unopr ()                                            {delete arg;}
biopr  ::~biopr ()                                            {delete larg;delete rarg;}
