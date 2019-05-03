#include"tokenization/token.hpp"
using namespace tokenization;
token::~token()
{

}
numeric::numeric(int value):value(value)
{

}
ident::ident(const std::string&name):name(name)
{

}
symbol::symbol(TK type):type(type)
{

}
