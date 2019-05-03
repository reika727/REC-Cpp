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
single::single(node*const stat):stat(stat)
{

}
single::~single()
{
    delete stat;
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
