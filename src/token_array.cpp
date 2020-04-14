#include"token_array.hpp"
#include"compilation_error.hpp"
#include<algorithm>
using namespace lexicon;
token_array::token_array(const std::string&src)
    :src(src),pos(0),line(1),col(1){}
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
std::optional<numeric>token_array::consume_numeric()
{
    skip_space_or_comment();
    if(is_all_read()||!std::isdigit(src[pos]))return std::nullopt;
    size_t sz;
    int num=std::stoi(src.substr(pos),&sz);
    auto ret=std::make_optional<numeric>(num,line,col);
    pos+=sz;
    col+=sz;
    return ret;
}
std::optional<identifier>token_array::consume_identifier()
{
    skip_space_or_comment();
    if(is_all_read()||!std::isalpha(src[pos])&&src[pos]!='_')return std::nullopt;
    auto name=std::string(src.begin()+pos,std::find_if_not(src.begin()+pos,src.end(),[](char c){return std::isalpha(c)||std::isdigit(c)||c=='_';}));
    auto ret=std::make_optional<identifier>(name,line,col);
    pos+=name.length();
    col+=name.length();
    return ret;
}
bool token_array::check_symbol(symbol::SYMBOL sym)
{
    skip_space_or_comment();
    if(is_all_read())return false;
    auto m=symbol::match(src,pos);
    return m.has_value()&&m.value().first==sym;
}
std::optional<symbol>token_array::consume_symbol(symbol::SYMBOL sym)
{
    skip_space_or_comment();
    if(is_all_read())return std::nullopt;
    if(auto m=symbol::match(src,pos);m.has_value()&&m.value().first==sym){
        auto ret=std::make_optional<symbol>(sym,line,col);
        pos+=m.value().second;
        col+=m.value().second;
        return ret;
    }else{
        return std::nullopt;
    }
}
