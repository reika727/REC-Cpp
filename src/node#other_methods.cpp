#include "compilation_error.hpp"
#include "node.hpp"
using namespace syntax;
std::string identifier::get_address() const
{
    for (auto &o : offset | std::views::reverse) {
        if (o.contains(name)) {
            return std::to_string(o[name]) + "(%rbp)";
        }
    }
    throw exception::compilation_error("未定義の変数です: " + name, line, col);
}
void identifier::allocate_on_stack() const
{
    allocate_on_stack(-(offset.back().size() + 1) * 8);
}
void identifier::allocate_on_stack(int off) const
{
    if (offset.back().count(name)) {
        throw exception::compilation_error("多重定義されました: " + name, line, col);
    }
    offset.back()[name] = off;
}
void node::enter_scope()
{
    offset.emplace_back();
}
int node::leave_scope()
{
    int ret = offset.back().size() * 8;
    offset.pop_back();
    return ret;
}
void iteration_statement::enter_break(const std::string &label)
{
    break_labels.push(label);
}
void iteration_statement::leave_break()
{
    break_labels.pop();
}
void iteration_statement::enter_continue(const std::string &label)
{
    continue_labels.push(label);
}
void iteration_statement::leave_continue()
{
    continue_labels.pop();
}
void function_definition::set_argument_list(lexicon::lexer &lx)
{
    if (!lx.consume_symbol_if(lexicon::symbol::symid::OPARENT)) {
        throw exception::compilation_error("引数リストが見つかりませんでした", lx.get_line(), lx.get_column());
    }
    if (lx.consume_symbol_if(lexicon::symbol::symid::VOID)) {
        if (!lx.consume_symbol_if(lexicon::symbol::symid::CPARENT)) {
            throw exception::compilation_error("不正な引数リストです", lx.get_line(), lx.get_column());
        }
    } else {
        while (true) {
            if (!lx.consume_type_specifier()) {
                throw exception::compilation_error("引数の型が見つかりませんでした", lx.get_line(), lx.get_column());
            }
            if (auto id = lx.consume_identifier()) {
                args.push_back(std::make_unique<const identifier>(id->name, id->line, id->col, expression::type_info::get_int())); /// @todo とりあえずintで固定
            } else {
                throw exception::compilation_error("引数名が見つかりませんでした", lx.get_line(), lx.get_column());
            }
            if (lx.consume_symbol_if(lexicon::symbol::symid::COMMA)) {
                continue;
            } else if (lx.consume_symbol_if(lexicon::symbol::symid::CPARENT)) {
                break;
            } else {
                throw exception::compilation_error("不正な区切り文字です", lx.get_line(), lx.get_column());
            }
        }
    }
}
/// @todo いずれはちゃんと実装してmaybe_unusedも取る
expression::type_info unopr::determine_type([[maybe_unused]] type_info arg_type)
{
    return type_info::get_int(); /// @todo とりあえずint
}
expression::type_info unopr_l::determine_type([[maybe_unused]] type_info arg_type)
{
    return type_info::get_int(); /// @todo とりあえずint
}
expression::type_info biopr::determine_type([[maybe_unused]] type_info larg_type, [[maybe_unused]] type_info rarg_type)
{
    return type_info::get_int(); /// @todo とりあえずint
}
expression::type_info biopr_l::determine_type([[maybe_unused]] type_info larg_type, [[maybe_unused]] type_info rarg_type)
{
    return type_info::get_int(); /// @todo とりあえずint
}
