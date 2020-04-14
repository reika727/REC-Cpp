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
std::optional<std::pair<symbol::SYMBOL,int>>symbol::match(const std::string&str,int pos)
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
        return{{symbol::INT,foo}};
    else if(foo=check_keyword("void",' ',')'))
        return{{symbol::VOID,foo}};
    else if(foo=check_keyword("if",'(',' '))
        return{{symbol::IF,foo}};
    else if(foo=check_keyword("else",'{',';',' '))
        return{{symbol::ELSE,foo}};
    else if(foo=check_keyword("while",'(',' '))
        return{{symbol::WHILE,foo}};
    else if(foo=check_keyword("for",'(',' '))
        return{{symbol::FOR,foo}};
    else if(foo=check_keyword("break",';',' '))
        return{{symbol::BREAK,foo}};
    else if(foo=check_keyword("continue",';',' '))
        return{{symbol::CONTINUE,foo}};
    else if(foo=check_keyword("return",' '))
        return{{symbol::RETURN,foo}};
    else if(foo=check_keyword("&&"))
        return{{symbol::APAP,foo}};
    else if(foo=check_keyword("||"))
        return{{symbol::VBVB,foo}};
    else if(foo=check_keyword("++"))
        return{{symbol::PLPL,foo}};
    else if(foo=check_keyword("--"))
        return{{symbol::MIMI,foo}};
    else if(foo=check_keyword("+="))
        return{{symbol::PLEQ,foo}};
    else if(foo=check_keyword("-="))
        return{{symbol::MIEQ,foo}};
    else if(foo=check_keyword("*="))
        return{{symbol::ASEQ,foo}};
    else if(foo=check_keyword("/="))
        return{{symbol::SLEQ,foo}};
    else if(foo=check_keyword("%="))
        return{{symbol::PEEQ,foo}};
    else if(foo=check_keyword("=="))
        return{{symbol::EQEQ,foo}};
    else if(foo=check_keyword("!="))
        return{{symbol::EXEQ,foo}};
    else if(foo=check_keyword("<="))
        return{{symbol::LEEQ,foo}};
    else if(foo=check_keyword(">="))
        return{{symbol::GREQ,foo}};
    else if(foo=check_keyword("+"))
        return{{symbol::PLUS,foo}};
    else if(foo=check_keyword("-"))
        return{{symbol::MINUS,foo}};
    else if(foo=check_keyword("*"))
        return{{symbol::ASTER,foo}};
    else if(foo=check_keyword("/"))
        return{{symbol::SLASH,foo}};
    else if(foo=check_keyword("%"))
        return{{symbol::PERCENT,foo}};
    else if(foo=check_keyword("<"))
        return{{symbol::LESS,foo}};
    else if(foo=check_keyword(">"))
        return{{symbol::GREATER,foo}};
    else if(foo=check_keyword("!"))
        return{{symbol::EXCLAM,foo}};
    else if(foo=check_keyword("="))
        return{{symbol::EQUAL,foo}};
    else if(foo=check_keyword(","))
        return{{symbol::COMMA,foo}};
    else if(foo=check_keyword(";"))
        return{{symbol::SCOLON,foo}};
    else if(foo=check_keyword("("))
        return{{symbol::OPARENT,foo}};
    else if(foo=check_keyword(")"))
        return{{symbol::CPARENT,foo}};
    else if(foo=check_keyword("{"))
        return{{symbol::OBRACE,foo}};
    else if(foo=check_keyword("}"))
        return{{symbol::CBRACE,foo}};
    else
        return std::nullopt;
}
