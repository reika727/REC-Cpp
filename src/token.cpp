#include"token.hpp"
#include"compilation_error.hpp"
#include<algorithm>
using namespace lexicon;
token::token(int line,int col)
    :line(line),col(col){}
token::~token(){}
numeric::numeric(int value,int line,int col)
    :token(line,col),value(value){}
identifier::identifier(const std::string&name,int line,int col)
    :token(line,col),name(name){}
symbol::symbol(symbol::SYMBOL sym,int line,int col)
    :token(line,col),sym(sym){}
token_array::token_array(const std::string&src)
    :src(src),pos(0),line(1),col(1){}
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
void token_array::skip_space_or_comment()
{
    while(pos<src.length()){
        if(std::isspace(src[pos])){
            if(src[pos]=='\n'){
                ++line;
                col=0;
            }
            ++pos;
            ++col;
        }else if(src.substr(pos,2)=="/*"){
            pos+=2;
            int l=line,c=col;
            while(true){
                if(pos>=src.length())
                    throw exception::compilation_error("コメントが閉じられていません",l,c);
                if(src[pos]=='\n'){
                    ++pos;
                    ++line;
                    col=1;
                }else if(src.substr(pos,2)=="*/"){
                    pos+=2;
                    col+=2;
                    break;
                }
            }
        }else{
            break;
        }
    }
}
int token_array::get_line()const noexcept
{
    return line;
}
int token_array::get_column()const noexcept
{
    return col;
}
bool token_array::is_all_read()
{
    skip_space_or_comment();
    return pos>=src.length();
}
std::shared_ptr<const numeric>token_array::consume_numeric()
{
    skip_space_or_comment();
    if(is_all_read()||!std::isdigit(src[pos]))return nullptr;
    size_t sz;
    int num=std::stoi(src.substr(pos),&sz);
    pos+=sz;
    col+=sz;
    return std::make_shared<const numeric>(num,line,col);
}
std::shared_ptr<const identifier>token_array::consume_identifier()
{
    skip_space_or_comment();
    if(is_all_read()||!std::isalpha(src[pos])&&src[pos]!='_')return nullptr;
    auto name=std::string(src.begin()+pos,std::find_if_not(src.begin()+pos,src.end(),[](char c){return std::isalpha(c)||std::isdigit(c)||c=='_';}));
    pos+=name.length();
    col+=name.length();
    return std::make_shared<const identifier>(name,line,col);
}
bool token_array::check_symbol(symbol::SYMBOL sym)
{
    skip_space_or_comment();
    if(is_all_read())return false;
    auto m=symbol::match(src,pos);
    return m.has_value()&&m.value().first==sym;
}
std::shared_ptr<const symbol>token_array::consume_symbol(symbol::SYMBOL sym)
{
    skip_space_or_comment();
    if(is_all_read())return nullptr;
    if(auto m=symbol::match(src,pos);m.has_value()&&m.value().first==sym){
        pos+=m.value().second;
        col+=m.value().second;
        return std::make_shared<const symbol>(m.value().first,line,col);
    }else{
        return nullptr;
    }
}
