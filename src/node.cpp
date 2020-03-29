#include"syntax/node.hpp"
#include"exception/compilation_error.hpp"
using namespace syntax;
std::shared_ptr<const expression>expression::get(lexicon::token_array&ta,bool for_initialization)
{
    return for_initialization?get_order14(ta):get_order15(ta);
}
std::shared_ptr<const expression>expression::get_order15(lexicon::token_array&ta) // , left to right
{
    auto ret=get_order14(ta);
    while(auto tp=ta.consume(lexicon::TK::COMMA))
        ret=std::make_shared<const comma>(ret,get_order14(ta),tp->line,tp->col);
    return ret;
}
std::shared_ptr<const expression>expression::get_order14(lexicon::token_array&ta) // = += -= *= /= right to left
{
    auto ret=get_order13(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::EQUAL))
            ret=std::make_shared<const assign>(ret,get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::PLEQ))
            ret=std::make_shared<const plasgn>(ret,get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::MIEQ))
            ret=std::make_shared<const miasgn>(ret,get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::ASEQ))
            ret=std::make_shared<const muasgn>(ret,get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::SLEQ))
            ret=std::make_shared<const diasgn>(ret,get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::PEEQ))
            ret=std::make_shared<const rmasgn>(ret,get_order14(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::shared_ptr<const expression>expression::get_order13(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order12(ta);
}
std::shared_ptr<const expression>expression::get_order12(lexicon::token_array&ta) // || left to right
{
    auto ret=get_order11(ta);
    while(auto tp=ta.consume(lexicon::TK::VBVB))
        ret=std::make_shared<const logor>(ret,get_order11(ta),tp->line,tp->col);
    return ret;
}
std::shared_ptr<const expression>expression::get_order11(lexicon::token_array&ta) // && left to right
{
    auto ret=get_order10(ta);
    while(auto tp=ta.consume(lexicon::TK::APAP))
        ret=std::make_shared<const logand>(ret,get_order10(ta),tp->line,tp->col);
    return ret;
}
std::shared_ptr<const expression>expression::get_order10(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order09(ta);
}
std::shared_ptr<const expression>expression::get_order09(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order08(ta);
}
std::shared_ptr<const expression>expression::get_order08(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order07(ta);
}
std::shared_ptr<const expression>expression::get_order07(lexicon::token_array&ta) // == != left to right
{
    auto ret=get_order06(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::EQEQ))
            ret=std::make_shared<const equal>(ret,get_order06(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::EXEQ))
            ret=std::make_shared<const nequal>(ret,get_order06(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::shared_ptr<const expression>expression::get_order06(lexicon::token_array&ta) // < > <= >= left to right
{
    auto ret=get_order05(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::LESS))
            ret=std::make_shared<const less>(ret,get_order05(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::GREATER))
            ret=std::make_shared<const greater>(ret,get_order05(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::LEEQ))
            ret=std::make_shared<const leeq>(ret,get_order05(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::GREQ))
            ret=std::make_shared<const greq>(ret,get_order05(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::shared_ptr<const expression>expression::get_order05(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order04(ta);
}
std::shared_ptr<const expression>expression::get_order04(lexicon::token_array&ta) // + - left to right
{
    auto ret=get_order03(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::PLUS))
            ret=std::make_shared<const bplus>(ret,get_order03(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::MINUS))
            ret=std::make_shared<const bminus>(ret,get_order03(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::shared_ptr<const expression>expression::get_order03(lexicon::token_array&ta) // * / % left to right
{
    auto ret=get_order02(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::ASTER))
            ret=std::make_shared<const multiply>(ret,get_order02(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::SLASH))
            ret=std::make_shared<const divide>(ret,get_order02(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::PERCENT))
            ret=std::make_shared<const remain>(ret,get_order02(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::shared_ptr<const expression>expression::get_order02(lexicon::token_array&ta) // + - ++ -- ! right to left
{
    if(auto tp=ta.consume(lexicon::TK::PLUS))
        return std::make_shared<const uplus>(get_order02(ta),tp->line,tp->col);
    else if(tp=ta.consume(lexicon::TK::MINUS))
        return std::make_shared<const uminus>(get_order02(ta),tp->line,tp->col);
    else if(tp=ta.consume(lexicon::TK::PLPL))
        return std::make_shared<const preinc>(get_order02(ta),tp->line,tp->col);
    else if(tp=ta.consume(lexicon::TK::MIMI))
        return std::make_shared<const predec>(get_order02(ta),tp->line,tp->col);
    else if(tp=ta.consume(lexicon::TK::EXCLAM))
        return std::make_shared<const lognot>(get_order02(ta),tp->line,tp->col);
    else
        return get_order01(ta);
}
std::shared_ptr<const expression>expression::get_order01(lexicon::token_array&ta) // () left to right
{
    auto ret=get_primary(ta);
    if(auto tp=ta.consume(lexicon::TK::PLPL)){
        return std::make_shared<const postinc>(ret,tp->line,tp->col);
    }else if(tp=ta.consume(lexicon::TK::MIMI)){
        return std::make_shared<const postdec>(ret,tp->line,tp->col);
    }else if(tp=ta.consume(lexicon::TK::OPARENT)){
        auto vars=std::vector<std::shared_ptr<const expression>>();
        if(!ta.consume(lexicon::TK::CPARENT)){
            while(true){
                vars.push_back(get_order14(ta));
                if(ta.consume(lexicon::TK::CPARENT))
                    break;
                else if(!ta.consume(lexicon::TK::COMMA))
                    throw exception::syntax_error("関数呼び出し演算子のコンマに問題があります",tp->line,tp->col);
            }
        }
        return std::make_shared<const fcall>(ret,vars,tp->line,tp->col);
    }else{
        return ret;
    }
}
std::shared_ptr<const expression>expression::get_primary(lexicon::token_array&ta) // literal, identifier, enclosed expression
{
    if(auto nump=std::dynamic_pointer_cast<const lexicon::numeric>(ta.consume(lexicon::TK::NUMERIC))){
        return std::make_shared<const numeric>(nump->value,nump->line,nump->col);
    }else if(auto idp=std::dynamic_pointer_cast<const lexicon::identifier>(ta.consume(lexicon::TK::IDENT))){
        return std::make_shared<const identifier>(idp->name,idp->line,idp->col);
    }else if(auto tp=ta.consume(lexicon::TK::OPARENT)){
        auto ret=get_order15(ta);
        if(!ta.consume(lexicon::TK::CPARENT))throw exception::syntax_error("括弧の対応が正しくありません",tp->line,tp->col);
        return ret;
    }
    throw exception::syntax_error("構文解析ができませんでした",ta.get_line(),ta.get_column());
}
std::shared_ptr<const statement>statement::get(lexicon::token_array&ta)
{
    if(ta.check(lexicon::TK::INT))return define_var::get(ta);
    else if(ta.check(lexicon::TK::IF))return _if_else_::get(ta);
    else if(ta.check(lexicon::TK::WHILE))return _while_::get(ta);
    else if(ta.check(lexicon::TK::FOR))return _for_::get(ta);
    else if(ta.check(lexicon::TK::BREAK))return _break_::get(ta);
    else if(ta.check(lexicon::TK::CONTINUE))return _continue_::get(ta);
    else if(ta.check(lexicon::TK::RETURN))return _return_::get(ta);
    else if(ta.check(lexicon::TK::OBRACE))return compound::get(ta);
    else if(ta.check(lexicon::TK::SCOLON))return null_statement::get(ta);
    else return expression_statement::get(ta);
}
std::shared_ptr<const expression_statement>expression_statement::get(lexicon::token_array&ta)
{
    auto ret=std::make_shared<expression_statement>(ta.get_line(),ta.get_column());
    ret->expr=expression::get(ta);
    if(!ta.consume(lexicon::TK::SCOLON))throw exception::syntax_error("セミコロンが見つかりませんでした",ta.get_line(),ta.get_column());
    return ret;
}
std::shared_ptr<const null_statement>null_statement::get(lexicon::token_array&ta)
{
    auto ret=std::make_shared<null_statement>(ta.get_line(),ta.get_column());
    if(!ta.consume(lexicon::TK::SCOLON))throw exception::syntax_error("セミコロンが見つかりませんでした",ta.get_line(),ta.get_column());
    return ret;
}
std::shared_ptr<const compound>compound::get(lexicon::token_array&ta)
{
    if(!ta.consume(lexicon::TK::OBRACE))throw exception::syntax_error("複文の開始ブラケットが見つかりませんでした",ta.get_line(),ta.get_column());
    auto ret=std::make_shared<compound>(ta.get_line(),ta.get_column());
    while(!ta.consume(lexicon::TK::CBRACE))ret->stats.push_back(statement::get(ta));
    return ret;
}
std::shared_ptr<const define_var>define_var::get(lexicon::token_array&ta)
{
    if(!ta.consume(lexicon::TK::INT))throw exception::syntax_error("型指定子が見つかりませんでした",ta.get_line(),ta.get_column());
    auto ret=std::make_shared<define_var>(ta.get_line(),ta.get_column());
    while(true){
        if(auto idp=std::dynamic_pointer_cast<const lexicon::identifier>(ta.consume(lexicon::TK::IDENT)))
            ret->vars.push_back(std::make_pair(idp->name,ta.consume(lexicon::TK::EQUAL)?expression::get(ta,true):nullptr));
        else
            throw exception::syntax_error("変数名が見つかりませんでした",ta.get_line(),ta.get_column());
        if(ta.consume(lexicon::TK::SCOLON))
            break;
        else if(!ta.consume(lexicon::TK::COMMA))
            throw exception::syntax_error("不正な区切り文字です",ta.get_line(),ta.get_column());
    }
    return ret;
}
std::shared_ptr<const _if_else_>_if_else_::get(lexicon::token_array&ta)
{
    if(!ta.consume(lexicon::TK::IF))throw exception::syntax_error("ifキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume(lexicon::TK::OPARENT))throw exception::syntax_error("ifの後ろに括弧がありません",ta.get_line(),ta.get_column());
    auto ret=std::make_shared<_if_else_>(ta.get_line(),ta.get_column());
    ret->cond=expression::get(ta);
    if(!ta.consume(lexicon::TK::CPARENT))throw exception::syntax_error("ifの後ろに括弧がありません",ta.get_line(),ta.get_column());
    ret->stat_if=statement::get(ta);
    ret->stat_else=ta.consume(lexicon::TK::ELSE)?statement::get(ta):nullptr;
    return ret;
}
std::shared_ptr<const _while_>_while_::get(lexicon::token_array&ta)
{
    if(!ta.consume(lexicon::TK::WHILE))throw exception::syntax_error("whileキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume(lexicon::TK::OPARENT))throw exception::syntax_error("whileの後ろに括弧がありません",ta.get_line(),ta.get_column());
    auto ret=std::make_shared<_while_>(ta.get_line(),ta.get_column());
    ret->cond=expression::get(ta);
    if(!ta.consume(lexicon::TK::CPARENT))throw exception::syntax_error("whileの後ろに括弧がありません",ta.get_line(),ta.get_column());
    ret->stat=statement::get(ta);
    return ret;
}
std::shared_ptr<const _for_>_for_::get(lexicon::token_array&ta)
{
    if(!ta.consume(lexicon::TK::FOR))throw exception::syntax_error("forキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume(lexicon::TK::OPARENT))throw exception::syntax_error("forの後ろに括弧がありません",ta.get_line(),ta.get_column());
    auto ret=std::make_shared<_for_>(ta.get_line(),ta.get_column());
    ret->init=ta.check(lexicon::TK::SCOLON)?nullptr:expression::get(ta);
    if(!ta.consume(lexicon::TK::SCOLON))throw exception::syntax_error("セミコロンが見つかりませんでした",ta.get_line(),ta.get_column());
    ret->cond=ta.check(lexicon::TK::SCOLON)?nullptr:expression::get(ta);
    if(!ta.consume(lexicon::TK::SCOLON))throw exception::syntax_error("セミコロンが見つかりませんでした",ta.get_line(),ta.get_column());
    ret->reinit=ta.check(lexicon::TK::CPARENT)?nullptr:expression::get(ta);
    if(!ta.consume(lexicon::TK::CPARENT))throw exception::syntax_error("forの後ろに括弧がありません",ta.get_line(),ta.get_column());
    ret->stat=statement::get(ta);
    return ret;
}
std::shared_ptr<const _break_>_break_::get(lexicon::token_array&ta)
{
    if(!ta.consume(lexicon::TK::BREAK))throw exception::syntax_error("breakキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume(lexicon::TK::SCOLON))throw exception::syntax_error("不正なbreak文です",ta.get_line(),ta.get_column());
    return std::make_shared<const _break_>(ta.get_line(),ta.get_column());
}
std::shared_ptr<const _continue_>_continue_::get(lexicon::token_array&ta)
{
    if(!ta.consume(lexicon::TK::CONTINUE))throw exception::syntax_error("continueキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume(lexicon::TK::SCOLON))throw exception::syntax_error("不正なcontinue文です",ta.get_line(),ta.get_column());
    return std::make_shared<const _continue_>(ta.get_line(),ta.get_column());
}
std::shared_ptr<const _return_>_return_::get(lexicon::token_array&ta)
{
    if(!ta.consume(lexicon::TK::RETURN))throw exception::syntax_error("returnキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    auto ret=std::make_shared<_return_>(ta.get_line(),ta.get_column());
    ret->value=expression::get(ta);
    if(!ta.consume(lexicon::TK::SCOLON))throw exception::syntax_error("不正なreturn文です",ta.get_line(),ta.get_column());
    return ret;
}
std::shared_ptr<const define_function>define_function::get(lexicon::token_array&ta)
{
    if(!ta.consume(lexicon::TK::INT))throw exception::syntax_error("関数の型が見つかりませんでした",ta.get_line(),ta.get_column());
    auto ret=std::make_shared<define_function>(ta.get_line(),ta.get_column());
    if(auto fidp=std::dynamic_pointer_cast<const lexicon::identifier>(ta.consume(lexicon::TK::IDENT)))
        ret->name=fidp->name;
    else
        throw exception::syntax_error("関数名が見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume(lexicon::TK::OPARENT))throw exception::syntax_error("引数リストが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume(lexicon::TK::CPARENT)){
        while(true){
            if(!ta.consume(lexicon::TK::INT))
                throw exception::syntax_error("引数の型が見つかりませんでした",ta.get_line(),ta.get_column());
            if(auto idp=std::dynamic_pointer_cast<const lexicon::identifier>(ta.consume(lexicon::TK::IDENT)))
                ret->args.push_back(idp->name);
            else
                throw exception::syntax_error("引数名が見つかりませんでした",ta.get_line(),ta.get_column());
            if(ta.consume(lexicon::TK::COMMA))
                continue;
            else if(ta.consume(lexicon::TK::CPARENT))
                break;
            else
                throw exception::syntax_error("不正な区切り文字です",ta.get_line(),ta.get_column());
        }
    }
    if(!ta.consume(lexicon::TK::OBRACE))
        throw exception::syntax_error("関数の開始ブラケットが見つかりません",ta.get_line(),ta.get_column());
    while(!ta.consume(lexicon::TK::CBRACE))ret->stats.push_back(statement::get(ta));
    return ret;
}
void numeric::check(semantics::analyzer&analy)const noexcept
{
    return;
}
void identifier::check(semantics::analyzer&analy)const
{
    if(!analy.is_available_var(name))throw exception::semantic_error("未定義の変数です: "+name,line,col);
}
void fcall::check(semantics::analyzer&analy)const
{
    if(!analy.is_available_func(func->name,vars.size())){
        if(func->name!="__builtin_print_u64"||vars.size()!=1)
            throw exception::semantic_error("未定義の関数です: "+func->name,line,col);
    }
    for(auto v:vars)v->check(analy);
}
void unopr::check(semantics::analyzer&analy)const
{
    arg->check(analy);
}
void unopr_l::check(semantics::analyzer&analy)const
{
    arg->check(analy);
}
void biopr::check(semantics::analyzer&analy)const
{
    larg->check(analy);
    rarg->check(analy);
}
void biopr_l::check(semantics::analyzer&analy)const
{
    larg->check(analy);
    rarg->check(analy);
}
void expression_statement::check(semantics::analyzer&analy)const
{
    expr->check(analy);
}
void null_statement::check(semantics::analyzer&analy)const
{
    return;
}
void compound::check(semantics::analyzer&analy)const
{
    analy.enter_scope();
    for(auto s:stats)s->check(analy);
    analy.leave_scope();
}
void define_var::check(semantics::analyzer&analy)const
{
    for(auto v:vars){
        if(!analy.is_definable_var(v.first))throw exception::semantic_error("二重定義されました: "+v.first,line,col);
        analy.define_var(v.first);
        if(v.second)v.second->check(analy);
    }
}
void _if_else_::check(semantics::analyzer&analy)const
{
    analy.enter_scope();
    cond->check(analy);
    stat_if->check(analy);
    if(stat_else)stat_else->check(analy);
    analy.leave_scope();
}
void _while_::check(semantics::analyzer&analy)const
{
    analy.enter_scope();
    analy.enter_break();
    analy.enter_continue();
    cond->check(analy);
    stat->check(analy);
    analy.leave_continue();
    analy.leave_break();
    analy.leave_scope();
}
void _for_::check(semantics::analyzer&analy)const
{
    analy.enter_scope();
    analy.enter_break();
    analy.enter_continue();
    if(init)init->check(analy);
    if(cond)cond->check(analy);
    if(reinit)reinit->check(analy);
    stat->check(analy);
    analy.leave_continue();
    analy.leave_break();
    analy.leave_scope();
}
void _break_::check(semantics::analyzer&analy)const
{
    if(!analy.is_breakable())throw exception::semantic_error("不適切なbreak文です",line,col);
}
void _continue_::check(semantics::analyzer&analy)const
{
    if(!analy.is_continuable())throw exception::semantic_error("不適切なcontinue文です",line,col);
}
void _return_::check(semantics::analyzer&analy)const
{
    value->check(analy);
}
void define_function::check(semantics::analyzer&analy)const
{
    if(!analy.is_definable_func(name))throw exception::semantic_error("二重定義されました: "+name,line,col);
    analy.define_func(name,args.size());
    analy.enter_scope();
    for(auto a:args)analy.define_var(a);
    for(auto s:stats)s->check(analy);
    analy.leave_scope();
}
void numeric::to_asm(code::generator&gen)const
{
    gen.write("mov",value,"%rax");
}
void identifier::to_asm(code::generator&gen)const
{
    gen.write("mov",code::generator::to_address(-gen.get_offset(name),"%rbp"),"%rax");
}
void identifier::refer(code::generator&gen)const
{
    gen.write("lea",code::generator::to_address(-gen.get_offset(name),"%rbp"),"%rax");
}
void fcall::to_asm(code::generator&gen)const
{
    for(int i=vars.size()-1;i>=0;--i){
        vars[i]->to_asm(gen);
        gen.write("push","%rax");
    }
    for(int i=0;i<std::min(6ul,vars.size());++i){
        switch(i){
            case 0 :gen.write("pop","%rdi");break;
            case 1 :gen.write("pop","%rsi");break;
            case 2 :gen.write("pop","%rdx");break;
            case 3 :gen.write("pop","%rcx");break;
            case 4 :gen.write("pop","%r8"); break;
            case 5 :gen.write("pop","%r9"); break;
        }
    }
    // TODO: 呼び出し規約がよくわからん
    //int align=(16-gen.get_var_size()%16)%16;
    //gen.write("sub",align,"%rsp");
    gen.write("call",func->name);
    //gen.write("add",align,"%rsp");
    if(vars.size()>6)gen.write("add",8*(vars.size()-6),"%rsp");
}
void uplus::to_asm(code::generator&gen)const
{
    arg->to_asm(gen);
}
void uminus::to_asm(code::generator&gen)const
{
    // TODO: imul命令を使うべきか検証する
    arg->to_asm(gen);
    gen.write("mov","%rax","%rdi");
    gen.write("mov",2,"%rsi");
    gen.write("mul","%rsi");
    gen.write("sub","%rax","%rdi");
    gen.write("mov","%rdi","%rax");
}
void lognot::to_asm(code::generator&gen)const
{
    arg->to_asm(gen);
    gen.write("cmp",0,"%rax");
    gen.write("sete","%al");
    gen.write("movzb","%al","%rax");
}
void preinc::to_asm(code::generator&gen)const
{
    arg->refer(gen);
    gen.write("add",1,code::generator::to_address("%rax"));
    gen.write("mov",code::generator::to_address("%rax"),"%rax");
}
void predec::to_asm(code::generator&gen)const
{
    arg->refer(gen);
    gen.write("sub",1,code::generator::to_address("%rax"));
    gen.write("mov",code::generator::to_address("%rax"),"%rax");
}
void postinc::to_asm(code::generator&gen)const
{
    arg->refer(gen);
    gen.write("push",code::generator::to_address("%rax"));
    gen.write("add",1,code::generator::to_address("%rax"));
    gen.write("pop","%rax");
}
void postdec::to_asm(code::generator&gen)const
{
    arg->refer(gen);
    gen.write("push",code::generator::to_address("%rax"));
    gen.write("sub",1,code::generator::to_address("%rax"));
    gen.write("pop","%rax");
}
void bplus::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("add","%rdi","%rax");
}
void bminus::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("sub","%rdi","%rax");
}
void multiply::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("mul","%rdi");
}
void divide::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("mov",0,"%rdx");
    gen.write("div","%rdi");
}
void remain::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("mov",0,"%rdx");
    gen.write("div","%rdi");
    gen.write("mov","%rdx","%rax");
}
void equal::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("cmp","%rdi","%rax");
    gen.write("sete","%al");
    gen.write("movzb","%al","%rax");
}
void nequal::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("cmp","%rdi","%rax");
    gen.write("setne","%al");
    gen.write("movzb","%al","%rax");
}
void less::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("cmp","%rdi","%rax");
    gen.write("setl","%al");
    gen.write("movzb","%al","%rax");
}
void greater::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("cmp","%rdi","%rax");
    gen.write("setg","%al");
    gen.write("movzb","%al","%rax");
}
void leeq::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("cmp","%rdi","%rax");
    gen.write("setle","%al");
    gen.write("movzb","%al","%rax");
}
void greq::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("cmp","%rdi","%rax");
    gen.write("setge","%al");
    gen.write("movzb","%al","%rax");
}
void logand::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("and","%rdi","%rax");
    gen.write("cmp",0,"%rax");
    gen.write("setne","%al");
    gen.write("movzb","%al","%rax");
}
void logor::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("or","%rdi","%rax");
    gen.write("cmp",0,"%rax");
    gen.write("setne","%al");
    gen.write("movzb","%al","%rax");
}
void comma::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
}
void assign::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->refer(gen);
    gen.write("pop","%rdi");
    gen.write("mov","%rdi",code::generator::to_address("%rax"));
    gen.write("mov",code::generator::to_address("%rax"),"%rax");
}
void plasgn::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->refer(gen);
    gen.write("pop","%rdi");
    gen.write("add","%rdi",code::generator::to_address("%rax"));
    gen.write("mov",code::generator::to_address("%rax"),"%rax");
}
void miasgn::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->refer(gen);
    gen.write("pop","%rdi");
    gen.write("sub","%rdi",code::generator::to_address("%rax"));
    gen.write("mov",code::generator::to_address("%rax"),"%rax");
}
void muasgn::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->refer(gen);
    gen.write("pop","%rdi");
    gen.write("mov","%rax","%rsi");
    gen.write("mov",code::generator::to_address("%rax"),"%rax");
    gen.write("mul","%rdi");
    gen.write("mov","%rax",code::generator::to_address("%rsi"));
}
void diasgn::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->refer(gen);
    gen.write("pop","%rdi");
    gen.write("mov","%rax","%rsi");
    gen.write("mov",code::generator::to_address("%rax"),"%rax");
    gen.write("mov",0,"%rdx");
    gen.write("div","%rdi");
    gen.write("mov","%rax",code::generator::to_address("%rsi"));
}
void rmasgn::to_asm(code::generator&gen)const
{
    rarg->to_asm(gen);
    gen.write("push","%rax");
    larg->refer(gen);
    gen.write("pop","%rdi");
    gen.write("mov","%rax","%rsi");
    gen.write("mov",code::generator::to_address("%rax"),"%rax");
    gen.write("mov",0,"%rdx");
    gen.write("div","%rdi");
    gen.write("mov","%rdx",code::generator::to_address("%rsi"));
    gen.write("mov","%rdx","%rax");
}
void expression_statement::to_asm(code::generator&gen)const
{
    expr->to_asm(gen);
}
void null_statement::to_asm(code::generator&gen)const
{
    gen.write("nop");
}
void compound::to_asm(code::generator&gen)const
{
    gen.enter_scope();
    for(auto s:stats)s->to_asm(gen);
    gen.write("add",gen.leave_scope(),"%rsp");
}
void define_var::to_asm(code::generator&gen)const
{
    for(auto v:vars){
        gen.write("sub",8,"%rsp");
        gen.set_offset(v.first);
        if(v.second){
            v.second->to_asm(gen);
            gen.write("mov","%rax",code::generator::to_address("%rsp"));
        }
    }
}
void _if_else_::to_asm(code::generator&gen)const
{
    std::string lelse=code::generator::get_unique_label(".Lieelse");
    std::string lend=code::generator::get_unique_label(".Lieend");
    gen.enter_scope();
    cond->to_asm(gen);
    gen.write("cmp",0,"%rax");
    gen.write("je",lelse);
    stat_if->to_asm(gen);
    gen.write("jmp",lend);
    gen.write(lelse+':');
    if(stat_else)stat_else->to_asm(gen);
    gen.write(lend+':');
    gen.write("add",gen.leave_scope(),"%rsp");
}
void _while_::to_asm(code::generator&gen)const
{
    std::string lbegin=code::generator::get_unique_label(".Lwbegin");
    std::string lend=code::generator::get_unique_label(".Lwend");
    gen.enter_scope();
    gen.enter_break(lend);
    gen.enter_continue(lbegin);
    gen.write(lbegin+':');
    cond->to_asm(gen);
    gen.write("cmp",0,"%rax");
    gen.write("je",lend);
    stat->to_asm(gen);
    gen.write("jmp",lbegin);
    gen.write(lend+':');
    gen.leave_continue();
    gen.leave_break();
    gen.write("add",gen.leave_scope(),"%rsp");
}
void _for_::to_asm(code::generator&gen)const
{
    std::string lbegin=code::generator::get_unique_label(".Lfbegin");
    std::string lreini=code::generator::get_unique_label(".Lfreini");
    std::string lend=code::generator::get_unique_label(".Lfend");
    gen.enter_scope();
    gen.enter_break(lend);
    gen.enter_continue(lreini);
    if(init)init->to_asm(gen);
    gen.write(lbegin+':');
    gen.write("mov",1,"%rax");
    if(cond)cond->to_asm(gen);
    gen.write("cmp",0,"%rax");
    gen.write("je",lend);
    stat->to_asm(gen);
    gen.write(lreini+':');
    if(reinit)reinit->to_asm(gen);
    gen.write("jmp",lbegin);
    gen.write(lend+':');
    gen.leave_continue();
    gen.leave_break();
    gen.write("add",gen.leave_scope(),"%rsp");
}
void _break_::to_asm(code::generator&gen)const
{
    gen.write("jmp",gen.get_break_label());
}
void _continue_::to_asm(code::generator&gen)const
{
    gen.write("jmp",gen.get_continue_label());
}
void _return_::to_asm(code::generator&gen)const
{
    value->to_asm(gen);
    gen.write("mov","%rbp","%rsp");
    gen.write("pop","%rbp");
    gen.write("ret");
}
void define_function::to_asm(code::generator&gen)const
{
    gen.enter_scope();
    gen.write(".globl "+name);
    gen.write(name+':');
    gen.write("push","%rbp");
    gen.write("mov","%rsp","%rbp");
    gen.write("sub",args.size()*8,"%rsp");
    for(int i=0;i<args.size();++i){
        std::string dst=code::generator::to_address(i+1-args.size(),"%rsp");
        gen.set_offset(args[i]);
        switch(i){
            case 0 :gen.write("mov","%rdi",dst);break;
            case 1 :gen.write("mov","%rsi",dst);break;
            case 2 :gen.write("mov","%rdx",dst);break;
            case 3 :gen.write("mov","%rcx",dst);break;
            case 4 :gen.write("mov","%r8" ,dst);break;
            case 5 :gen.write("mov","%r9" ,dst);break;
            default:gen.write("mov",code::generator::to_address(8*(i-6)+16,"%rbp"),dst);break;
        }
    }
    // TODO: com内で必ずreturnすることを前提にしている問題を解決する
    for(auto s:stats)s->to_asm(gen);
    gen.leave_scope();
}
node::node(int line,int col)
    :line(line),col(col){}
numeric::numeric(int value,int line,int col)
    :expression(line,col),value(value){}
identifier::identifier(const std::string&name,int line,int col)
    :expression(line,col),name(name){}
fcall::fcall(const std::shared_ptr<const expression>&func,const std::vector<std::shared_ptr<const expression>>&vars,int line,int col)
    :expression(line,col),func(std::dynamic_pointer_cast<const identifier>(func)),vars(vars)
{
    if(!(this->func)){
        throw exception::unsupported_error("関数名が識別子ではありません",line,col);
    }
}
unopr::unopr(const std::shared_ptr<const expression>&arg,int line,int col)
    :expression(line,col),arg(arg){}
unopr_l::unopr_l(const std::shared_ptr<const expression>&arg,int line,int col)
    :expression(line,col),arg(std::dynamic_pointer_cast<const identifier>(arg))
{
    if(!(this->arg)){
        throw exception::unsupported_error("引数が識別子ではありません",line,col);
    }
}
biopr::biopr(const std::shared_ptr<const expression>&larg,const std::shared_ptr<const expression>&rarg,int line,int col)
    :expression(line,col),larg(larg),rarg(rarg){}
biopr_l::biopr_l(const std::shared_ptr<const expression>&larg,const std::shared_ptr<const expression>&rarg,int line,int col)
    :expression(line,col),larg(std::dynamic_pointer_cast<const identifier>(larg)),rarg(rarg)
{
    if(!(this->larg)){
        throw exception::unsupported_error("右引数が識別子ではありません",line,col);
    }
}
