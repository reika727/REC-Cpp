#include"lexicon/token.hpp"
using namespace lexicon;
token::token(TK type,int line,int col)
    :type(type),line(line),col(col){}
numeric::numeric(int value,int line,int col)
    :token(TK::NUMERIC,line,col),value(value){}
identifier::identifier(const std::string&name,int line,int col)
    :token(TK::IDENT,line,col),name(name){}
symbol::symbol(TK type,int line,int col)
    :token(type,line,col){}
token::~token(){}
