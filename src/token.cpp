#include"lexicon/token.hpp"
using namespace lexicon;
token::token(TK type)
    :type(type){}
numeric::numeric(int value)
    :token(TK::NUMERIC),value(value){}
ident::ident(const std::string&name)
    :token(TK::IDENT),name(name){}
symbol::symbol(TK type)
    :token(type){}
