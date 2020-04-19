#include"node.hpp"
#include"compilation_error.hpp"
using namespace syntax;
node::node(int line,int col)
    :line(line),col(col){}
expression::expression(int line,int col,type_info type)
    :node(line,col),type(type){}
identifier::identifier(const std::string&name,int line,int col,type_info type)
    :expression_l(line,col,type),name(name){}
numeric::numeric(int value,int line,int col,type_info type)
    :expression(line,col,type),value(value){}
fcall::fcall(std::unique_ptr<const expression>_func,std::vector<std::unique_ptr<const expression>>&_vars,int line,int col,type_info type)
    :expression(line,col,type)
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
    :expression(line,col,determine_type(arg->type)),arg(std::move(arg)){}
unopr_l::unopr_l(std::unique_ptr<const expression>_arg,int line,int col)
    :expression(line,col,determine_type(arg->type))
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
    :expression(line,col,determine_type(larg->type,rarg->type)),larg(std::move(larg)),rarg(std::move(rarg)){}
biopr_l::biopr_l(std::unique_ptr<const expression>_larg,std::unique_ptr<const expression>rarg,int line,int col)
    :expression(line,col,determine_type(larg->type,rarg->type)),rarg(std::move(rarg))
{
    auto row=_larg.release();
    if(auto casted=dynamic_cast<const expression_l*>(row)){
        larg=std::unique_ptr<const expression_l>(casted);
    }else{
        delete row;
        throw exception::compilation_error("左引数が左辺値ではありません",line,col);
    }
}
expression_statement::expression_statement(lexicon::lexer&lx)
    :statement(lx.get_line(),lx.get_column())
{
    expr=expression::get(lx);
    if(!lx.consume_symbol_if(lexicon::symbol::SCOLON))
        throw exception::compilation_error("セミコロンが見つかりませんでした",lx.get_line(),lx.get_column());
}
null_statement::null_statement(lexicon::lexer&lx)
    :statement(lx.get_line(),lx.get_column())
{
    if(!lx.consume_symbol_if(lexicon::symbol::SCOLON))
        throw exception::compilation_error("セミコロンが見つかりませんでした",lx.get_line(),lx.get_column());
}
compound::compound(lexicon::lexer&lx)
    :statement(lx.get_line(),lx.get_column())
{
    if(!lx.consume_symbol_if(lexicon::symbol::OBRACE))
        throw exception::compilation_error("複文の開始ブラケットが見つかりませんでした",lx.get_line(),lx.get_column());
    while(!lx.consume_symbol_if(lexicon::symbol::CBRACE))stats.push_back(statement::get(lx));
}
var_difinition::var_difinition(lexicon::lexer&lx)
    :statement(lx.get_line(),lx.get_column())
{
    if(!lx.consume_symbol_if(lexicon::symbol::INT))
        throw exception::compilation_error("型指定子が見つかりませんでした",lx.get_line(),lx.get_column());
    while(true){
        if(auto idp=lx.consume_identifier())
            vars.push_back(
                std::make_pair(
                    std::make_unique<const identifier>(idp->name,idp->line,idp->col,expression::type_info::get_int()), // TODO: とりあえずintで固定
                    lx.consume_symbol_if(lexicon::symbol::EQUAL)?expression::get(lx,true):nullptr
                )
            );
        else
            throw exception::compilation_error("変数名が見つかりませんでした",lx.get_line(),lx.get_column());
        if(lx.consume_symbol_if(lexicon::symbol::SCOLON))
            break;
        else if(!lx.consume_symbol_if(lexicon::symbol::COMMA))
            throw exception::compilation_error("不正な区切り文字です",lx.get_line(),lx.get_column());
    }
}
_if_else_::_if_else_(lexicon::lexer&lx)
    :statement(lx.get_line(),lx.get_column())
{
    if(!lx.consume_symbol_if(lexicon::symbol::IF))
        throw exception::compilation_error("ifキーワードが見つかりませんでした",lx.get_line(),lx.get_column());
    if(!lx.consume_symbol_if(lexicon::symbol::OPARENT))
        throw exception::compilation_error("ifの後ろに括弧がありません",lx.get_line(),lx.get_column());
    cond=expression::get(lx);
    if(!lx.consume_symbol_if(lexicon::symbol::CPARENT))
        throw exception::compilation_error("ifの後ろに括弧がありません",lx.get_line(),lx.get_column());
    stat_if=statement::get(lx);
    stat_else=lx.consume_symbol_if(lexicon::symbol::ELSE)?statement::get(lx):nullptr;
}
_while_::_while_(lexicon::lexer&lx)
    :iteration_statement(lx.get_line(),lx.get_column())
{
    if(!lx.consume_symbol_if(lexicon::symbol::WHILE))
        throw exception::compilation_error("whileキーワードが見つかりませんでした",lx.get_line(),lx.get_column());
    if(!lx.consume_symbol_if(lexicon::symbol::OPARENT))
        throw exception::compilation_error("whileの後ろに括弧がありません",lx.get_line(),lx.get_column());
    cond=expression::get(lx);
    if(!lx.consume_symbol_if(lexicon::symbol::CPARENT))
        throw exception::compilation_error("whileの後ろに括弧がありません",lx.get_line(),lx.get_column());
    stat=statement::get(lx);
}
_for_::_for_(lexicon::lexer&lx)
    :iteration_statement(lx.get_line(),lx.get_column())
{
    if(!lx.consume_symbol_if(lexicon::symbol::FOR))
        throw exception::compilation_error("forキーワードが見つかりませんでした",lx.get_line(),lx.get_column());
    if(!lx.consume_symbol_if(lexicon::symbol::OPARENT))
        throw exception::compilation_error("forの後ろに括弧がありません",lx.get_line(),lx.get_column());
    init=lx.check_symbol(lexicon::symbol::SCOLON)?nullptr:expression::get(lx);
    if(!lx.consume_symbol_if(lexicon::symbol::SCOLON))
        throw exception::compilation_error("セミコロンが見つかりませんでした",lx.get_line(),lx.get_column());
    cond=lx.check_symbol(lexicon::symbol::SCOLON)?nullptr:expression::get(lx);
    if(!lx.consume_symbol_if(lexicon::symbol::SCOLON))
        throw exception::compilation_error("セミコロンが見つかりませんでした",lx.get_line(),lx.get_column());
    reinit=lx.check_symbol(lexicon::symbol::CPARENT)?nullptr:expression::get(lx);
    if(!lx.consume_symbol_if(lexicon::symbol::CPARENT))
        throw exception::compilation_error("forの後ろに括弧がありません",lx.get_line(),lx.get_column());
    stat=statement::get(lx);
}
_break_::_break_(lexicon::lexer&lx)
    :statement(lx.get_line(),lx.get_column())
{
    if(!lx.consume_symbol_if(lexicon::symbol::BREAK))
        throw exception::compilation_error("breakキーワードが見つかりませんでした",lx.get_line(),lx.get_column());
    if(!lx.consume_symbol_if(lexicon::symbol::SCOLON))
        throw exception::compilation_error("不正なbreak文です",lx.get_line(),lx.get_column());
}
_continue_::_continue_(lexicon::lexer&lx)
    :statement(lx.get_line(),lx.get_column())
{
    if(!lx.consume_symbol_if(lexicon::symbol::CONTINUE))
        throw exception::compilation_error("continueキーワードが見つかりませんでした",lx.get_line(),lx.get_column());
    if(!lx.consume_symbol_if(lexicon::symbol::SCOLON))
        throw exception::compilation_error("不正なcontinue文です",lx.get_line(),lx.get_column());
}
_return_::_return_(lexicon::lexer&lx)
    :statement(lx.get_line(),lx.get_column())
{
    if(!lx.consume_symbol_if(lexicon::symbol::RETURN))
        throw exception::compilation_error("returnキーワードが見つかりませんでした",lx.get_line(),lx.get_column());
    value=expression::get(lx);
    if(!lx.consume_symbol_if(lexicon::symbol::SCOLON))
        throw exception::compilation_error("不正なreturn文です",lx.get_line(),lx.get_column());
}
function_difinition::function_difinition(lexicon::lexer&lx)
    :node(lx.get_line(),lx.get_column())
{
    if(!lx.consume_symbol_if(lexicon::symbol::INT))
        throw exception::compilation_error("関数の型が見つかりませんでした",lx.get_line(),lx.get_column());
    if(auto fidp=lx.consume_identifier())
        name=fidp->name;
    else
        throw exception::compilation_error("関数名が見つかりませんでした",lx.get_line(),lx.get_column());
    if(!lx.consume_symbol_if(lexicon::symbol::OPARENT))
        throw exception::compilation_error("引数リストが見つかりませんでした",lx.get_line(),lx.get_column());
    if(lx.consume_symbol_if(lexicon::symbol::VOID)){
        if(!lx.consume_symbol_if(lexicon::symbol::CPARENT))
            throw exception::compilation_error("不正な引数リストです",lx.get_line(),lx.get_column());
    }else{
        while(true){
            if(!lx.consume_symbol_if(lexicon::symbol::INT))
                throw exception::compilation_error("引数の型が見つかりませんでした",lx.get_line(),lx.get_column());
            if(auto idp=lx.consume_identifier())
                args.push_back(std::make_unique<const identifier>(idp->name,idp->line,idp->col,expression::type_info::get_int())); // TODO: とりあえずintで固定
            else
                throw exception::compilation_error("引数名が見つかりませんでした",lx.get_line(),lx.get_column());
            if(lx.consume_symbol_if(lexicon::symbol::COMMA))
                continue;
            else if(lx.consume_symbol_if(lexicon::symbol::CPARENT))
                break;
            else
                throw exception::compilation_error("不正な区切り文字です",lx.get_line(),lx.get_column());
        }
    }
    if(!lx.consume_symbol_if(lexicon::symbol::OBRACE))
        throw exception::compilation_error("関数の開始ブラケットが見つかりません",lx.get_line(),lx.get_column());
    while(!lx.consume_symbol_if(lexicon::symbol::CBRACE))stats.push_back(statement::get(lx));
}
translation_unit::translation_unit(lexicon::lexer&lx)
    :node(lx.get_line(),lx.get_column())
{
    while(!lx.is_all_read())funcs.push_back(std::make_unique<const function_difinition>(lx));
}
