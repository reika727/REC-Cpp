#include"lexicon/token_array.hpp"
#include<algorithm>
#include<initializer_list>
#include<stdexcept>
using namespace lexicon;
token_array::token_array(const std::string&s)
{
    for(int i=0;i<s.length();){
        if(isspace(s[i])){
            ++i;
            continue;
        }
        auto check_keyword=[s,&i](const std::string&token,auto...follow){
            if(s.substr(i,token.length())!=token)return false;
            if(sizeof...(follow)!=0){
                bool match=false;
                for(auto&&f:std::initializer_list<char>{follow...})match|=i+token.length()<s.length()&&s[i+token.length()]==f;
                if(!match)return false;
            }
            i+=token.length();
            return true;
        };
        auto check_comment_closed=[s,&i](){
            while(true){
                if(++i>=s.length())throw std::runtime_error("コメントが閉じられていません");
                else if(s.substr(i,2)=="*/"&&(i+=2))break;
            }
        };
        auto get_numeric_literal=[s,&i](){
            size_t sz;
            int ret=std::stoi(s.substr(i),&sz);
            i+=sz;
            return ret;
        };
        auto get_identifier=[s,&i](){
            auto beg=s.begin()+i;
            auto ret=std::string(beg,find_if_not(beg,s.end(),[](char c){return isalpha(c)||isdigit(c)||c=='_';}));
            i+=ret.length();
            return ret;
        };
        if(check_keyword("int",' '))
            tv.push_back(new symbol(TK::INT));
        else if(check_keyword("if",'(',' '))
            tv.push_back(new symbol(TK::IF));
        else if(check_keyword("else",'{',';',' '))
            tv.push_back(new symbol(TK::ELSE));
        else if(check_keyword("while",'(',' '))
            tv.push_back(new symbol(TK::WHILE));
        else if(check_keyword("for",'(',' '))
            tv.push_back(new symbol(TK::FOR));
        else if(check_keyword("break",';',' '))
            tv.push_back(new symbol(TK::BREAK));
        else if(check_keyword("continue",';',' '))
            tv.push_back(new symbol(TK::CONTINUE));
        else if(check_keyword("return",' '))
            tv.push_back(new symbol(TK::RETURN));
        else if(s.substr(i,2)=="/*")
            check_comment_closed();
        else if(check_keyword("&&"))
            tv.push_back(new symbol(TK::APAP));
        else if(check_keyword("||"))
            tv.push_back(new symbol(TK::VBVB));
        else if(check_keyword("++"))
            tv.push_back(new symbol(TK::PLPL));
        else if(check_keyword("--"))
            tv.push_back(new symbol(TK::MIMI));
        else if(check_keyword("+="))
            tv.push_back(new symbol(TK::PLEQ));
        else if(check_keyword("-="))
            tv.push_back(new symbol(TK::MIEQ));
        else if(check_keyword("*="))
            tv.push_back(new symbol(TK::ASEQ));
        else if(check_keyword("/="))
            tv.push_back(new symbol(TK::SLEQ));
        else if(check_keyword("%="))
            tv.push_back(new symbol(TK::PEEQ));
        else if(check_keyword("=="))
            tv.push_back(new symbol(TK::EQEQ));
        else if(check_keyword("!="))
            tv.push_back(new symbol(TK::EXEQ));
        else if(check_keyword("<="))
            tv.push_back(new symbol(TK::LEEQ));
        else if(check_keyword(">="))
            tv.push_back(new symbol(TK::GREQ));
        else if(check_keyword("+"))
            tv.push_back(new symbol(TK::PLUS));
        else if(check_keyword("-"))
            tv.push_back(new symbol(TK::MINUS));
        else if(check_keyword("*"))
            tv.push_back(new symbol(TK::ASTER));
        else if(check_keyword("/"))
            tv.push_back(new symbol(TK::SLASH));
        else if(check_keyword("%"))
            tv.push_back(new symbol(TK::PERCENT));
        else if(check_keyword("<"))
            tv.push_back(new symbol(TK::LESS));
        else if(check_keyword(">"))
            tv.push_back(new symbol(TK::GREATER));
        else if(check_keyword("!"))
            tv.push_back(new symbol(TK::EXCLAM));
        else if(check_keyword("="))
            tv.push_back(new symbol(TK::EQUAL));
        else if(check_keyword(","))
            tv.push_back(new symbol(TK::COMMA));
        else if(check_keyword(";"))
            tv.push_back(new symbol(TK::SCOLON));
        else if(check_keyword("("))
            tv.push_back(new symbol(TK::OPARENT));
        else if(check_keyword(")"))
            tv.push_back(new symbol(TK::CPARENT));
        else if(check_keyword("{"))
            tv.push_back(new symbol(TK::OBRACE));
        else if(check_keyword("}"))
            tv.push_back(new symbol(TK::CBRACE));
        else if(isdigit(s[i]))
            tv.push_back(new numeric(get_numeric_literal()));
        else if(isalpha(s[i])||s[i]=='_')
            tv.push_back(new ident(get_identifier()));
        else
            throw std::runtime_error("認識できないトークンが含まれます");
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
