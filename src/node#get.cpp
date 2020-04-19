#include"node.hpp"
#include"compilation_error.hpp"
using namespace syntax;
std::unique_ptr<const expression>expression::get(lexicon::lexer&lx,bool for_initialization)
{
    return for_initialization?get_order14(lx):get_order15(lx);
}
std::unique_ptr<const expression>expression::get_order15(lexicon::lexer&lx) // , left to right
{
    auto ret=get_order14(lx);
    while(auto sym=lx.consume_symbol_if(lexicon::symbol::COMMA))
        ret=std::make_unique<const comma>(std::move(ret),get_order14(lx),sym->line,sym->col);
    return ret;
}
std::unique_ptr<const expression>expression::get_order14(lexicon::lexer&lx) // = += -= *= /= right to left
{
    auto ret=get_order13(lx);
    while(true){
        if(auto sym=lx.consume_symbol_if(lexicon::symbol::EQUAL))
            ret=std::make_unique<const assign>(std::move(ret),get_order14(lx),sym->line,sym->col);
        else if(sym=lx.consume_symbol_if(lexicon::symbol::PLEQ))
            ret=std::make_unique<const plasgn>(std::move(ret),get_order14(lx),sym->line,sym->col);
        else if(sym=lx.consume_symbol_if(lexicon::symbol::MIEQ))
            ret=std::make_unique<const miasgn>(std::move(ret),get_order14(lx),sym->line,sym->col);
        else if(sym=lx.consume_symbol_if(lexicon::symbol::ASEQ))
            ret=std::make_unique<const muasgn>(std::move(ret),get_order14(lx),sym->line,sym->col);
        else if(sym=lx.consume_symbol_if(lexicon::symbol::SLEQ))
            ret=std::make_unique<const diasgn>(std::move(ret),get_order14(lx),sym->line,sym->col);
        else if(sym=lx.consume_symbol_if(lexicon::symbol::PEEQ))
            ret=std::make_unique<const rmasgn>(std::move(ret),get_order14(lx),sym->line,sym->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order13(lexicon::lexer&lx)
{
    // TODO: placeholder
    return get_order12(lx);
}
std::unique_ptr<const expression>expression::get_order12(lexicon::lexer&lx) // || left to right
{
    auto ret=get_order11(lx);
    while(auto sym=lx.consume_symbol_if(lexicon::symbol::VBVB))
        ret=std::make_unique<const logor>(std::move(ret),get_order11(lx),sym->line,sym->col);
    return ret;
}
std::unique_ptr<const expression>expression::get_order11(lexicon::lexer&lx) // && left to right
{
    auto ret=get_order10(lx);
    while(auto sym=lx.consume_symbol_if(lexicon::symbol::APAP))
        ret=std::make_unique<const logand>(std::move(ret),get_order10(lx),sym->line,sym->col);
    return ret;
}
std::unique_ptr<const expression>expression::get_order10(lexicon::lexer&lx)
{
    // TODO: placeholder
    return get_order09(lx);
}
std::unique_ptr<const expression>expression::get_order09(lexicon::lexer&lx)
{
    // TODO: placeholder
    return get_order08(lx);
}
std::unique_ptr<const expression>expression::get_order08(lexicon::lexer&lx)
{
    // TODO: placeholder
    return get_order07(lx);
}
std::unique_ptr<const expression>expression::get_order07(lexicon::lexer&lx) // == != left to right
{
    auto ret=get_order06(lx);
    while(true){
        if(auto sym=lx.consume_symbol_if(lexicon::symbol::EQEQ))
            ret=std::make_unique<const equal>(std::move(ret),get_order06(lx),sym->line,sym->col);
        else if(sym=lx.consume_symbol_if(lexicon::symbol::EXEQ))
            ret=std::make_unique<const nequal>(std::move(ret),get_order06(lx),sym->line,sym->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order06(lexicon::lexer&lx) // < > <= >= left to right
{
    auto ret=get_order05(lx);
    while(true){
        if(auto sym=lx.consume_symbol_if(lexicon::symbol::LESS))
            ret=std::make_unique<const less>(std::move(ret),get_order05(lx),sym->line,sym->col);
        else if(sym=lx.consume_symbol_if(lexicon::symbol::GREATER))
            ret=std::make_unique<const greater>(std::move(ret),get_order05(lx),sym->line,sym->col);
        else if(sym=lx.consume_symbol_if(lexicon::symbol::LEEQ))
            ret=std::make_unique<const leeq>(std::move(ret),get_order05(lx),sym->line,sym->col);
        else if(sym=lx.consume_symbol_if(lexicon::symbol::GREQ))
            ret=std::make_unique<const greq>(std::move(ret),get_order05(lx),sym->line,sym->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order05(lexicon::lexer&lx)
{
    // TODO: placeholder
    return get_order04(lx);
}
std::unique_ptr<const expression>expression::get_order04(lexicon::lexer&lx) // + - left to right
{
    auto ret=get_order03(lx);
    while(true){
        if(auto sym=lx.consume_symbol_if(lexicon::symbol::PLUS))
            ret=std::make_unique<const bplus>(std::move(ret),get_order03(lx),sym->line,sym->col);
        else if(sym=lx.consume_symbol_if(lexicon::symbol::MINUS))
            ret=std::make_unique<const bminus>(std::move(ret),get_order03(lx),sym->line,sym->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order03(lexicon::lexer&lx) // * / % left to right
{
    auto ret=get_order02(lx);
    while(true){
        if(auto sym=lx.consume_symbol_if(lexicon::symbol::ASTER))
            ret=std::make_unique<const multiply>(std::move(ret),get_order02(lx),sym->line,sym->col);
        else if(sym=lx.consume_symbol_if(lexicon::symbol::SLASH))
            ret=std::make_unique<const divide>(std::move(ret),get_order02(lx),sym->line,sym->col);
        else if(sym=lx.consume_symbol_if(lexicon::symbol::PERCENT))
            ret=std::make_unique<const remain>(std::move(ret),get_order02(lx),sym->line,sym->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order02(lexicon::lexer&lx) // + - ++ -- ! right to left
{
    if(auto sym=lx.consume_symbol_if(lexicon::symbol::PLUS))
        return std::make_unique<const uplus>(get_order02(lx),sym->line,sym->col);
    else if(sym=lx.consume_symbol_if(lexicon::symbol::MINUS))
        return std::make_unique<const uminus>(get_order02(lx),sym->line,sym->col);
    else if(sym=lx.consume_symbol_if(lexicon::symbol::PLPL))
        return std::make_unique<const preinc>(get_order02(lx),sym->line,sym->col);
    else if(sym=lx.consume_symbol_if(lexicon::symbol::MIMI))
        return std::make_unique<const predec>(get_order02(lx),sym->line,sym->col);
    else if(sym=lx.consume_symbol_if(lexicon::symbol::EXCLAM))
        return std::make_unique<const lognot>(get_order02(lx),sym->line,sym->col);
    else
        return get_order01(lx);
}
std::unique_ptr<const expression>expression::get_order01(lexicon::lexer&lx) // () left to right
{
    auto ret=get_primary(lx);
    if(auto sym=lx.consume_symbol_if(lexicon::symbol::PLPL)){
        return std::make_unique<const postinc>(std::move(ret),sym->line,sym->col);
    }else if(sym=lx.consume_symbol_if(lexicon::symbol::MIMI)){
        return std::make_unique<const postdec>(std::move(ret),sym->line,sym->col);
    }else if(sym=lx.consume_symbol_if(lexicon::symbol::OPARENT)){
        auto vars=std::vector<std::unique_ptr<const expression>>();
        if(!lx.consume_symbol_if(lexicon::symbol::CPARENT)){
            while(true){
                vars.push_back(get_order14(lx));
                if(lx.consume_symbol_if(lexicon::symbol::CPARENT))
                    break;
                else if(!lx.consume_symbol_if(lexicon::symbol::COMMA))
                    throw exception::compilation_error("関数呼び出し演算子のコンマに問題があります",sym->line,sym->col);
            }
        }
        return std::make_unique<const fcall>(std::move(ret),vars,sym->line,sym->col,expression::type_info::get_int()); // TODO: とりあえずintで固定
    }else{
        return ret;
    }
}
std::unique_ptr<const expression>expression::get_primary(lexicon::lexer&lx) // literal, identifier, enclosed expression
{
    if(auto id=lx.consume_identifier()){
        return std::make_unique<const identifier>(id->name,id->line,id->col,expression::type_info::get_int()); // TODO: とりあえずintで固定
    }else if(auto num=lx.consume_numeric()){
        return std::make_unique<const numeric>(num->value,num->line,num->col,expression::type_info::get_int()); // TODO: とりあえずintで固定
    }else if(auto sym=lx.consume_symbol_if(lexicon::symbol::OPARENT)){
        auto ret=get_order15(lx);
        if(!lx.consume_symbol_if(lexicon::symbol::CPARENT))
            throw exception::compilation_error("括弧の対応が正しくありません",sym->line,sym->col);
        return ret;
    }
    throw exception::compilation_error("構文解析ができませんでした",lx.get_line(),lx.get_column());
}
std::unique_ptr<const statement>statement::get(lexicon::lexer&lx)
{
    if(lx.check_symbol(lexicon::symbol::INT))return std::make_unique<const var_difinition>(lx);
    else if(lx.check_symbol(lexicon::symbol::IF))return std::make_unique<const _if_else_>(lx);
    else if(lx.check_symbol(lexicon::symbol::WHILE))return std::make_unique<const _while_>(lx);
    else if(lx.check_symbol(lexicon::symbol::FOR))return std::make_unique<const _for_>(lx);
    else if(lx.check_symbol(lexicon::symbol::BREAK))return std::make_unique<const _break_>(lx);
    else if(lx.check_symbol(lexicon::symbol::CONTINUE))return std::make_unique<const _continue_>(lx);
    else if(lx.check_symbol(lexicon::symbol::RETURN))return std::make_unique<const _return_>(lx);
    else if(lx.check_symbol(lexicon::symbol::OBRACE))return std::make_unique<const compound>(lx);
    else if(lx.check_symbol(lexicon::symbol::SCOLON))return std::make_unique<const null_statement>(lx);
    else return std::make_unique<const expression_statement>(lx);
}
