#include"syntax/tree.hpp"
#include"semantics/analyzer.hpp"
#include"code/generator.hpp"
#include<stdexcept>
using namespace syntax;
using TK=lexicon::TK;
const function*tree::func()
{
    if(!ta.consume(TK::INT))throw std::runtime_error("関数の型が見つかりませんでした");
    auto fidp=dynamic_cast<const lexicon::ident*>(ta.consume(TK::IDENT));
    if(!fidp)throw std::runtime_error("関数名が見つかりませんでした");
    if(!ta.consume(TK::OPARENT))throw std::runtime_error("引数リストが見つかりませんでした");
    auto vars=new std::vector<std::string>;
    if(!ta.consume(TK::CPARENT)){
        while(true){
            if(!ta.consume(TK::INT))
                throw std::runtime_error("引数の型が見つかりませんでした");
            if(auto idp=dynamic_cast<const lexicon::ident*>(ta.consume(TK::IDENT)))
                vars->push_back(idp->name);
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
    if(ta.consume(TK::SCOLON))
        return new function(fidp->name,vars,nullptr);
    else if(auto comp=dynamic_cast<const compound*>(stat()))
        return new function(fidp->name,vars,comp);
    else
        throw std::runtime_error("関数の本体が見つかりませんでした");
}
const statement*tree::stat()
{
    if(ta.consume(TK::INT)){
        auto vars=new std::vector<std::pair<std::string,const expression*>>;
        while(true){
            if(auto idp=dynamic_cast<const lexicon::ident*>(ta.consume(TK::IDENT)))
                vars->push_back(std::make_pair(idp->name,ta.consume(TK::EQUAL)?order14():nullptr));
            else
                throw std::runtime_error("無効な宣言です");
            if(ta.consume(TK::SCOLON))
                break;
            else if(!ta.consume(TK::COMMA))
                throw std::runtime_error("不正な区切り文字です");
        }
        return new define_var(vars);
    }else if(ta.consume(TK::IF)){
        if(!ta.consume(TK::OPARENT))throw std::runtime_error("ifの後ろに括弧がありません");
        auto cond=new single(order15());
        if(!ta.consume(TK::CPARENT))throw std::runtime_error("ifの後ろに括弧がありません");
        auto st1=stat();
        auto st2=ta.consume(TK::ELSE)?stat():new single(nullptr);
        return new _if_else_(cond,st1,st2);
    }else if(ta.consume(TK::WHILE)){
        if(!ta.consume(TK::OPARENT))throw std::runtime_error("whileの後ろに括弧がありません");
        auto cond=new single(order15());
        if(!ta.consume(TK::CPARENT))throw std::runtime_error("whileの後ろに括弧がありません");
        auto st=stat();
        return new _while_(cond,st);
    }else if(ta.consume(TK::FOR)){
        if(!ta.consume(TK::OPARENT))throw std::runtime_error("forの後ろに括弧がありません");
        auto init=emptiable_single();
        if(!ta.consume(TK::SCOLON))throw std::runtime_error("不正な区切り文字です");
        auto cond=emptiable_single();
        if(!ta.consume(TK::SCOLON))throw std::runtime_error("不正な区切り文字です");
        auto reinit=emptiable_single();
        if(!ta.consume(TK::CPARENT))throw std::runtime_error("forの後ろに括弧がありません");
        auto st=stat();
        return new _for_(init,cond,reinit,st);
    }else if(ta.consume(TK::BREAK)){
        if(!ta.consume(TK::SCOLON))throw std::runtime_error("不正なbreak文です");
        return new _break_;
    }else if(ta.consume(TK::CONTINUE)){
        if(!ta.consume(TK::SCOLON))throw std::runtime_error("不正なcontinue文です");
        return new _continue_;
    }else if(ta.consume(TK::RETURN)){
        return new _return_(new single(order15()));
    }else if(ta.consume(TK::OBRACE)){
        auto stats=new std::vector<const statement*>;
        while(!ta.consume(TK::CBRACE))stats->push_back(stat());
        return new compound(stats);
    }else{
        auto ret=emptiable_single();
        if(!ta.consume(TK::SCOLON))throw std::runtime_error("不正な区切り文字です");
        return ret;
    }
}
const single*tree::emptiable_single()
{
    return new single(ta.check(TK::SCOLON)||ta.check(TK::CPARENT)?nullptr:order15());
}
const expression*tree::order15() // , left to right
{
    auto ret=order14();
    while(ta.consume(TK::COMMA))ret=new comma(ret,order14());
    return ret;
}
const expression*tree::order14() // = += -= *= /= right to left
{
    auto ret=order12();
    while(true){
        if(ta.consume(TK::EQUAL))
            ret=new assign(ret,order14());
        else if(ta.consume(TK::PLEQ))
            ret=new plasgn(ret,order14());
        else if(ta.consume(TK::MIEQ))
            ret=new miasgn(ret,order14());
        else if(ta.consume(TK::ASEQ))
            ret=new muasgn(ret,order14());
        else if(ta.consume(TK::SLEQ))
            ret=new diasgn(ret,order14());
        else if(ta.consume(TK::PEEQ))
            ret=new rmasgn(ret,order14());
        else
            break;
    }
    return ret;
}
const expression*tree::order12() // || left to right
{
    auto ret=order11();
    while(ta.consume(TK::VBVB))ret=new logor(ret,order11());
    return ret;
}
const expression*tree::order11() // && left to right
{
    auto ret=order07();
    while(ta.consume(TK::APAP))ret=new logand(ret,order07());
    return ret;
}
const expression*tree::order07() // == != left to right
{
    auto ret=order06();
    while(true){
        if(ta.consume(TK::EQEQ))
            ret=new equal (ret,order06());
        else if(ta.consume(TK::EXEQ))
            ret=new nequal(ret,order06());
        else
            break;
    }
    return ret;
}
const expression*tree::order06() // < > <= >= left to right
{
    auto ret=order04();
    while(true){
        if(ta.consume(TK::LESS))
            ret=new less(ret,order04());
        else if(ta.consume(TK::GREATER))
            ret=new greater(ret,order04());
        else if(ta.consume(TK::LEEQ))
            ret=new leeq(ret,order04());
        else if(ta.consume(TK::GREQ))
            ret=new greq(ret,order04());
        else
            break;
    }
    return ret;
}
const expression*tree::order04() // + - left to right
{
    auto ret=order03();
    while(true){
        if(ta.consume(TK::PLUS))
            ret=new plus(ret,order03());
        else if(ta.consume(TK::MINUS))
            ret=new minus(ret,order03());
        else
            break;
    }
    return ret;
}
const expression*tree::order03() // * / % left to right
{
    auto ret=order02();
    while(true){
        if(ta.consume(TK::ASTER))
            ret=new multi(ret,order02());
        else if(ta.consume(TK::SLASH))
            ret=new divide(ret,order02());
        else if(ta.consume(TK::PERCENT))
            ret=new remain(ret,order02());
        else
            break;
    }
    return ret;
}
const expression*tree::order02() // + - ++ -- ! right to left
{
    if(ta.consume(TK::PLUS))
        return new uplus(order02());
    else if(ta.consume(TK::MINUS))
        return new uminus(order02());
    else if(ta.consume(TK::PLPL))
        return new preinc(order02());
    else if(ta.consume(TK::MIMI))
        return new predec(order02());
    else if(ta.consume(TK::EXCLAM))
        return new lognot(order02());
    else
        return order01();
}
const expression*tree::order01() // () left to right
{
    auto ret=order00();
    if(ta.consume(TK::PLPL)){
        ret=new postinc(ret);
    }else if(ta.consume(TK::MIMI)){
        ret=new postdec(ret);
    }else if(ta.consume(TK::OPARENT)){
        auto vars=new std::vector<const expression*>;
        if(!ta.consume(TK::CPARENT)){
            while(true){
                vars->push_back(order14());
                if(ta.consume(TK::CPARENT))
                    break;
                else if(!ta.consume(TK::COMMA))
                    throw std::runtime_error("無効な関数呼び出しです");
            }
        }
        ret=new fcall(ret,vars);
    }
    return ret;
}
const expression*tree::order00() // literal, identifier, enclosed expression
{
    if(auto nump=dynamic_cast<const lexicon::numeric*>(ta.consume(TK::NUMERIC))){
        return new numeric(nump->value);
    }else if(auto idp=dynamic_cast<const lexicon::ident*>(ta.consume(TK::IDENT))){
        return new ident(idp->name);
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
tree::~tree()
{
    for(auto f:funcs)delete f;
}
