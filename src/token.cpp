#include"token.hpp"
using namespace lexicon;
token::token(int line,int col)
    :line(line),col(col){}
numeric::numeric(int value,int line,int col)
    :token(line,col),value(value){}
identifier::identifier(const std::string&name,int line,int col)
    :token(line,col),name(name){}
symbol::symbol(symbol::symid id,int line,int col)
    :token(line,col),id(id){}
symbol&symbol::operator=(const symbol&)
{
    return*this;
}
std::optional<std::pair<symbol::symid,int>>symbol::match(const std::string&str,int pos)
{
    auto check_keyword=[str,pos](const std::string&token,auto...follow)->size_t{
        if(str.substr(pos,token.length())!=token)return 0;
        if(sizeof...(follow)!=0){
            if(pos+token.length()>=str.length())return 0;
            if(((follow!=str[pos+token.length()])&&...))return 0;
        }
        return token.length();
    };
    if(auto foo=check_keyword("int",' '))
        return{{symbol::symid::INT,foo}};
    else if((foo=check_keyword("void",' ',')')))
        return{{symbol::symid::VOID,foo}};
    else if((foo=check_keyword("if",'(',' ')))
        return{{symbol::symid::IF,foo}};
    else if((foo=check_keyword("else",'{',';',' ')))
        return{{symbol::symid::ELSE,foo}};
    else if((foo=check_keyword("while",'(',' ')))
        return{{symbol::symid::WHILE,foo}};
    else if((foo=check_keyword("for",'(',' ')))
        return{{symbol::symid::FOR,foo}};
    else if((foo=check_keyword("break",';',' ')))
        return{{symbol::symid::BREAK,foo}};
    else if((foo=check_keyword("continue",';',' ')))
        return{{symbol::symid::CONTINUE,foo}};
    else if((foo=check_keyword("return",' ')))
        return{{symbol::symid::RETURN,foo}};
    else if((foo=check_keyword("&&")))
        return{{symbol::symid::APAP,foo}};
    else if((foo=check_keyword("||")))
        return{{symbol::symid::VBVB,foo}};
    else if((foo=check_keyword("++")))
        return{{symbol::symid::PLPL,foo}};
    else if((foo=check_keyword("--")))
        return{{symbol::symid::MIMI,foo}};
    else if((foo=check_keyword("+=")))
        return{{symbol::symid::PLEQ,foo}};
    else if((foo=check_keyword("-=")))
        return{{symbol::symid::MIEQ,foo}};
    else if((foo=check_keyword("*=")))
        return{{symbol::symid::ASEQ,foo}};
    else if((foo=check_keyword("/=")))
        return{{symbol::symid::SLEQ,foo}};
    else if((foo=check_keyword("%=")))
        return{{symbol::symid::PEEQ,foo}};
    else if((foo=check_keyword("==")))
        return{{symbol::symid::EQEQ,foo}};
    else if((foo=check_keyword("!=")))
        return{{symbol::symid::EXEQ,foo}};
    else if((foo=check_keyword("<=")))
        return{{symbol::symid::LEEQ,foo}};
    else if((foo=check_keyword(">=")))
        return{{symbol::symid::GREQ,foo}};
    else if((foo=check_keyword("+")))
        return{{symbol::symid::PLUS,foo}};
    else if((foo=check_keyword("-")))
        return{{symbol::symid::MINUS,foo}};
    else if((foo=check_keyword("*")))
        return{{symbol::symid::ASTER,foo}};
    else if((foo=check_keyword("/")))
        return{{symbol::symid::SLASH,foo}};
    else if((foo=check_keyword("%")))
        return{{symbol::symid::PERCENT,foo}};
    else if((foo=check_keyword("<")))
        return{{symbol::symid::LESS,foo}};
    else if((foo=check_keyword(">")))
        return{{symbol::symid::GREATER,foo}};
    else if((foo=check_keyword("!")))
        return{{symbol::symid::EXCLAM,foo}};
    else if((foo=check_keyword("=")))
        return{{symbol::symid::EQUAL,foo}};
    else if((foo=check_keyword(",")))
        return{{symbol::symid::COMMA,foo}};
    else if((foo=check_keyword(";")))
        return{{symbol::symid::SCOLON,foo}};
    else if((foo=check_keyword("(")))
        return{{symbol::symid::OPARENT,foo}};
    else if((foo=check_keyword(")")))
        return{{symbol::symid::CPARENT,foo}};
    else if((foo=check_keyword("{")))
        return{{symbol::symid::OBRACE,foo}};
    else if((foo=check_keyword("}")))
        return{{symbol::symid::CBRACE,foo}};
    else
        return std::nullopt;
}
