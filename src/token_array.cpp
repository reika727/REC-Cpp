#include"lexicon/token_array.hpp"
#include<algorithm>
#include<initializer_list>
#include<stdexcept>
using namespace lexicon;
token_array::token_array(const std::string&s)
{
    for(int i=0;i<s.length();){
        bool token_detected=false;
        auto check_keyword=[s,&i,&token_detected](const std::string&token,auto...follow){
            if(s.substr(i,token.length())!=token)return false;
            if(sizeof...(follow)!=0){
                bool match=false;
                for(auto&&f:std::initializer_list<char>{follow...})match|=i+token.length()<s.length()&&s[i+token.length()]==f;
                if(!match)return false;
            }
            i+=token.length();
            token_detected=true;
            return true;
        };
        auto check_comment_closed=[s,&i](){
            while(true){
                if(++i>=s.length())throw std::runtime_error("コメントが閉じられていません");
                else if(s.substr(i,2)=="*/"&&(i+=2))break;
            }
        };
        auto get_numeric_literal=[s,&i,&token_detected](){
            size_t sz;
            int ret=std::stoi(s.substr(i),&sz);
            i+=sz;
            token_detected=true;
            return ret;
        };
        auto get_identifier=[s,&i,&token_detected](){
            auto beg=s.begin()+i;
            auto ret=std::string(beg,find_if_not(beg,s.end(),[](char c){return isalpha(c)||isdigit(c)||c=='_';}));
            i+=ret.length();
            token_detected=true;
            return ret;
        };
        if(check_keyword("int",' '))               tv.push_back(new symbol(TK::INT));
        if(check_keyword("if",'(',' '))            tv.push_back(new symbol(TK::IF));
        if(check_keyword("else",'{',';',' '))      tv.push_back(new symbol(TK::ELSE));
        if(check_keyword("while",'(',' '))         tv.push_back(new symbol(TK::WHILE));
        if(check_keyword("for",'(',' '))           tv.push_back(new symbol(TK::FOR));
        if(check_keyword("break",';',' '))         tv.push_back(new symbol(TK::BREAK));
        if(check_keyword("continue",';',' '))      tv.push_back(new symbol(TK::CONTINUE));
        if(check_keyword("return",' '))            tv.push_back(new symbol(TK::RETURN));
        if(s.substr(i,2)=="/*")                    check_comment_closed();
        if(check_keyword("&&"))                    tv.push_back(new symbol(TK::APAP));
        if(check_keyword("||"))                    tv.push_back(new symbol(TK::VBVB));
        if(check_keyword("++"))                    tv.push_back(new symbol(TK::PLPL));
        if(check_keyword("--"))                    tv.push_back(new symbol(TK::MIMI));
        if(check_keyword("+="))                    tv.push_back(new symbol(TK::PLEQ));
        if(check_keyword("-="))                    tv.push_back(new symbol(TK::MIEQ));
        if(check_keyword("*="))                    tv.push_back(new symbol(TK::ASEQ));
        if(check_keyword("/="))                    tv.push_back(new symbol(TK::SLEQ));
        if(check_keyword("%="))                    tv.push_back(new symbol(TK::PEEQ));
        if(check_keyword("=="))                    tv.push_back(new symbol(TK::EQEQ));
        if(check_keyword("!="))                    tv.push_back(new symbol(TK::EXEQ));
        if(check_keyword("<="))                    tv.push_back(new symbol(TK::LEEQ));
        if(check_keyword(">="))                    tv.push_back(new symbol(TK::GREQ));
        if(check_keyword("+"))                     tv.push_back(new symbol(TK::PLUS));
        if(check_keyword("-"))                     tv.push_back(new symbol(TK::MINUS));
        if(check_keyword("*"))                     tv.push_back(new symbol(TK::ASTER));
        if(check_keyword("/"))                     tv.push_back(new symbol(TK::SLASH));
        if(check_keyword("%"))                     tv.push_back(new symbol(TK::PERCENT));
        if(check_keyword("<"))                     tv.push_back(new symbol(TK::LESS));
        if(check_keyword(">"))                     tv.push_back(new symbol(TK::GREATER));
        if(check_keyword("!"))                     tv.push_back(new symbol(TK::EXCLAM));
        if(check_keyword("="))                     tv.push_back(new symbol(TK::EQUAL));
        if(check_keyword(","))                     tv.push_back(new symbol(TK::COMMA));
        if(check_keyword(";"))                     tv.push_back(new symbol(TK::SCOLON));
        if(check_keyword("("))                     tv.push_back(new symbol(TK::OPARENT));
        if(check_keyword(")"))                     tv.push_back(new symbol(TK::CPARENT));
        if(check_keyword("{"))                     tv.push_back(new symbol(TK::OBRACE));
        if(check_keyword("}"))                     tv.push_back(new symbol(TK::CBRACE));
        if(check_keyword(" ")||check_keyword("\n"))continue;
        if(isdigit(s[i]))                          tv.push_back(new numeric(get_numeric_literal()));
        if(isalpha(s[i])||s[i]=='_')               tv.push_back(new ident(get_identifier()));
        if(!token_detected)                        throw std::runtime_error("認識できないトークンが含まれます");
    }
    itr=tv.begin();
}
bool token_array::check(TK type)const noexcept
{
    return itr!=tv.end()&&(*itr)->type==type;
}
const token*token_array::consume(TK type)noexcept
{
    return check(type)?*(itr++):nullptr;
}
bool token_array::is_all_read()const noexcept
{
    return itr==tv.end();
}
token_array::~token_array()
{
    for(auto t:tv)delete t;
}
