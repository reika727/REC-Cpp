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
biopr::biopr(node*larg,ND type,node*rarg):larg(larg),type(type),rarg(rarg)
{

}
biopr::~biopr()
{
    delete larg;
    delete rarg;
}
