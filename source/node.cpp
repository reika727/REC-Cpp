#include"parsing/node.hpp"
using namespace parsing;
node::~node()
{

}
numeric::numeric(int value):value(value)
{

}
numeric::~numeric()
{

}
ident::ident(const std::string&name):name(name)
{

}
ident::~ident()
{

}
unopr::unopr(ND type,node*const arg):type(type),arg(arg)
{

}
unopr::~unopr()
{
    delete arg;
}
biopr::biopr(node*const left,ND type,node*const right):larg(left),type(type),rarg(right)
{

}
biopr::~biopr()
{
    delete larg;
    delete rarg;
}
statement::~statement()
{

}
single::single(node*const _stat)
{
    if(auto sp=dynamic_cast<single*>(_stat);sp!=nullptr&&sp->stat==nullptr){
	stat=nullptr;
    }else{
	stat=_stat;
    }
}
single::~single()
{
    delete stat;
}
bool single::is_nop()
{
    return stat==nullptr;
}
compound::compound()
{
    stats.push_back(nullptr);
}
compound::~compound()
{
    for(auto s:stats)delete s;
}
void compound::push_back(statement*const st)
{
    stats.back()=st;
    stats.push_back(nullptr);
}
_if_::_if_(single*const cond,statement*const st):cond(cond),st(st)
{

}
_if_::~_if_()
{
    delete cond;
    delete st;
}
_else_::_else_(statement*const st):st(st)
{

}
_else_::~_else_()
{
    delete st;
}
_while_::_while_(single*const cond,statement*const st):cond(cond),st(st)
{

}
_while_::~_while_()
{
    delete cond;
    delete st;
}
_for_::_for_(single*const init,single*const cond,single*const reinit,statement*const st):init(init),cond(cond),reinit(reinit),st(st)
{

}
_for_::~_for_()
{
    delete init;
    delete cond;
    delete reinit;
    delete st;
}
