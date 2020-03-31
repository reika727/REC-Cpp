#include"lexicon/token_array.hpp"
#include"exception/compilation_error.hpp"
#include<algorithm>
using namespace lexicon;
token_array::token_array(const std::string&s)
{
    int line=1,col=1;
    auto push_back_symbol=[this,&line,&col](TK sym){
        tokens.push_back(std::make_shared<const symbol>(sym,line,col));
    };
    auto push_back_number=[this,&line,&col](int num){
        tokens.push_back(std::make_shared<const numeric>(num,line,col));
    };
    auto push_back_identifier=[this,&line,&col](const std::string&name){
        tokens.push_back(std::make_shared<const identifier>(name,line,col));
    };
    for(int i=0;i<s.length();){
        if(isspace(s[i])){
            if(s[i]=='\n'){
                ++line;
                col=0;
            }
            ++i;
            ++col;
            continue;
        }
        auto check_keyword=[&col,s,&i](const std::string&token,auto...follow){
            if(s.substr(i,token.length())!=token)return false;
            if(sizeof...(follow)!=0){
                if(i+token.length()>=s.length())return false;
                if(((follow!=s[i+token.length()])&&...))return false;
            }
            i+=token.length();
            col+=token.length();
            return true;
        };
        auto check_comment_closed=[&line,&col,s,&i](){
            int l=line,c=col;
            for(;i<s.length();++i){
                if(s[i]=='\n'){
                    ++line;
                    col=1;
                    continue;
                }
                if(s.substr(i,2)=="*/"){
                    i+=2;
                    col+=2;
                    return;
                }
            }
            throw exception::compilation_error("コメントが閉じられていません",l,c);
        };
        auto get_numeric_literal=[&col,s,&i](){
            size_t sz;
            int ret=std::stoi(s.substr(i),&sz);
            i+=sz;
            col+=sz;
            return ret;
        };
        auto get_identifier=[&col,s,&i](){
            auto beg=s.begin()+i;
            auto ret=std::string(beg,find_if_not(beg,s.end(),[](char c){return isalpha(c)||isdigit(c)||c=='_';}));
            i+=ret.length();
            col+=ret.length();
            return ret;
        };
        if(check_keyword("int",' '))
            push_back_symbol(TK::INT);
        else if(check_keyword("void",' ',')'))
            push_back_symbol(TK::VOID);
        else if(check_keyword("if",'(',' '))
            push_back_symbol(TK::IF);
        else if(check_keyword("else",'{',';',' '))
            push_back_symbol(TK::ELSE);
        else if(check_keyword("while",'(',' '))
            push_back_symbol(TK::WHILE);
        else if(check_keyword("for",'(',' '))
            push_back_symbol(TK::FOR);
        else if(check_keyword("break",';',' '))
            push_back_symbol(TK::BREAK);
        else if(check_keyword("continue",';',' '))
            push_back_symbol(TK::CONTINUE);
        else if(check_keyword("return",' '))
            push_back_symbol(TK::RETURN);
        else if(s.substr(i,2)=="/*")
            check_comment_closed();
        else if(check_keyword("&&"))
            push_back_symbol(TK::APAP);
        else if(check_keyword("||"))
            push_back_symbol(TK::VBVB);
        else if(check_keyword("++"))
            push_back_symbol(TK::PLPL);
        else if(check_keyword("--"))
            push_back_symbol(TK::MIMI);
        else if(check_keyword("+="))
            push_back_symbol(TK::PLEQ);
        else if(check_keyword("-="))
            push_back_symbol(TK::MIEQ);
        else if(check_keyword("*="))
            push_back_symbol(TK::ASEQ);
        else if(check_keyword("/="))
            push_back_symbol(TK::SLEQ);
        else if(check_keyword("%="))
            push_back_symbol(TK::PEEQ);
        else if(check_keyword("=="))
            push_back_symbol(TK::EQEQ);
        else if(check_keyword("!="))
            push_back_symbol(TK::EXEQ);
        else if(check_keyword("<="))
            push_back_symbol(TK::LEEQ);
        else if(check_keyword(">="))
            push_back_symbol(TK::GREQ);
        else if(check_keyword("+"))
            push_back_symbol(TK::PLUS);
        else if(check_keyword("-"))
            push_back_symbol(TK::MINUS);
        else if(check_keyword("*"))
            push_back_symbol(TK::ASTER);
        else if(check_keyword("/"))
            push_back_symbol(TK::SLASH);
        else if(check_keyword("%"))
            push_back_symbol(TK::PERCENT);
        else if(check_keyword("<"))
            push_back_symbol(TK::LESS);
        else if(check_keyword(">"))
            push_back_symbol(TK::GREATER);
        else if(check_keyword("!"))
            push_back_symbol(TK::EXCLAM);
        else if(check_keyword("="))
            push_back_symbol(TK::EQUAL);
        else if(check_keyword(","))
            push_back_symbol(TK::COMMA);
        else if(check_keyword(";"))
            push_back_symbol(TK::SCOLON);
        else if(check_keyword("("))
            push_back_symbol(TK::OPARENT);
        else if(check_keyword(")"))
            push_back_symbol(TK::CPARENT);
        else if(check_keyword("{"))
            push_back_symbol(TK::OBRACE);
        else if(check_keyword("}"))
            push_back_symbol(TK::CBRACE);
        else if(isdigit(s[i]))
            push_back_number(get_numeric_literal());
        else if(isalpha(s[i])||s[i]=='_')
            push_back_identifier(get_identifier());
        else
            throw exception::compilation_error("認識できないトークンが含まれます",line,col);
    }
    itr=tokens.begin();
}
bool token_array::is_all_read()const noexcept
{
    return itr==tokens.end();
}
bool token_array::check(TK type)const noexcept
{
    return !is_all_read()&&(*itr)->type==type;
}
int token_array::get_line()const noexcept
{
    return is_all_read()?-1:(*itr)->line;
}
int token_array::get_column()const noexcept
{
    return is_all_read()?-1:(*itr)->col;
}
std::shared_ptr<const token>token_array::consume(TK type)noexcept
{
    return check(type)?*itr++:nullptr;
}
