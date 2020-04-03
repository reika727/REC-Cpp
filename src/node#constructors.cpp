#include"node.hpp"
#include"compilation_error.hpp"
using namespace syntax;
node::node(int line,int col)
    :line(line),col(col){}
numeric::numeric(int value,int line,int col)
    :expression(line,col),value(value){}
identifier::identifier(const std::string&name,int line,int col)
    :expression_l(line,col),name(name){}
fcall::fcall(std::unique_ptr<const expression>_func,std::vector<std::unique_ptr<const expression>>&_vars,int line,int col)
    :expression(line,col)
{
    auto row=_func.release();
    if(auto casted=dynamic_cast<const identifier*>(row)){
        func=std::unique_ptr<const identifier>(casted);
        std::move(_vars.begin(),_vars.end(),std::back_inserter(vars));
    }else{
        delete row;
        throw exception::compilation_error("未実装機能: 関数名が識別子ではありません",line,col);
    }
}
unopr::unopr(std::unique_ptr<const expression>arg,int line,int col)
    :expression(line,col),arg(std::move(arg)){}
unopr_l::unopr_l(std::unique_ptr<const expression>_arg,int line,int col)
    :expression(line,col)
{
    auto row=_arg.release();
    if(auto casted=dynamic_cast<const expression_l*>(row)){
        arg=std::unique_ptr<const expression_l>(casted);
    }else{
        delete row;
        throw exception::compilation_error("引数が左辺値ではありません",line,col);
    }
}
biopr::biopr(std::unique_ptr<const expression>larg,std::unique_ptr<const expression>rarg,int line,int col)
    :expression(line,col),larg(std::move(larg)),rarg(std::move(rarg)){}
biopr_l::biopr_l(std::unique_ptr<const expression>_larg,std::unique_ptr<const expression>rarg,int line,int col)
    :expression(line,col),rarg(std::move(rarg))
{
    auto row=_larg.release();
    if(auto casted=dynamic_cast<const expression_l*>(row)){
        larg=std::unique_ptr<const expression_l>(casted);
    }else{
        delete row;
        throw exception::compilation_error("左引数が左辺値ではありません",line,col);
    }
}
expression_statement::expression_statement(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    expr=expression::get(ta);
    if(!ta.consume_symbol(lexicon::symbol::SCOLON))
        throw exception::compilation_error("セミコロンが見つかりませんでした",ta.get_line(),ta.get_column());
}
null_statement::null_statement(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume_symbol(lexicon::symbol::SCOLON))
        throw exception::compilation_error("セミコロンが見つかりませんでした",ta.get_line(),ta.get_column());
}
compound::compound(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume_symbol(lexicon::symbol::OBRACE))
        throw exception::compilation_error("複文の開始ブラケットが見つかりませんでした",ta.get_line(),ta.get_column());
    while(!ta.consume_symbol(lexicon::symbol::CBRACE))stats.push_back(statement::get(ta));
}
var_difinition::var_difinition(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume_symbol(lexicon::symbol::INT))
        throw exception::compilation_error("型指定子が見つかりませんでした",ta.get_line(),ta.get_column());
    while(true){
        if(auto idp=ta.consume_identifier())
            vars.push_back(
                std::make_pair(
                    std::make_unique<const identifier>(idp->name,idp->line,idp->col),
                    ta.consume_symbol(lexicon::symbol::EQUAL)?expression::get(ta,true):nullptr
                )
            );
        else
            throw exception::compilation_error("変数名が見つかりませんでした",ta.get_line(),ta.get_column());
        if(ta.consume_symbol(lexicon::symbol::SCOLON))
            break;
        else if(!ta.consume_symbol(lexicon::symbol::COMMA))
            throw exception::compilation_error("不正な区切り文字です",ta.get_line(),ta.get_column());
    }
}
_if_else_::_if_else_(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume_symbol(lexicon::symbol::IF))
        throw exception::compilation_error("ifキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume_symbol(lexicon::symbol::OPARENT))
        throw exception::compilation_error("ifの後ろに括弧がありません",ta.get_line(),ta.get_column());
    cond=expression::get(ta);
    if(!ta.consume_symbol(lexicon::symbol::CPARENT))
        throw exception::compilation_error("ifの後ろに括弧がありません",ta.get_line(),ta.get_column());
    stat_if=statement::get(ta);
    stat_else=ta.consume_symbol(lexicon::symbol::ELSE)?statement::get(ta):nullptr;
}
_while_::_while_(lexicon::token_array&ta)
    :iteration_statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume_symbol(lexicon::symbol::WHILE))
        throw exception::compilation_error("whileキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume_symbol(lexicon::symbol::OPARENT))
        throw exception::compilation_error("whileの後ろに括弧がありません",ta.get_line(),ta.get_column());
    cond=expression::get(ta);
    if(!ta.consume_symbol(lexicon::symbol::CPARENT))
        throw exception::compilation_error("whileの後ろに括弧がありません",ta.get_line(),ta.get_column());
    stat=statement::get(ta);
}
_for_::_for_(lexicon::token_array&ta)
    :iteration_statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume_symbol(lexicon::symbol::FOR))
        throw exception::compilation_error("forキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume_symbol(lexicon::symbol::OPARENT))
        throw exception::compilation_error("forの後ろに括弧がありません",ta.get_line(),ta.get_column());
    init=ta.check_symbol(lexicon::symbol::SCOLON)?nullptr:expression::get(ta);
    if(!ta.consume_symbol(lexicon::symbol::SCOLON))
        throw exception::compilation_error("セミコロンが見つかりませんでした",ta.get_line(),ta.get_column());
    cond=ta.check_symbol(lexicon::symbol::SCOLON)?nullptr:expression::get(ta);
    if(!ta.consume_symbol(lexicon::symbol::SCOLON))
        throw exception::compilation_error("セミコロンが見つかりませんでした",ta.get_line(),ta.get_column());
    reinit=ta.check_symbol(lexicon::symbol::CPARENT)?nullptr:expression::get(ta);
    if(!ta.consume_symbol(lexicon::symbol::CPARENT))
        throw exception::compilation_error("forの後ろに括弧がありません",ta.get_line(),ta.get_column());
    stat=statement::get(ta);
}
_break_::_break_(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume_symbol(lexicon::symbol::BREAK))
        throw exception::compilation_error("breakキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume_symbol(lexicon::symbol::SCOLON))
        throw exception::compilation_error("不正なbreak文です",ta.get_line(),ta.get_column());
}
_continue_::_continue_(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume_symbol(lexicon::symbol::CONTINUE))
        throw exception::compilation_error("continueキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume_symbol(lexicon::symbol::SCOLON))
        throw exception::compilation_error("不正なcontinue文です",ta.get_line(),ta.get_column());
}
_return_::_return_(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume_symbol(lexicon::symbol::RETURN))
        throw exception::compilation_error("returnキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    value=expression::get(ta);
    if(!ta.consume_symbol(lexicon::symbol::SCOLON))
        throw exception::compilation_error("不正なreturn文です",ta.get_line(),ta.get_column());
}
function_difinition::function_difinition(lexicon::token_array&ta)
    :node(ta.get_line(),ta.get_column())
{
    if(!ta.consume_symbol(lexicon::symbol::INT))
        throw exception::compilation_error("関数の型が見つかりませんでした",ta.get_line(),ta.get_column());
    if(auto fidp=ta.consume_identifier())
        name=fidp->name;
    else
        throw exception::compilation_error("関数名が見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume_symbol(lexicon::symbol::OPARENT))
        throw exception::compilation_error("引数リストが見つかりませんでした",ta.get_line(),ta.get_column());
    if(ta.consume_symbol(lexicon::symbol::VOID)){
        if(!ta.consume_symbol(lexicon::symbol::CPARENT))
            throw exception::compilation_error("不正な引数リストです",ta.get_line(),ta.get_column());
    }else{
        while(true){
            if(!ta.consume_symbol(lexicon::symbol::INT))
                throw exception::compilation_error("引数の型が見つかりませんでした",ta.get_line(),ta.get_column());
            if(auto idp=ta.consume_identifier())
                args.push_back(std::make_unique<const identifier>(idp->name,idp->line,idp->col));
            else
                throw exception::compilation_error("引数名が見つかりませんでした",ta.get_line(),ta.get_column());
            if(ta.consume_symbol(lexicon::symbol::COMMA))
                continue;
            else if(ta.consume_symbol(lexicon::symbol::CPARENT))
                break;
            else
                throw exception::compilation_error("不正な区切り文字です",ta.get_line(),ta.get_column());
        }
    }
    if(!ta.consume_symbol(lexicon::symbol::OBRACE))
        throw exception::compilation_error("関数の開始ブラケットが見つかりません",ta.get_line(),ta.get_column());
    while(!ta.consume_symbol(lexicon::symbol::CBRACE))stats.push_back(statement::get(ta));
}
translation_unit::translation_unit(lexicon::token_array&ta)
    :node(ta.get_line(),ta.get_column())
{
    while(!ta.is_all_read())funcs.push_back(std::make_unique<const function_difinition>(ta));
}
