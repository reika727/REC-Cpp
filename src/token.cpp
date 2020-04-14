#include"token.hpp"
using namespace lexicon;
token::token(int line,int col)
    :line(line),col(col){}
numeric::numeric(int value,int line,int col)
    :token(line,col),value(value){}
identifier::identifier(const std::string&name,int line,int col)
    :token(line,col),name(name){}
symbol::symbol(symbol::SYMBOL sym,int line,int col)
    :token(line,col),sym(sym){}
symbol&symbol::operator=(const symbol&)
{
    return*this;
}
