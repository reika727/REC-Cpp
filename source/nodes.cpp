#include"abstract_syntax_tree/nodes.hpp"
using namespace abstract_syntax_tree;
node::~node()
{

}
numeric::numeric(int value):value(value)
{

}
ident::ident(const std::string&name):name(name)
{

}
fcall::fcall(const std::string&name):name(name)
{

}
fcall::~fcall()
{
    for(auto a:args)delete a;
}
unopr::unopr(ND type,node*arg):type(type),arg(arg)
{

}
unopr::~unopr()
{
    delete arg;
}
biopr::biopr(node*left,ND type,node*right):larg(left),type(type),rarg(right)
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
single::single(node*stat):stat(stat)
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
declare::~declare()
{
    for(auto v:vars)delete v.second;
}
_if_::_if_(single*cond,statement*st):cond(cond),st(st)
{

}
_if_::~_if_()
{
    delete cond;
    delete st;
}
_else_::_else_(statement*st):st(st)
{

}
_else_::~_else_()
{
    delete st;
}
_while_::_while_(single*cond,statement*st):cond(cond),st(st)
{

}
_while_::~_while_()
{
    delete cond;
    delete st;
}
_for_::_for_(single*init,single*cond,single*reinit,statement*st):init(init),cond(cond),reinit(reinit),st(st)
{

}
_for_::~_for_()
{
    delete init;
    delete cond;
    delete reinit;
    delete st;
}
