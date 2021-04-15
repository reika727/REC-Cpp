#include "common.hpp"
#include "compilation_error.hpp"
#include "node.hpp"
#include <algorithm>
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
void numeric::to_asm(code::writer &wr) const
{
    wr.write("mov", value, "%rax");
}
void identifier::to_asm(code::writer &wr) const
{
    wr.write("mov", get_address(), "%rax");
}
void fcall::to_asm(code::writer &wr) const
{
    if (vars.size() > 6) {
        wr.write("sub", 8 * (vars.size() - 6), "%rsp");
    }
    for (int i = std::ssize(vars) - 1; i >= 0; --i) {
        auto argument_storage = [i] {
            return i < 6 ? std::vector{"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"}[i]
                         : format("%d(%%rsp)", 8 * (i - 6));
        };
        vars[i]->to_asm(wr);
        wr.write("mov", "%rax", argument_storage());
    }
    /// @todo System V ABIに従いスタックフレームを調整する
    wr.write("call", func->name);
    if (vars.size() > 6) {
        wr.write("add", 8 * (vars.size() - 6), "%rsp");
    }
}
void uplus::to_asm(code::writer &wr) const
{
    arg->to_asm(wr);
}
void uminus::to_asm(code::writer &wr) const
{
    arg->to_asm(wr);
    wr.write("neg", "%rax");
}
void lognot::to_asm(code::writer &wr) const
{
    arg->to_asm(wr);
    wr.write("cmp", 0, "%rax");
    wr.write("sete", "%al");
    wr.write("movzb", "%al", "%rax");
}
void preinc::to_asm(code::writer &wr) const
{
    wr.write("incq", arg->get_address());
    arg->to_asm(wr);
}
void predec::to_asm(code::writer &wr) const
{
    wr.write("decq", arg->get_address());
    arg->to_asm(wr);
}
void postinc::to_asm(code::writer &wr) const
{
    arg->to_asm(wr);
    wr.write("incq", arg->get_address());
}
void postdec::to_asm(code::writer &wr) const
{
    arg->to_asm(wr);
    wr.write("decq", arg->get_address());
}
void bplus::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("add", "%rdi", "%rax");
}
void bminus::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("sub", "%rdi", "%rax");
}
void multiply::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("imul", "%rdi");
}
void divide::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("xor", "%rdx", "%rdx");
    wr.write("idiv", "%rdi");
}
void remain::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("xor", "%rdx", "%rdx");
    wr.write("idiv", "%rdi");
    wr.write("mov", "%rdx", "%rax");
}
void equal::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("cmp", "%rdi", "%rax");
    wr.write("sete", "%al");
    wr.write("movzb", "%al", "%rax");
}
void nequal::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("cmp", "%rdi", "%rax");
    wr.write("setne", "%al");
    wr.write("movzb", "%al", "%rax");
}
void less::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("cmp", "%rdi", "%rax");
    wr.write("setl", "%al");
    wr.write("movzb", "%al", "%rax");
}
void greater::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("cmp", "%rdi", "%rax");
    wr.write("setg", "%al");
    wr.write("movzb", "%al", "%rax");
}
void leeq::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("cmp", "%rdi", "%rax");
    wr.write("setle", "%al");
    wr.write("movzb", "%al", "%rax");
}
void greq::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("cmp", "%rdi", "%rax");
    wr.write("setge", "%al");
    wr.write("movzb", "%al", "%rax");
}
void logand::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("and", "%rdi", "%rax");
    wr.write("cmp", 0, "%rax");
    wr.write("setne", "%al");
    wr.write("movzb", "%al", "%rax");
}
void logor::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("or", "%rdi", "%rax");
    wr.write("cmp", 0, "%rax");
    wr.write("setne", "%al");
    wr.write("movzb", "%al", "%rax");
}
void comma::to_asm(code::writer &wr) const
{
    larg->to_asm(wr);
    rarg->to_asm(wr);
}
void assign::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("mov", "%rax", larg->get_address());
}
void plasgn::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("add", "%rax", larg->get_address());
    larg->to_asm(wr);
}
void miasgn::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("sub", "%rax", larg->get_address());
    larg->to_asm(wr);
}
void muasgn::to_asm(code::writer &wr) const
{
    std::string addr = larg->get_address();
    rarg->to_asm(wr);
    wr.write("imulq", addr);
    wr.write("mov", "%rax", addr);
}
void diasgn::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("xor", "%rdx", "%rdx");
    wr.write("idiv", "%rdi");
    wr.write("mov", "%rax", larg->get_address());
}
void rmasgn::to_asm(code::writer &wr) const
{
    rarg->to_asm(wr);
    wr.write("push", "%rax");
    larg->to_asm(wr);
    wr.write("pop", "%rdi");
    wr.write("xor", "%rdx", "%rdx");
    wr.write("idiv", "%rdi");
    wr.write("mov", "%rdx", larg->get_address());
    wr.write("mov", "%rdx", "%rax");
}
void expression_statement::to_asm(code::writer &wr) const
{
    expr->to_asm(wr);
}
void null_statement::to_asm(code::writer &wr) const
{
    wr.write("nop");
}
void compound::to_asm(code::writer &wr) const
{
    enter_scope();
    for (const auto &s : stats) {
        s->to_asm(wr);
    }
    wr.write("add", leave_scope(), "%rsp");
}
void var_definition::to_asm(code::writer &wr) const
{
    wr.write("sub", 8 * vars.size(), "%rsp");
    for (const auto &[id, expr] : vars) {
        id->allocate_on_stack();
        if (expr) {
            expr->to_asm(wr);
            wr.write("mov", "%rax", id->get_address());
        }
    }
}
void _if_else_::to_asm(code::writer &wr) const
{
    std::string lelse = code::writer::get_unique_label(".Lieelse");
    std::string lend = code::writer::get_unique_label(".Lieend");
    enter_scope();
    cond->to_asm(wr);
    wr.write("cmp", 0, "%rax");
    wr.write("je", lelse);
    stat_if->to_asm(wr);
    wr.write("jmp", lend);
    wr.write(lelse + ':');
    if (stat_else) {
        stat_else->to_asm(wr);
    }
    wr.write(lend + ':');
    wr.write("add", leave_scope(), "%rsp");
}
void _while_::to_asm(code::writer &wr) const
{
    std::string lbegin = code::writer::get_unique_label(".Lwbegin");
    std::string lend = code::writer::get_unique_label(".Lwend");
    enter_scope();
    enter_break(lend);
    enter_continue(lbegin);
    wr.write(lbegin + ':');
    cond->to_asm(wr);
    wr.write("cmp", 0, "%rax");
    wr.write("je", lend);
    stat->to_asm(wr);
    wr.write("jmp", lbegin);
    wr.write(lend + ':');
    leave_continue();
    leave_break();
    wr.write("add", leave_scope(), "%rsp");
}
void _for_::to_asm(code::writer &wr) const
{
    std::string lbegin = code::writer::get_unique_label(".Lfbegin");
    std::string lreini = code::writer::get_unique_label(".Lfreini");
    std::string lend = code::writer::get_unique_label(".Lfend");
    enter_scope();
    enter_break(lend);
    enter_continue(lreini);
    if (init) {
        init->to_asm(wr);
    }
    wr.write(lbegin + ':');
    if (cond) {
        cond->to_asm(wr);
        wr.write("cmp", 0, "%rax");
        wr.write("je", lend);
    }
    stat->to_asm(wr);
    wr.write(lreini + ':');
    if (reinit) {
        reinit->to_asm(wr);
    }
    wr.write("jmp", lbegin);
    wr.write(lend + ':');
    leave_continue();
    leave_break();
    wr.write("add", leave_scope(), "%rsp");
}
void _break_::to_asm(code::writer &wr) const
{
    if (iteration_statement::break_labels.empty()) {
        throw exception::compilation_error("不適切なbreak文です", line, col);
    }
    wr.write("jmp", iteration_statement::break_labels.top());
}
void _continue_::to_asm(code::writer &wr) const
{
    if (iteration_statement::continue_labels.empty()) {
        throw exception::compilation_error("不適切なcontinue文です", line, col);
    }
    wr.write("jmp", iteration_statement::continue_labels.top());
}
void _return_::to_asm(code::writer &wr) const
{
    value->to_asm(wr);
    wr.write("mov", "%rbp", "%rsp");
    wr.write("pop", "%rbp");
    wr.write("ret");
}
void function_definition::to_asm(code::writer &wr) const
{
    wr.write(name + ':');
    wr.write("push", "%rbp");
    wr.write("mov", "%rsp", "%rbp");
    wr.write("sub", std::min(6ul, args.size()) * 8, "%rsp");
    /// @todo 現在の実装では関数に入る際に2回スコープをくぐっている。要検証。
    enter_scope();
    for (std::size_t i = 0; i < args.size(); ++i) {
        // 6つめまでの引数はレジスタに入っているのでスタック上に新しく確保した領域にそれをコピー、
        // 7つめ以降の引数はすでにスタックに積まれているのでそのオフセットを記録
        if (i < 6) {
            args[i]->allocate_on_stack();
            wr.write("mov", std::vector{"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"}[i], args[i]->get_address());
        } else {
            args[i]->allocate_on_stack(i * 8 - 32);
        }
    }
    /// @todo com内で必ずreturnすることを前提にしている問題を解決する
    comp->to_asm(wr);
    leave_scope();
}
void translation_unit::to_asm(code::writer &wr) const
{
    enter_scope();
    for (const auto &f : funcs) {
        f->to_asm(wr);
    }
    leave_scope();
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
