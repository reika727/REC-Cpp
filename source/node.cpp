#include"abstract_syntax_tree/node.hpp"
using namespace abstract_syntax_tree;
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
fcall::fcall(const std::string&name):name(name)
{

}
fcall::~fcall()
{
    for(auto a:args)delete a;
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
single::single(node*const stat):stat(stat)
{

}
single::~single()
{
    delete stat;
}
bool single::is_nop()
{
    return stat==nullptr;
}
compound::~compound()
{
    for(auto s:stats)delete s;
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
