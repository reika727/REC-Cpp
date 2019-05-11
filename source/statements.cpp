#include"syntax/statements.hpp"
#include"code/generator.hpp"
#include"code/gcfuncs.hpp"
#include"code/assembly/instructions.hpp"
#include"code/assembly/registries.hpp"
using namespace syntax;
using namespace code::assembly;
using code::derefer;
using code::unique_label;
void statement::eval(const code::generator&gen)
{

}
void single::eval(const code::generator&gen)
{
    if(stat){
	stat->to_asm(gen);
	gen.write(pop,rax);
    }
}
void compound::eval(const code::generator&gen)
{
    for(auto s:stats)s->eval(gen);
}
void declare::eval(const code::generator&gen)
{
    for(auto v:vars){
	gen.write(sub,8,rsp);
	gen.set_offset(v.first);
	if(v.second){
	    v.second->to_asm(gen);
	    gen.write(pop,rsi);
	    gen.write(mov,rsi,derefer(rsp));
	}
    }
}
void _if_else_::eval(const code::generator&gen)
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
void _while_::eval(const code::generator&gen)
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
void _for_::eval(const code::generator&gen)
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
statement::~statement(){}
single::single(node*stat):stat(stat){}
_if_else_::_if_else_(single*cond,statement*st1,statement*st2):cond(cond),st1(st1),st2(st2){}
_while_::_while_(single*cond,statement*st):cond(cond),st(st){}
_for_::_for_(single*init,single*cond,single*reinit,statement*st):init(init),cond(cond),reinit(reinit),st(st){}
single::~single(){delete stat;}
compound::~compound(){for(auto s:stats)delete s;}
declare::~declare(){for(auto v:vars)delete v.second;}
_if_else_::~_if_else_(){delete cond;delete st1;delete st2;}
_while_::~_while_(){delete cond;delete st;}
_for_::~_for_(){delete init;delete cond;delete reinit;delete st;}
