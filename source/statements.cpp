#include"syntax/statements.hpp"
using namespace syntax;
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
