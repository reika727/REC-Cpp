#include"syntax/node.hpp"
#include"exception/compilation_error.hpp"
#include<algorithm>
#include<typeinfo>
using namespace syntax;
void node::enter_scope()
{
    offset.emplace_back();
}
int node::leave_scope()
{
    int ret=offset.back().size()*8;
    offset.pop_back();
    return ret;
}
void iteration_statement::enter_break(const std::string&label)
{
    break_labels.push(label);
}
void iteration_statement::leave_break()
{
    break_labels.pop();
}
void iteration_statement::enter_continue(const std::string&label)
{
    continue_labels.push(label);
}
void iteration_statement::leave_continue()
{
    continue_labels.pop();
}
std::unique_ptr<const expression>expression::get(lexicon::token_array&ta,bool for_initialization)
{
    return for_initialization?get_order14(ta):get_order15(ta);
}
std::unique_ptr<const expression>expression::get_order15(lexicon::token_array&ta) // , left to right
{
    auto ret=get_order14(ta);
    while(auto tp=ta.consume(lexicon::TK::COMMA))
        ret=std::make_unique<const comma>(std::move(ret),get_order14(ta),tp->line,tp->col);
    return ret;
}
std::unique_ptr<const expression>expression::get_order14(lexicon::token_array&ta) // = += -= *= /= right to left
{
    auto ret=get_order13(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::EQUAL))
            ret=std::make_unique<const assign>(std::move(ret),get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::PLEQ))
            ret=std::make_unique<const plasgn>(std::move(ret),get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::MIEQ))
            ret=std::make_unique<const miasgn>(std::move(ret),get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::ASEQ))
            ret=std::make_unique<const muasgn>(std::move(ret),get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::SLEQ))
            ret=std::make_unique<const diasgn>(std::move(ret),get_order14(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::PEEQ))
            ret=std::make_unique<const rmasgn>(std::move(ret),get_order14(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order13(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order12(ta);
}
std::unique_ptr<const expression>expression::get_order12(lexicon::token_array&ta) // || left to right
{
    auto ret=get_order11(ta);
    while(auto tp=ta.consume(lexicon::TK::VBVB))
        ret=std::make_unique<const logor>(std::move(ret),get_order11(ta),tp->line,tp->col);
    return ret;
}
std::unique_ptr<const expression>expression::get_order11(lexicon::token_array&ta) // && left to right
{
    auto ret=get_order10(ta);
    while(auto tp=ta.consume(lexicon::TK::APAP))
        ret=std::make_unique<const logand>(std::move(ret),get_order10(ta),tp->line,tp->col);
    return ret;
}
std::unique_ptr<const expression>expression::get_order10(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order09(ta);
}
std::unique_ptr<const expression>expression::get_order09(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order08(ta);
}
std::unique_ptr<const expression>expression::get_order08(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order07(ta);
}
std::unique_ptr<const expression>expression::get_order07(lexicon::token_array&ta) // == != left to right
{
    auto ret=get_order06(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::EQEQ))
            ret=std::make_unique<const equal>(std::move(ret),get_order06(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::EXEQ))
            ret=std::make_unique<const nequal>(std::move(ret),get_order06(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order06(lexicon::token_array&ta) // < > <= >= left to right
{
    auto ret=get_order05(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::LESS))
            ret=std::make_unique<const less>(std::move(ret),get_order05(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::GREATER))
            ret=std::make_unique<const greater>(std::move(ret),get_order05(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::LEEQ))
            ret=std::make_unique<const leeq>(std::move(ret),get_order05(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::GREQ))
            ret=std::make_unique<const greq>(std::move(ret),get_order05(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order05(lexicon::token_array&ta)
{
    // TODO: placeholder
    return get_order04(ta);
}
std::unique_ptr<const expression>expression::get_order04(lexicon::token_array&ta) // + - left to right
{
    auto ret=get_order03(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::PLUS))
            ret=std::make_unique<const bplus>(std::move(ret),get_order03(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::MINUS))
            ret=std::make_unique<const bminus>(std::move(ret),get_order03(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order03(lexicon::token_array&ta) // * / % left to right
{
    auto ret=get_order02(ta);
    while(true){
        if(auto tp=ta.consume(lexicon::TK::ASTER))
            ret=std::make_unique<const multiply>(std::move(ret),get_order02(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::SLASH))
            ret=std::make_unique<const divide>(std::move(ret),get_order02(ta),tp->line,tp->col);
        else if(tp=ta.consume(lexicon::TK::PERCENT))
            ret=std::make_unique<const remain>(std::move(ret),get_order02(ta),tp->line,tp->col);
        else
            break;
    }
    return ret;
}
std::unique_ptr<const expression>expression::get_order02(lexicon::token_array&ta) // + - ++ -- ! right to left
{
    if(auto tp=ta.consume(lexicon::TK::PLUS))
        return std::make_unique<const uplus>(get_order02(ta),tp->line,tp->col);
    else if(tp=ta.consume(lexicon::TK::MINUS))
        return std::make_unique<const uminus>(get_order02(ta),tp->line,tp->col);
    else if(tp=ta.consume(lexicon::TK::PLPL))
        return std::make_unique<const preinc>(get_order02(ta),tp->line,tp->col);
    else if(tp=ta.consume(lexicon::TK::MIMI))
        return std::make_unique<const predec>(get_order02(ta),tp->line,tp->col);
    else if(tp=ta.consume(lexicon::TK::EXCLAM))
        return std::make_unique<const lognot>(get_order02(ta),tp->line,tp->col);
    else
        return get_order01(ta);
}
std::unique_ptr<const expression>expression::get_order01(lexicon::token_array&ta) // () left to right
{
    auto ret=get_primary(ta);
    if(auto tp=ta.consume(lexicon::TK::PLPL)){
        return std::make_unique<const postinc>(std::move(ret),tp->line,tp->col);
    }else if(tp=ta.consume(lexicon::TK::MIMI)){
        return std::make_unique<const postdec>(std::move(ret),tp->line,tp->col);
    }else if(tp=ta.consume(lexicon::TK::OPARENT)){
        auto vars=std::vector<std::unique_ptr<const expression>>();
        if(!ta.consume(lexicon::TK::CPARENT)){
            while(true){
                vars.push_back(get_order14(ta));
                if(ta.consume(lexicon::TK::CPARENT))
                    break;
                else if(!ta.consume(lexicon::TK::COMMA))
                    throw exception::compilation_error("関数呼び出し演算子のコンマに問題があります",tp->line,tp->col);
            }
        }
        return std::make_unique<const fcall>(std::move(ret),vars,tp->line,tp->col);
    }else{
        return ret;
    }
}
std::unique_ptr<const expression>expression::get_primary(lexicon::token_array&ta) // literal, identifier, enclosed expression
{
    if(auto nump=std::dynamic_pointer_cast<const lexicon::numeric>(ta.consume(lexicon::TK::NUMERIC))){
        return std::make_unique<const numeric>(nump->value,nump->line,nump->col);
    }else if(auto idp=std::dynamic_pointer_cast<const lexicon::identifier>(ta.consume(lexicon::TK::IDENT))){
        return std::make_unique<const identifier>(idp->name,idp->line,idp->col);
    }else if(auto tp=ta.consume(lexicon::TK::OPARENT)){
        auto ret=get_order15(ta);
        if(!ta.consume(lexicon::TK::CPARENT))
            throw exception::compilation_error("括弧の対応が正しくありません",tp->line,tp->col);
        return ret;
    }
    throw exception::compilation_error("構文解析ができませんでした",ta.get_line(),ta.get_column());
}
std::unique_ptr<const statement>statement::get(lexicon::token_array&ta)
{
    if(ta.check(lexicon::TK::INT))return std::make_unique<const var_difinition>(ta);
    else if(ta.check(lexicon::TK::IF))return std::make_unique<const _if_else_>(ta);
    else if(ta.check(lexicon::TK::WHILE))return std::make_unique<const _while_>(ta);
    else if(ta.check(lexicon::TK::FOR))return std::make_unique<const _for_>(ta);
    else if(ta.check(lexicon::TK::BREAK))return std::make_unique<const _break_>(ta);
    else if(ta.check(lexicon::TK::CONTINUE))return std::make_unique<const _continue_>(ta);
    else if(ta.check(lexicon::TK::RETURN))return std::make_unique<const _return_>(ta);
    else if(ta.check(lexicon::TK::OBRACE))return std::make_unique<const compound>(ta);
    else if(ta.check(lexicon::TK::SCOLON))return std::make_unique<const null_statement>(ta);
    else return std::make_unique<const expression_statement>(ta);
}
void numeric::to_asm(code::writer&wr)const
{
    wr.write("mov",value,"%rax");
}
void identifier::to_asm(code::writer&wr)const
{
    wr.write("mov",get_address(),"%rax");
}
std::string identifier::get_address()const
{
    auto itr=std::find_if(
            offset.rbegin(),offset.rend(),
            [this](const std::map<std::string,int>&mp){
                return mp.count(name)==1;
            }
        );
    if(itr==offset.rend())
        throw exception::compilation_error("未定義の変数です: "+name,line,col);
    return std::to_string((*itr)[name])+"(%rbp)";
}
void identifier::allocate_on_stack()const
{
    allocate_on_stack(-(offset.back().size()+1)*8);
}
void identifier::allocate_on_stack(int off)const
{
    if(offset.back().count(name))
        throw exception::compilation_error("多重定義されました: "+name,line,col);
    offset.back()[name]=off;
}
void fcall::to_asm(code::writer&wr)const
{
    if(vars.size()>6)wr.write("sub",8*(vars.size()-6),"%rsp");
    for(int i=vars.size()-1;i>=6;--i){
        vars[i]->to_asm(wr);
        wr.write("mov","%rax",std::to_string(8*(i-6))+"(%rsp)");
    }
    for(int i=std::min(5ul,vars.size()-1);i>=0;--i){
        vars[i]->to_asm(wr);
        wr.write("mov","%rax",std::vector{"%rdi","%rsi","%rdx","%rcx","%r8","%r9"}[i]);
    }
    // TODO: System V ABIに従いスタックフレームを調整する
    wr.write("call",func->name);
    if(vars.size()>6)wr.write("add",8*(vars.size()-6),"%rsp");
}
void uplus::to_asm(code::writer&wr)const
{
    arg->to_asm(wr);
}
void uminus::to_asm(code::writer&wr)const
{
    arg->to_asm(wr);
    wr.write("neg","%rax");
}
void lognot::to_asm(code::writer&wr)const
{
    arg->to_asm(wr);
    wr.write("cmp",0,"%rax");
    wr.write("sete","%al");
    wr.write("movzb","%al","%rax");
}
void preinc::to_asm(code::writer&wr)const
{
    wr.write("incq",arg->get_address());
    arg->to_asm(wr);
}
void predec::to_asm(code::writer&wr)const
{
    wr.write("decq",arg->get_address());
    arg->to_asm(wr);
}
void postinc::to_asm(code::writer&wr)const
{
    arg->to_asm(wr);
    wr.write("incq",arg->get_address());
}
void postdec::to_asm(code::writer&wr)const
{
    arg->to_asm(wr);
    wr.write("decq",arg->get_address());
}
void bplus::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("add","%rdi","%rax");
}
void bminus::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("sub","%rdi","%rax");
}
void multiply::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("imul","%rdi");
}
void divide::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("xor","%rdx","%rdx");
    wr.write("idiv","%rdi");
}
void remain::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("xor","%rdx","%rdx");
    wr.write("idiv","%rdi");
    wr.write("mov","%rdx","%rax");
}
void equal::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("cmp","%rdi","%rax");
    wr.write("sete","%al");
    wr.write("movzb","%al","%rax");
}
void nequal::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("cmp","%rdi","%rax");
    wr.write("setne","%al");
    wr.write("movzb","%al","%rax");
}
void less::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("cmp","%rdi","%rax");
    wr.write("setl","%al");
    wr.write("movzb","%al","%rax");
}
void greater::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("cmp","%rdi","%rax");
    wr.write("setg","%al");
    wr.write("movzb","%al","%rax");
}
void leeq::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("cmp","%rdi","%rax");
    wr.write("setle","%al");
    wr.write("movzb","%al","%rax");
}
void greq::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("cmp","%rdi","%rax");
    wr.write("setge","%al");
    wr.write("movzb","%al","%rax");
}
void logand::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("and","%rdi","%rax");
    wr.write("cmp",0,"%rax");
    wr.write("setne","%al");
    wr.write("movzb","%al","%rax");
}
void logor::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("or","%rdi","%rax");
    wr.write("cmp",0,"%rax");
    wr.write("setne","%al");
    wr.write("movzb","%al","%rax");
}
void comma::to_asm(code::writer&wr)const
{
    larg->to_asm(wr);
    rarg->to_asm(wr);
}
void assign::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax",larg->get_address());
}
void plasgn::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("add","%rax",larg->get_address());
    larg->to_asm(wr);
}
void miasgn::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("sub","%rax",larg->get_address());
    larg->to_asm(wr);
}
void muasgn::to_asm(code::writer&wr)const
{
    std::string addr=larg->get_address();
    rarg->to_asm(wr);
    wr.write("imulq",addr);
    wr.write("mov","%rax",addr);
}
void diasgn::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("xor","%rdx","%rdx");
    wr.write("idiv","%rdi");
    wr.write("mov","%rax",larg->get_address());
    larg->to_asm(wr);
}
void rmasgn::to_asm(code::writer&wr)const
{
    rarg->to_asm(wr);
    wr.write("mov","%rax","%rdi");
    larg->to_asm(wr);
    wr.write("xor","%rdx","%rdx");
    wr.write("idiv","%rdi");
    wr.write("mov","%rdx",larg->get_address());
    larg->to_asm(wr);
}
void expression_statement::to_asm(code::writer&wr)const
{
    expr->to_asm(wr);
}
void null_statement::to_asm(code::writer&wr)const
{
    wr.write("nop");
}
void compound::to_asm(code::writer&wr)const
{
    enter_scope();
    for(const auto&s:stats)s->to_asm(wr);
    wr.write("add",leave_scope(),"%rsp");
}
void var_difinition::to_asm(code::writer&wr)const
{
    wr.write("sub",8*vars.size(),"%rsp");
    for(const auto&v:vars){
        v.first->allocate_on_stack();
        if(v.second){
            v.second->to_asm(wr);
            wr.write("mov","%rax",v.first->get_address());
        }
    }
}
void _if_else_::to_asm(code::writer&wr)const
{
    std::string lelse=code::writer::get_unique_label(".Lieelse");
    std::string lend=code::writer::get_unique_label(".Lieend");
    enter_scope();
    cond->to_asm(wr);
    wr.write("cmp",0,"%rax");
    wr.write("je",lelse);
    stat_if->to_asm(wr);
    wr.write("jmp",lend);
    wr.write(lelse+':');
    if(stat_else)stat_else->to_asm(wr);
    wr.write(lend+':');
    wr.write("add",leave_scope(),"%rsp");
}
void _while_::to_asm(code::writer&wr)const
{
    std::string lbegin=code::writer::get_unique_label(".Lwbegin");
    std::string lend=code::writer::get_unique_label(".Lwend");
    enter_scope();
    enter_break(lend);
    enter_continue(lbegin);
    wr.write(lbegin+':');
    cond->to_asm(wr);
    wr.write("cmp",0,"%rax");
    wr.write("je",lend);
    stat->to_asm(wr);
    wr.write("jmp",lbegin);
    wr.write(lend+':');
    leave_continue();
    leave_break();
    wr.write("add",leave_scope(),"%rsp");
}
void _for_::to_asm(code::writer&wr)const
{
    std::string lbegin=code::writer::get_unique_label(".Lfbegin");
    std::string lreini=code::writer::get_unique_label(".Lfreini");
    std::string lend=code::writer::get_unique_label(".Lfend");
    enter_scope();
    enter_break(lend);
    enter_continue(lreini);
    if(init)init->to_asm(wr);
    wr.write(lbegin+':');
    wr.write("mov",1,"%rax");
    if(cond)cond->to_asm(wr);
    wr.write("cmp",0,"%rax");
    wr.write("je",lend);
    stat->to_asm(wr);
    wr.write(lreini+':');
    if(reinit)reinit->to_asm(wr);
    wr.write("jmp",lbegin);
    wr.write(lend+':');
    leave_continue();
    leave_break();
    wr.write("add",leave_scope(),"%rsp");
}
void _break_::to_asm(code::writer&wr)const
{
    if(iteration_statement::break_labels.empty())
        throw exception::compilation_error("不適切なbreak文です",line,col);
    wr.write("jmp",iteration_statement::break_labels.top());
}
void _continue_::to_asm(code::writer&wr)const
{
    if(iteration_statement::continue_labels.empty())
        throw exception::compilation_error("不適切なcontinue文です",line,col);
    wr.write("jmp",iteration_statement::continue_labels.top());
}
void _return_::to_asm(code::writer&wr)const
{
    value->to_asm(wr);
    wr.write("mov","%rbp","%rsp");
    wr.write("pop","%rbp");
    wr.write("ret");
}
void function_difinition::to_asm(code::writer&wr)const
{
    wr.write(".globl "+name);
    wr.write(name+':');
    wr.write("push","%rbp");
    wr.write("mov","%rsp","%rbp");
    wr.write("sub",std::min(6ul,args.size())*8,"%rsp");
    enter_scope();
    for(int i=0;i<std::min(6ul,args.size());++i){
        args[i]->allocate_on_stack();
        wr.write("mov",std::vector{"%rdi","%rsi","%rdx","%rcx","%r8","%r9"}[i],args[i]->get_address());
    }
    for(int i=6;i<args.size();++i)
        args[i]->allocate_on_stack(i*8-32);
    // TODO: com内で必ずreturnすることを前提にしている問題を解決する
    for(const auto&s:stats)s->to_asm(wr);
    leave_scope();
}
void translation_unit::to_asm(code::writer&wr)const
{
    enter_scope();
    for(const auto&f:funcs)f->to_asm(wr);
    leave_scope();
}
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
    if(auto casted=dynamic_cast<const identifier*>(row)){
        arg=std::unique_ptr<const identifier>(casted);
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
    if(auto casted=dynamic_cast<const identifier*>(row)){
        larg=std::unique_ptr<const identifier>(casted);
    }else{
        delete row;
        throw exception::compilation_error("左引数が左辺値ではありません",line,col);
    }
}
expression_statement::expression_statement(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    expr=expression::get(ta);
    if(!ta.consume(lexicon::TK::SCOLON))
        throw exception::compilation_error("セミコロンが見つかりませんでした",ta.get_line(),ta.get_column());
}
null_statement::null_statement(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume(lexicon::TK::SCOLON))
        throw exception::compilation_error("セミコロンが見つかりませんでした",ta.get_line(),ta.get_column());
}
compound::compound(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume(lexicon::TK::OBRACE))
        throw exception::compilation_error("複文の開始ブラケットが見つかりませんでした",ta.get_line(),ta.get_column());
    while(!ta.consume(lexicon::TK::CBRACE))stats.push_back(statement::get(ta));
}
var_difinition::var_difinition(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume(lexicon::TK::INT))
        throw exception::compilation_error("型指定子が見つかりませんでした",ta.get_line(),ta.get_column());
    while(true){
        if(auto idp=std::dynamic_pointer_cast<const lexicon::identifier>(ta.consume(lexicon::TK::IDENT)))
            vars.push_back(
                std::make_pair(
                    std::make_unique<const identifier>(idp->name,idp->line,idp->col),
                    ta.consume(lexicon::TK::EQUAL)?expression::get(ta,true):nullptr
                )
            );
        else
            throw exception::compilation_error("変数名が見つかりませんでした",ta.get_line(),ta.get_column());
        if(ta.consume(lexicon::TK::SCOLON))
            break;
        else if(!ta.consume(lexicon::TK::COMMA))
            throw exception::compilation_error("不正な区切り文字です",ta.get_line(),ta.get_column());
    }
}
_if_else_::_if_else_(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume(lexicon::TK::IF))
        throw exception::compilation_error("ifキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume(lexicon::TK::OPARENT))
        throw exception::compilation_error("ifの後ろに括弧がありません",ta.get_line(),ta.get_column());
    cond=expression::get(ta);
    if(!ta.consume(lexicon::TK::CPARENT))
        throw exception::compilation_error("ifの後ろに括弧がありません",ta.get_line(),ta.get_column());
    stat_if=statement::get(ta);
    stat_else=ta.consume(lexicon::TK::ELSE)?statement::get(ta):nullptr;
}
_while_::_while_(lexicon::token_array&ta)
    :iteration_statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume(lexicon::TK::WHILE))
        throw exception::compilation_error("whileキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume(lexicon::TK::OPARENT))
        throw exception::compilation_error("whileの後ろに括弧がありません",ta.get_line(),ta.get_column());
    cond=expression::get(ta);
    if(!ta.consume(lexicon::TK::CPARENT))
        throw exception::compilation_error("whileの後ろに括弧がありません",ta.get_line(),ta.get_column());
    stat=statement::get(ta);
}
_for_::_for_(lexicon::token_array&ta)
    :iteration_statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume(lexicon::TK::FOR))
        throw exception::compilation_error("forキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume(lexicon::TK::OPARENT))
        throw exception::compilation_error("forの後ろに括弧がありません",ta.get_line(),ta.get_column());
    init=ta.check(lexicon::TK::SCOLON)?nullptr:expression::get(ta);
    if(!ta.consume(lexicon::TK::SCOLON))
        throw exception::compilation_error("セミコロンが見つかりませんでした",ta.get_line(),ta.get_column());
    cond=ta.check(lexicon::TK::SCOLON)?nullptr:expression::get(ta);
    if(!ta.consume(lexicon::TK::SCOLON))
        throw exception::compilation_error("セミコロンが見つかりませんでした",ta.get_line(),ta.get_column());
    reinit=ta.check(lexicon::TK::CPARENT)?nullptr:expression::get(ta);
    if(!ta.consume(lexicon::TK::CPARENT))
        throw exception::compilation_error("forの後ろに括弧がありません",ta.get_line(),ta.get_column());
    stat=statement::get(ta);
}
_break_::_break_(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume(lexicon::TK::BREAK))
        throw exception::compilation_error("breakキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume(lexicon::TK::SCOLON))
        throw exception::compilation_error("不正なbreak文です",ta.get_line(),ta.get_column());
}
_continue_::_continue_(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume(lexicon::TK::CONTINUE))
        throw exception::compilation_error("continueキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume(lexicon::TK::SCOLON))
        throw exception::compilation_error("不正なcontinue文です",ta.get_line(),ta.get_column());
}
_return_::_return_(lexicon::token_array&ta)
    :statement(ta.get_line(),ta.get_column())
{
    if(!ta.consume(lexicon::TK::RETURN))
        throw exception::compilation_error("returnキーワードが見つかりませんでした",ta.get_line(),ta.get_column());
    value=expression::get(ta);
    if(!ta.consume(lexicon::TK::SCOLON))
        throw exception::compilation_error("不正なreturn文です",ta.get_line(),ta.get_column());
}
function_difinition::function_difinition(lexicon::token_array&ta)
    :node(ta.get_line(),ta.get_column())
{
    if(!ta.consume(lexicon::TK::INT))
        throw exception::compilation_error("関数の型が見つかりませんでした",ta.get_line(),ta.get_column());
    if(auto fidp=std::dynamic_pointer_cast<const lexicon::identifier>(ta.consume(lexicon::TK::IDENT)))
        name=fidp->name;
    else
        throw exception::compilation_error("関数名が見つかりませんでした",ta.get_line(),ta.get_column());
    if(!ta.consume(lexicon::TK::OPARENT))
        throw exception::compilation_error("引数リストが見つかりませんでした",ta.get_line(),ta.get_column());
    if(ta.consume(lexicon::TK::VOID)){
        if(!ta.consume(lexicon::TK::CPARENT))
            throw exception::compilation_error("不正な引数リストです",ta.get_line(),ta.get_column());
    }else{
        while(true){
            if(!ta.consume(lexicon::TK::INT))
                throw exception::compilation_error("引数の型が見つかりませんでした",ta.get_line(),ta.get_column());
            if(auto idp=std::dynamic_pointer_cast<const lexicon::identifier>(ta.consume(lexicon::TK::IDENT)))
                args.push_back(std::make_unique<const identifier>(idp->name,idp->line,idp->col));
            else
                throw exception::compilation_error("引数名が見つかりませんでした",ta.get_line(),ta.get_column());
            if(ta.consume(lexicon::TK::COMMA))
                continue;
            else if(ta.consume(lexicon::TK::CPARENT))
                break;
            else
                throw exception::compilation_error("不正な区切り文字です",ta.get_line(),ta.get_column());
        }
    }
    if(!ta.consume(lexicon::TK::OBRACE))
        throw exception::compilation_error("関数の開始ブラケットが見つかりません",ta.get_line(),ta.get_column());
    while(!ta.consume(lexicon::TK::CBRACE))stats.push_back(statement::get(ta));
}
translation_unit::translation_unit(lexicon::token_array&ta)
    :node(ta.get_line(),ta.get_column())
{
    while(!ta.is_all_read())funcs.push_back(std::make_unique<const function_difinition>(ta));
}
