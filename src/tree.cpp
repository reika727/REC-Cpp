#include"syntax/tree.hpp"
#include"semantics/analyzer.hpp"
#include"code/generator.hpp"
#include<stdexcept>
using namespace syntax;
using TK=lexicon::TK;
std::shared_ptr<const function>tree::func()
{
    if(!ta.consume(TK::INT))throw std::runtime_error("関数の型が見つかりませんでした");
    auto fidp=std::dynamic_pointer_cast<const lexicon::ident>(ta.consume(TK::IDENT));
    if(!fidp)throw std::runtime_error("関数名が見つかりませんでした");
    if(!ta.consume(TK::OPARENT))throw std::runtime_error("引数リストが見つかりませんでした");
    auto vars=std::vector<std::string>();
    if(!ta.consume(TK::CPARENT)){
        while(true){
            if(!ta.consume(TK::INT))
                throw std::runtime_error("引数の型が見つかりませんでした");
            if(auto idp=std::dynamic_pointer_cast<const lexicon::ident>(ta.consume(TK::IDENT)))
                vars.push_back(idp->name);
            else
                throw std::runtime_error("引数名が見つかりませんでした");
            if(ta.consume(TK::COMMA))
                continue;
            else if(ta.consume(TK::CPARENT))
                break;
            else
                throw std::runtime_error("不正な区切り文字です");
        }
    }
    if(auto comp=std::dynamic_pointer_cast<const compound>(stat()))
        return std::make_shared<const function>(fidp->name,vars,comp);
    else
        throw std::runtime_error("関数の本体が見つかりませんでした");
}
std::shared_ptr<const statement>tree::stat()
{
    if(ta.consume(TK::INT)){
        auto vars=std::vector<std::pair<std::string,std::shared_ptr<const expression>>>();
        while(true){
            if(auto idp=std::dynamic_pointer_cast<const lexicon::ident>(ta.consume(TK::IDENT)))
                vars.push_back(std::make_pair(idp->name,ta.consume(TK::EQUAL)?order14():nullptr));
            else
                throw std::runtime_error("無効な宣言です");
            if(ta.consume(TK::SCOLON))
                break;
            else if(!ta.consume(TK::COMMA))
                throw std::runtime_error("不正な区切り文字です");
        }
        return std::make_shared<const define_var>(vars);
    }else if(ta.consume(TK::IF)){
        if(!ta.consume(TK::OPARENT))throw std::runtime_error("ifの後ろに括弧がありません");
        auto cond=std::make_shared<const expression_statement>(order15());
        if(!ta.consume(TK::CPARENT))throw std::runtime_error("ifの後ろに括弧がありません");
        auto st1=stat();
        auto st2=ta.consume(TK::ELSE)?stat():std::make_shared<const null_statement>();
        return std::make_shared<const _if_else_>(cond,st1,st2);
    }else if(ta.consume(TK::WHILE)){
        if(!ta.consume(TK::OPARENT))throw std::runtime_error("whileの後ろに括弧がありません");
        auto cond=std::make_shared<const expression_statement>(order15());
        if(!ta.consume(TK::CPARENT))throw std::runtime_error("whileの後ろに括弧がありません");
        auto st=stat();
        return std::make_shared<const _while_>(cond,st);
    }else if(ta.consume(TK::FOR)){
        if(!ta.consume(TK::OPARENT))throw std::runtime_error("forの後ろに括弧がありません");
        auto init=get_single();
        auto cond=get_single();
        auto reinit=get_single();
        if(!ta.consume(TK::CPARENT))throw std::runtime_error("forの後ろに括弧がありません");
        auto st=stat();
        return std::make_shared<const _for_>(init,cond,reinit,st);
    }else if(ta.consume(TK::BREAK)){
        if(!ta.consume(TK::SCOLON))throw std::runtime_error("不正なbreak文です");
        return std::make_shared<const _break_>();
    }else if(ta.consume(TK::CONTINUE)){
        if(!ta.consume(TK::SCOLON))throw std::runtime_error("不正なcontinue文です");
        return std::make_shared<const _continue_>();
    }else if(ta.consume(TK::RETURN)){
        auto ret=std::make_shared<const _return_>(std::make_shared<const expression_statement>(order15()));
        if(!ta.consume(TK::SCOLON))throw std::runtime_error("不正なreturn文です");
        return ret;
    }else if(ta.consume(TK::OBRACE)){
        auto stats=std::vector<std::shared_ptr<const statement>>();
        while(!ta.consume(TK::CBRACE))stats.push_back(stat());
        return std::make_shared<const compound>(stats);
    }else{
        auto ret=get_single();
        return ret;
    }
}
std::shared_ptr<const single_statement>tree::get_single()
{
    if(ta.consume(TK::SCOLON)){
        return std::make_shared<const null_statement>();
    }else{
        auto ret=std::make_shared<const expression_statement>(order15());
        if(!ta.consume(TK::SCOLON))throw std::runtime_error("式文の後ろにセミコロンがありません");
        return ret;
    }
}
std::shared_ptr<const expression>tree::order15() // , left to right
{
    auto ret=order14();
    while(ta.consume(TK::COMMA))ret=std::make_shared<const comma>(ret,order14());
    return ret;
}
std::shared_ptr<const expression>tree::order14() // = += -= *= /= right to left
{
    auto ret=order12();
    while(true){
        if(ta.consume(TK::EQUAL))
            ret=std::make_shared<const assign>(ret,order14());
        else if(ta.consume(TK::PLEQ))
            ret=std::make_shared<const plasgn>(ret,order14());
        else if(ta.consume(TK::MIEQ))
            ret=std::make_shared<const miasgn>(ret,order14());
        else if(ta.consume(TK::ASEQ))
            ret=std::make_shared<const muasgn>(ret,order14());
        else if(ta.consume(TK::SLEQ))
            ret=std::make_shared<const diasgn>(ret,order14());
        else if(ta.consume(TK::PEEQ))
            ret=std::make_shared<const rmasgn>(ret,order14());
        else
            break;
    }
    return ret;
}
std::shared_ptr<const expression>tree::order12() // || left to right
{
    auto ret=order11();
    while(ta.consume(TK::VBVB))ret=std::make_shared<const logor>(ret,order11());
    return ret;
}
std::shared_ptr<const expression>tree::order11() // && left to right
{
    auto ret=order07();
    while(ta.consume(TK::APAP))ret=std::make_shared<const logand>(ret,order07());
    return ret;
}
std::shared_ptr<const expression>tree::order07() // == != left to right
{
    auto ret=order06();
    while(true){
        if(ta.consume(TK::EQEQ))
            ret=std::make_shared<const equal>(ret,order06());
        else if(ta.consume(TK::EXEQ))
            ret=std::make_shared<const nequal>(ret,order06());
        else
            break;
    }
    return ret;
}
std::shared_ptr<const expression>tree::order06() // < > <= >= left to right
{
    auto ret=order04();
    while(true){
        if(ta.consume(TK::LESS))
            ret=std::make_shared<const less>(ret,order04());
        else if(ta.consume(TK::GREATER))
            ret=std::make_shared<const greater>(ret,order04());
        else if(ta.consume(TK::LEEQ))
            ret=std::make_shared<const leeq>(ret,order04());
        else if(ta.consume(TK::GREQ))
            ret=std::make_shared<const greq>(ret,order04());
        else
            break;
    }
    return ret;
}
std::shared_ptr<const expression>tree::order04() // + - left to right
{
    auto ret=order03();
    while(true){
        if(ta.consume(TK::PLUS))
            ret=std::make_shared<const plus>(ret,order03());
        else if(ta.consume(TK::MINUS))
            ret=std::make_shared<const minus>(ret,order03());
        else
            break;
    }
    return ret;
}
std::shared_ptr<const expression>tree::order03() // * / % left to right
{
    auto ret=order02();
    while(true){
        if(ta.consume(TK::ASTER))
            ret=std::make_shared<const multi>(ret,order02());
        else if(ta.consume(TK::SLASH))
            ret=std::make_shared<const divide>(ret,order02());
        else if(ta.consume(TK::PERCENT))
            ret=std::make_shared<const remain>(ret,order02());
        else
            break;
    }
    return ret;
}
std::shared_ptr<const expression>tree::order02() // + - ++ -- ! right to left
{
    if(ta.consume(TK::PLUS))
        return std::make_shared<const uplus>(order02());
    else if(ta.consume(TK::MINUS))
        return std::make_shared<const uminus>(order02());
    else if(ta.consume(TK::PLPL))
        return std::make_shared<const preinc>(order02());
    else if(ta.consume(TK::MIMI))
        return std::make_shared<const predec>(order02());
    else if(ta.consume(TK::EXCLAM))
        return std::make_shared<const lognot>(order02());
    else
        return order01();
}
std::shared_ptr<const expression>tree::order01() // () left to right
{
    auto ret=order00();
    if(ta.consume(TK::PLPL)){
        ret=std::make_shared<const postinc>(ret);
    }else if(ta.consume(TK::MIMI)){
        ret=std::make_shared<const postdec>(ret);
    }else if(ta.consume(TK::OPARENT)){
        auto vars=std::vector<std::shared_ptr<const expression>>();
        if(!ta.consume(TK::CPARENT)){
            while(true){
                vars.push_back(order14());
                if(ta.consume(TK::CPARENT))
                    break;
                else if(!ta.consume(TK::COMMA))
                    throw std::runtime_error("無効な関数呼び出しです");
            }
        }
        ret=std::make_shared<const fcall>(ret,vars);
    }
    return ret;
}
std::shared_ptr<const expression>tree::order00() // literal, identifier, enclosed expression
{
    if(auto nump=std::dynamic_pointer_cast<const lexicon::numeric>(ta.consume(TK::NUMERIC))){
        return std::make_shared<const numeric>(nump->value);
    }else if(auto idp=std::dynamic_pointer_cast<const lexicon::ident>(ta.consume(TK::IDENT))){
        return std::make_shared<const ident>(idp->name);
    }else if(ta.consume(TK::OPARENT)){
        auto ret=order15();
        if(!ta.consume(TK::CPARENT))throw std::runtime_error("括弧の対応が正しくありません");
        return ret;
    }else{
        throw std::runtime_error("構文解析ができませんでした");
    }
}
tree::tree(const std::string&src):ta(src)
{
    while(!ta.is_all_read())funcs.push_back(func());
}
void tree::check()const
{
    semantics::analyzer analy;
    for(auto f:funcs)f->check(analy);
}
void tree::to_asm(const std::string&dst)const
{
    code::generator cg(dst);
    for(auto f:funcs)f->to_asm(cg);
}
