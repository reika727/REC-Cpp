#include"syntax/statements.hpp"
#include"semantics/analyzer.hpp"
#include"code/generator.hpp"
#include"code/gcfuncs.hpp"
#include"code/assembly/instructions.hpp"
#include"code/assembly/registries.hpp"
#include<stdexcept>
using namespace syntax;
using code::address;
using code::unique_label;
void single::eval(code::generator&gen)const
{
    if(stat){
	stat->to_asm(gen);
	gen.write(pop,rax);
    }
}
void compound::eval(code::generator&gen)const
{
    for(auto s:*stats)s->eval(gen);
}
void declare::eval(code::generator&gen)const
{
    for(auto v:*vars){
	gen.write(sub,8,rsp);
	gen.set_offset(v.first);
	if(v.second){
	    v.second->to_asm(gen);
	    gen.write(pop,rax);
	    gen.write(mov,rax,address(rsp));
	}
    }
}
void _if_else_::eval(code::generator&gen)const
{
    std::string el=unique_label(".Lelse");
    std::string end=unique_label(".Lend");
    cond->eval(gen);
    gen.write(cmp,0,rax);
    gen.write(je,el);
    st1->eval(gen);
    gen.write(jmp,end);
    gen.write(el+':');
    st2->eval(gen);
    gen.write(end+':');
}
void _while_::eval(code::generator&gen)const
{
    std::string beg=unique_label(".Lbegin");
    std::string end=unique_label(".Lend");
    gen.write(beg+':');
    cond->eval(gen);
    gen.write(cmp,0,rax);
    gen.write(je,end);
    st->eval(gen);
    gen.write(jmp,beg);
    gen.write(end+':');
}
void _for_::eval(code::generator&gen)const
{
    std::string beg=unique_label(".Lbegin");
    std::string end=unique_label(".Lend");
    init->eval(gen);
    gen.write(beg+':');
    gen.write(mov,1,rax);
    cond->eval(gen);
    gen.write(cmp,0,rax);
    gen.write(je,end);
    st->eval(gen);
    reinit->eval(gen);
    gen.write(jmp,beg);
    gen.write(end+':');
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
single   ::single    (const node*stat)                                                           :stat(stat)                                 {}
compound ::compound  (const std::vector<const statement*>*stats)                                 :stats(stats)                               {}
declare  ::declare   (const std::vector<std::pair<std::string,const node*>>*vars)                :vars(vars)                                 {}
_if_else_::_if_else_ (const single*cond,const statement*st1,const statement*st2)                 :cond(cond),st1(st1),st2(st2)               {}
_while_  ::_while_   (const single*cond,const statement*st)                                      :cond(cond),st(st)                          {}
_for_    ::_for_     (const single*init,const single*cond,const single*reinit,const statement*st):init(init),cond(cond),reinit(reinit),st(st){}
statement::~statement()                                                                                                                      {}
single   ::~single   ()                                                                                                          {delete stat;}
compound ::~compound ()                                                                              {for(auto s:*stats)delete s;delete stats;}
declare  ::~declare  ()                                                                         {for(auto v:*vars)delete v.second;delete vars;}
_if_else_::~_if_else_()                                                                                    {delete cond;delete st1;delete st2;}
_while_  ::~_while_  ()                                                                                                {delete cond;delete st;}
_for_    ::~_for_    ()                                                                      {delete init;delete cond;delete reinit;delete st;}
