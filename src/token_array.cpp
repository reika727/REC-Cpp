#include"lexicon/token_array.hpp"
#include"exception/compilation_error.hpp"
#include<algorithm>
using namespace lexicon;
token::token(TK type,int line,int col)
    :type(type),line(line),col(col){}
token::~token(){}
numeric::numeric(int value,int line,int col)
    :token(TK::NUMERIC,line,col),value(value){}
identifier::identifier(const std::string&name,int line,int col)
    :token(TK::IDENT,line,col),name(name){}
symbol::symbol(TK type,int line,int col)
    :token(type,line,col){}
token_array::token_array(const std::string&src)
{
    int line=1,col=1;
    auto push_symbol=[this,&line,&col](TK sym){
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
            push_symbol(TK::INT);
        else if(check_keyword("void",' ',')'))
            push_symbol(TK::VOID);
        else if(check_keyword("if",'(',' '))
            push_symbol(TK::IF);
        else if(check_keyword("else",'{',';',' '))
            push_symbol(TK::ELSE);
        else if(check_keyword("while",'(',' '))
            push_symbol(TK::WHILE);
        else if(check_keyword("for",'(',' '))
            push_symbol(TK::FOR);
        else if(check_keyword("break",';',' '))
            push_symbol(TK::BREAK);
        else if(check_keyword("continue",';',' '))
            push_symbol(TK::CONTINUE);
        else if(check_keyword("return",' '))
            push_symbol(TK::RETURN);
        else if(src.substr(i,2)=="/*")
            check_comment_closed();
        else if(check_keyword("&&"))
            push_symbol(TK::APAP);
        else if(check_keyword("||"))
            push_symbol(TK::VBVB);
        else if(check_keyword("++"))
            push_symbol(TK::PLPL);
        else if(check_keyword("--"))
            push_symbol(TK::MIMI);
        else if(check_keyword("+="))
            push_symbol(TK::PLEQ);
        else if(check_keyword("-="))
            push_symbol(TK::MIEQ);
        else if(check_keyword("*="))
            push_symbol(TK::ASEQ);
        else if(check_keyword("/="))
            push_symbol(TK::SLEQ);
        else if(check_keyword("%="))
            push_symbol(TK::PEEQ);
        else if(check_keyword("=="))
            push_symbol(TK::EQEQ);
        else if(check_keyword("!="))
            push_symbol(TK::EXEQ);
        else if(check_keyword("<="))
            push_symbol(TK::LEEQ);
        else if(check_keyword(">="))
            push_symbol(TK::GREQ);
        else if(check_keyword("+"))
            push_symbol(TK::PLUS);
        else if(check_keyword("-"))
            push_symbol(TK::MINUS);
        else if(check_keyword("*"))
            push_symbol(TK::ASTER);
        else if(check_keyword("/"))
            push_symbol(TK::SLASH);
        else if(check_keyword("%"))
            push_symbol(TK::PERCENT);
        else if(check_keyword("<"))
            push_symbol(TK::LESS);
        else if(check_keyword(">"))
            push_symbol(TK::GREATER);
        else if(check_keyword("!"))
            push_symbol(TK::EXCLAM);
        else if(check_keyword("="))
            push_symbol(TK::EQUAL);
        else if(check_keyword(","))
            push_symbol(TK::COMMA);
        else if(check_keyword(";"))
            push_symbol(TK::SCOLON);
        else if(check_keyword("("))
            push_symbol(TK::OPARENT);
        else if(check_keyword(")"))
            push_symbol(TK::CPARENT);
        else if(check_keyword("{"))
            push_symbol(TK::OBRACE);
        else if(check_keyword("}"))
            push_symbol(TK::CBRACE);
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
bool token_array::check(TK type)const noexcept
{
    return !is_all_read()&&tokens.front()->type==type;
}
int token_array::get_line()const noexcept
{
    return is_all_read()?-1:tokens.front()->line;
}
int token_array::get_column()const noexcept
{
    return is_all_read()?-1:tokens.front()->col;
}
std::shared_ptr<const token>token_array::consume(TK type)noexcept
{
    decltype(tokens)::value_type ret=nullptr;
    if(check(type)){
        ret=tokens.front();
        tokens.pop();
    }
    return ret;
}
