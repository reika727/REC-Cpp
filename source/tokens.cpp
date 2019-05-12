#include"lexicon/tokens.hpp"
using namespace lexicon;
numeric::numeric(int value)             :value(value){}
ident  ::ident  (const std::string&name):name(name)  {}
symbol ::symbol (TK type)               :type(type)  {}
token  ::~token ()                                   {}
