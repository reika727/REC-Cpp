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
{
    int line=1,col=1;
    auto push_symbol=[this,&line,&col](symbol::SYMBOL sym){
        tokens.push(std::make_shared<const symbol>(sym,line,col));
    };
    auto push_number=[this,&line,&col](int num){
        tokens.push(std::make_shared<const numeric>(num,line,col));
    };
    auto push_identifier=[this,&line,&col](const std::string&name){
        tokens.push(std::make_shared<const identifier>(name,line,col));
    };
    for(int i=0;i<src.length();){
        if(isspace(src[i])){
            if(src[i]=='\n'){
                ++line;
                col=0;
            }
            ++i;
            ++col;
            continue;
        }
        auto check_keyword=[&col,src,&i](const std::string&token,auto...follow){
            if(src.substr(i,token.length())!=token)return false;
            if(sizeof...(follow)!=0){
                if(i+token.length()>=src.length())return false;
                if(((follow!=src[i+token.length()])&&...))return false;
            }
            i+=token.length();
            col+=token.length();
            return true;
        };
        auto check_comment_closed=[&line,&col,src,&i](){
            int l=line,c=col;
            for(;i<src.length();++i){
                if(src[i]=='\n'){
                    ++line;
                    col=1;
                    continue;
                }
                if(src.substr(i,2)=="*/"){
                    i+=2;
                    col+=2;
                    return;
                }
            }
            throw exception::compilation_error("コメントが閉じられていません",l,c);
        };
        auto get_numeric_literal=[&col,src,&i](){
            size_t sz;
            int ret=std::stoi(src.substr(i),&sz);
            i+=sz;
            col+=sz;
            return ret;
        };
        auto get_identifier=[&col,src,&i](){
            auto beg=src.begin()+i;
            auto ret=std::string(beg,find_if_not(beg,src.end(),[](char c){return isalpha(c)||isdigit(c)||c=='_';}));
            i+=ret.length();
            col+=ret.length();
            return ret;
        };
        if(check_keyword("int",' '))
            push_symbol(symbol::INT);
        else if(check_keyword("void",' ',')'))
            push_symbol(symbol::VOID);
        else if(check_keyword("if",'(',' '))
            push_symbol(symbol::IF);
        else if(check_keyword("else",'{',';',' '))
            push_symbol(symbol::ELSE);
        else if(check_keyword("while",'(',' '))
            push_symbol(symbol::WHILE);
        else if(check_keyword("for",'(',' '))
            push_symbol(symbol::FOR);
        else if(check_keyword("break",';',' '))
            push_symbol(symbol::BREAK);
        else if(check_keyword("continue",';',' '))
            push_symbol(symbol::CONTINUE);
        else if(check_keyword("return",' '))
            push_symbol(symbol::RETURN);
        else if(src.substr(i,2)=="/*")
            check_comment_closed();
        else if(check_keyword("&&"))
            push_symbol(symbol::APAP);
        else if(check_keyword("||"))
            push_symbol(symbol::VBVB);
        else if(check_keyword("++"))
            push_symbol(symbol::PLPL);
        else if(check_keyword("--"))
            push_symbol(symbol::MIMI);
        else if(check_keyword("+="))
            push_symbol(symbol::PLEQ);
        else if(check_keyword("-="))
            push_symbol(symbol::MIEQ);
        else if(check_keyword("*="))
            push_symbol(symbol::ASEQ);
        else if(check_keyword("/="))
            push_symbol(symbol::SLEQ);
        else if(check_keyword("%="))
            push_symbol(symbol::PEEQ);
        else if(check_keyword("=="))
            push_symbol(symbol::EQEQ);
        else if(check_keyword("!="))
            push_symbol(symbol::EXEQ);
        else if(check_keyword("<="))
            push_symbol(symbol::LEEQ);
        else if(check_keyword(">="))
            push_symbol(symbol::GREQ);
        else if(check_keyword("+"))
            push_symbol(symbol::PLUS);
        else if(check_keyword("-"))
            push_symbol(symbol::MINUS);
        else if(check_keyword("*"))
            push_symbol(symbol::ASTER);
        else if(check_keyword("/"))
            push_symbol(symbol::SLASH);
        else if(check_keyword("%"))
            push_symbol(symbol::PERCENT);
        else if(check_keyword("<"))
            push_symbol(symbol::LESS);
        else if(check_keyword(">"))
            push_symbol(symbol::GREATER);
        else if(check_keyword("!"))
            push_symbol(symbol::EXCLAM);
        else if(check_keyword("="))
            push_symbol(symbol::EQUAL);
        else if(check_keyword(","))
            push_symbol(symbol::COMMA);
        else if(check_keyword(";"))
            push_symbol(symbol::SCOLON);
        else if(check_keyword("("))
            push_symbol(symbol::OPARENT);
        else if(check_keyword(")"))
            push_symbol(symbol::CPARENT);
        else if(check_keyword("{"))
            push_symbol(symbol::OBRACE);
        else if(check_keyword("}"))
            push_symbol(symbol::CBRACE);
        else if(isdigit(src[i]))
            push_number(get_numeric_literal());
        else if(isalpha(src[i])||src[i]=='_')
            push_identifier(get_identifier());
        else
            throw exception::compilation_error("認識できないトークンが含まれます",line,col);
    }
}
bool token_array::is_all_read()const noexcept
{
    return tokens.empty();
}
int token_array::get_line()const noexcept
{
    return is_all_read()?-1:tokens.front()->line;
}
int token_array::get_column()const noexcept
{
    return is_all_read()?-1:tokens.front()->col;
}
bool token_array::check_symbol(symbol::SYMBOL sym)const noexcept
{
    if(is_all_read())return false;
    if(auto foo=std::dynamic_pointer_cast<const symbol>(tokens.front()))
        return foo->sym==sym;
    else
        return false;
}
std::shared_ptr<const numeric>token_array::consume_numeric()noexcept
{
    if(is_all_read())return nullptr;
    auto ret=std::dynamic_pointer_cast<const numeric>(tokens.front());
    if(ret)tokens.pop();
    return ret;
}
std::shared_ptr<const identifier>token_array::consume_identifier()noexcept
{
    if(is_all_read())return nullptr;
    auto ret=std::dynamic_pointer_cast<const identifier>(tokens.front());
    if(ret)tokens.pop();
    return ret;
}
std::shared_ptr<const symbol>token_array::consume_symbol(symbol::SYMBOL sym)noexcept
{
    std::shared_ptr<const symbol>ret=nullptr;
    if(check_symbol(sym)){
        ret=std::dynamic_pointer_cast<const symbol>(tokens.front());
        tokens.pop();
    }
    return ret;
}
