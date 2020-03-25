#include"syntax/node.hpp"
#include<stdexcept>
using namespace syntax;
void numeric::check(semantics::analyzer&analy)const noexcept
{
    return;
}
void identifier::check(semantics::analyzer&analy)const
{
    if(!analy.is_available_var(name))throw std::runtime_error("未定義の変数です: "+name);
}
void fcall::check(semantics::analyzer&analy)const
{
    // TODO: 関数ポインタに対応する
    if(auto fp=std::dynamic_pointer_cast<const identifier>(func)){
        if(!analy.is_available_func(fp->name,vars.size()))throw std::runtime_error("未定義の関数です: "+fp->name);
        for(auto v:vars)v->check(analy);
    }else{
        throw std::runtime_error("無効な関数呼び出しです");
    }
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
        if(!analy.is_definable_var(v.first))throw std::runtime_error("二重定義されました: "+v.first);
        analy.define_var(v.first);
        if(v.second)v.second->check(analy);
    }
}
void _if_else_::check(semantics::analyzer&analy)const
{
    analy.enter_scope();
    cond->check(analy);
    stat_if->check(analy);
    stat_else->check(analy);
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
    init->check(analy);
    cond->check(analy);
    reinit->check(analy);
    stat->check(analy);
    analy.leave_continue();
    analy.leave_break();
    analy.leave_scope();
}
void _break_::check(semantics::analyzer&analy)const
{
    if(!analy.is_breakable())throw std::runtime_error("不適切なbreak文です");
}
void _continue_::check(semantics::analyzer&analy)const
{
    if(!analy.is_continuable())throw std::runtime_error("不適切なcontinue文です");
}
void _return_::check(semantics::analyzer&analy)const
{
    value->check(analy);
}
void define_function::check(semantics::analyzer&analy)const
{
    if(!analy.is_definable_func(name))throw std::runtime_error("二重定義されました: "+name);
    analy.define_func(name,args.size());
    analy.enter_scope();
    for(auto a:args)analy.define_var(a);
    for(auto s:com->stats)s->check(analy);
    analy.leave_scope();
}
void numeric::to_asm(code::generator&gen)const
{
    gen.write("push",value);
}
void identifier::to_asm(code::generator&gen)const
{
    gen.write("push",code::generator::get_address(-gen.get_offset(name),"%rbp"));
}
void identifier::refer(code::generator&gen)const
{
    gen.write("lea",code::generator::get_address(-gen.get_offset(name),"%rbp"),"%rax");
    gen.write("push","%rax");
}
void fcall::to_asm(code::generator&gen)const
{
    int align=(16-gen.get_var_size()%16)%16;
    gen.write("sub",align,"%rsp");
    for(int i=vars.size()-1;i>=0;--i){
        vars[i]->to_asm(gen);
        switch(i){
            case 0 :gen.write("pop","%rdi");break;
            case 1 :gen.write("pop","%rsi");break;
            case 2 :gen.write("pop","%rdx");break;
            case 3 :gen.write("pop","%rcx");break;
            case 4 :gen.write("pop","%r8"); break;
            case 5 :gen.write("pop","%r9"); break;
        }
    }
    // TODO: 関数ポインタに対応する
    gen.write("call",std::dynamic_pointer_cast<const identifier>(func)->name);
    if(vars.size()>6)gen.write("add",8*(vars.size()-6),"%rsp");
    gen.write("add",align,"%rsp");
    gen.write("push","%rax");
}
void uplus::to_asm(code::generator&gen)const
{
    arg->to_asm(gen);
}
void uminus::to_asm(code::generator&gen)const
{
    arg->to_asm(gen);
    gen.write("pop","%rax");
    gen.write("mov","%rax","%rdi");
    gen.write("mov",2,"%rsi");
    gen.write("mul","%rsi");
    gen.write("sub","%rax","%rdi");
    gen.write("push","%rdi");
}
void lognot::to_asm(code::generator&gen)const
{
    arg->to_asm(gen);
    gen.write("pop","%rax");
    gen.write("cmp",0,"%rax");
    gen.write("sete","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void preinc::to_asm(code::generator&gen)const
{
    arg->refer(gen);
    gen.write("pop","%rax");
    gen.write("add",1,code::generator::get_address("%rax"));
    gen.write("push",code::generator::get_address("%rax"));
}
void predec::to_asm(code::generator&gen)const
{
    arg->refer(gen);
    gen.write("pop","%rax");
    gen.write("sub",1,code::generator::get_address("%rax"));
    gen.write("push",code::generator::get_address("%rax"));
}
void postinc::to_asm(code::generator&gen)const
{
    arg->refer(gen);
    gen.write("pop","%rax");
    gen.write("push",code::generator::get_address("%rax"));
    gen.write("add",1,code::generator::get_address("%rax"));
}
void postdec::to_asm(code::generator&gen)const
{
    arg->refer(gen);
    gen.write("pop","%rax");
    gen.write("push",code::generator::get_address("%rax"));
    gen.write("sub",1,code::generator::get_address("%rax"));
}
void bplus::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("add","%rdi","%rax");
    gen.write("push","%rax");
}
void bminus::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("sub","%rdi","%rax");
    gen.write("push","%rax");
}
void multiply::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("mul","%rdi");
    gen.write("push","%rax");
}
void divide::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("mov",0,"%rdx");
    gen.write("div","%rdi");
    gen.write("push","%rax");
}
void remain::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("mov",0,"%rdx");
    gen.write("div","%rdi");
    gen.write("push","%rdx");
}
void equal::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("cmp","%rdi","%rax");
    gen.write("sete","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void nequal::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("cmp","%rdi","%rax");
    gen.write("setne","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void less::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("cmp","%rdi","%rax");
    gen.write("setl","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void greater::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("cmp","%rdi","%rax");
    gen.write("setg","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void leeq::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("cmp","%rdi","%rax");
    gen.write("setle","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void greq::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("cmp","%rdi","%rax");
    gen.write("setge","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void logand::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("and","%rdi","%rax");
    gen.write("cmp",0,"%rax");
    gen.write("setne","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void logor::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("or","%rdi","%rax");
    gen.write("cmp",0,"%rax");
    gen.write("setne","%al");
    gen.write("movzb","%al","%rax");
    gen.write("push","%rax");
}
void comma::to_asm(code::generator&gen)const
{
    larg->to_asm(gen);
    gen.write("add",8,"%rsp");
    rarg->to_asm(gen);
}
void assign::to_asm(code::generator&gen)const
{
    larg->refer(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("mov","%rdi",code::generator::get_address("%rax"));
    gen.write("push",code::generator::get_address("%rax"));
}
void plasgn::to_asm(code::generator&gen)const
{
    larg->refer(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("add","%rdi",code::generator::get_address("%rax"));
    gen.write("push",code::generator::get_address("%rax"));
}
void miasgn::to_asm(code::generator&gen)const
{
    larg->refer(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("sub","%rdi",code::generator::get_address("%rax"));
    gen.write("push",code::generator::get_address("%rax"));
}
void muasgn::to_asm(code::generator&gen)const
{
    larg->refer(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("mov","%rax","%rsi");
    gen.write("mov",code::generator::get_address("%rax"),"%rax");
    gen.write("mul","%rdi");
    gen.write("mov","%rax",code::generator::get_address("%rsi"));
    gen.write("push","%rax");
}
void diasgn::to_asm(code::generator&gen)const
{
    larg->refer(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("mov","%rax","%rsi");
    gen.write("mov",code::generator::get_address("%rax"),"%rax");
    gen.write("mov",0,"%rdx");
    gen.write("div","%rdi");
    gen.write("mov","%rax",code::generator::get_address("%rsi"));
    gen.write("push","%rax");
}
void rmasgn::to_asm(code::generator&gen)const
{
    larg->refer(gen);
    rarg->to_asm(gen);
    gen.write("pop","%rdi");
    gen.write("pop","%rax");
    gen.write("mov","%rax","%rsi");
    gen.write("mov",code::generator::get_address("%rax"),"%rax");
    gen.write("mov",0,"%rdx");
    gen.write("div","%rdi");
    gen.write("mov","%rdx",code::generator::get_address("%rsi"));
    gen.write("push","%rdx");
}
void expression_statement::to_asm(code::generator&gen)const
{
    expr->to_asm(gen);
    gen.write("pop","%rax");
}
void null_statement::to_asm(code::generator&gen)const
{
    return;
}
void compound::to_asm(code::generator&gen)const
{
    gen.enter_scope();
    for(auto s:stats)s->to_asm(gen);
    gen.leave_scope();
}
void define_var::to_asm(code::generator&gen)const
{
    for(auto v:vars){
        gen.write("sub",8,"%rsp");
        gen.set_offset(v.first);
        if(v.second){
            v.second->to_asm(gen);
            gen.write("pop",code::generator::get_address("%rsp"));
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
    stat_else->to_asm(gen);
    gen.write(lend+':');
    gen.leave_scope();
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
    gen.leave_scope();
}
void _for_::to_asm(code::generator&gen)const
{
    std::string lbegin=code::generator::get_unique_label(".Lfbegin");
    std::string lreini=code::generator::get_unique_label(".Lfreini");
    std::string lend=code::generator::get_unique_label(".Lfend");
    gen.enter_scope();
    gen.enter_break(lend);
    gen.enter_continue(lreini);
    init->to_asm(gen);
    gen.write(lbegin+':');
    gen.write("mov",1,"%rax");
    cond->to_asm(gen);
    gen.write("cmp",0,"%rax");
    gen.write("je",lend);
    stat->to_asm(gen);
    gen.write(lreini+':');
    reinit->to_asm(gen);
    gen.write("jmp",lbegin);
    gen.write(lend+':');
    gen.leave_continue();
    gen.leave_break();
    gen.leave_scope();
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
    gen.write(".globl "+name);
    gen.write(name+':');
    gen.write("push","%rbp");
    gen.write("mov","%rsp","%rbp");
    gen.enter_scope();
    gen.write("sub",8*args.size(),"%rsp");
    for(int i=0;i<args.size();++i){
        std::string dst=code::generator::get_address(i+1-args.size(),"%rsp");
        gen.set_offset(args[i]);
        switch(i){
            case 0 :gen.write("mov","%rdi",dst);break;
            case 1 :gen.write("mov","%rsi",dst);break;
            case 2 :gen.write("mov","%rdx",dst);break;
            case 3 :gen.write("mov","%rcx",dst);break;
            case 4 :gen.write("mov","%r8" ,dst);break;
            case 5 :gen.write("mov","%r9" ,dst);break;
            default:
                    gen.write("mov",code::generator::get_address(8*(i-6)+16,"%rbp"),"%rax");
                    gen.write("mov","%rax",dst);
                    break;
        }
    }
    for(auto s:com->stats)s->to_asm(gen);
    gen.leave_scope();
    //TODO: なんとかする
    gen.write("mov","%rbp","%rsp");
    gen.write("pop","%rbp");
    gen.write("ret");
}
numeric::numeric(int value)
    :value(value){}
identifier::identifier(const std::string&name)
    :name(name){}
fcall::fcall(const std::shared_ptr<const expression>&func,const std::vector<std::shared_ptr<const expression>>&vars)
    :func(func),vars(vars){}
unopr::unopr(const std::shared_ptr<const expression>&arg)
    :arg(arg){}
unopr_l::unopr_l(const std::shared_ptr<const expression>&arg)
    :arg(std::dynamic_pointer_cast<const identifier>(arg))
{
    if(!(this->arg)){
        throw std::runtime_error("右辺値を引数にとることはできません");
    }
}
biopr::biopr(const std::shared_ptr<const expression>&larg,const std::shared_ptr<const expression>&rarg)
    :larg(larg),rarg(rarg){}
biopr_l::biopr_l(const std::shared_ptr<const expression>&larg,const std::shared_ptr<const expression>&rarg)
    :larg(std::dynamic_pointer_cast<const identifier>(larg)),rarg(rarg)
{
    if(!(this->larg)){
        throw std::runtime_error("右辺値を引数にとることはできません");
    }
}
expression_statement::expression_statement(const std::shared_ptr<const expression>&expr)
    :expr(expr){}
compound::compound(const std::vector<std::shared_ptr<const statement>>&stats)
    :stats(stats){}
define_var::define_var(const std::vector<std::pair<std::string,std::shared_ptr<const expression>>>&vars)
    :vars(vars){}
_if_else_::_if_else_(const std::shared_ptr<const expression_statement>&cond,const std::shared_ptr<const statement>&stat_if,const std::shared_ptr<const statement>&stat_else)
    :cond(cond),stat_if(stat_if),stat_else(stat_else){}
_while_::_while_(const std::shared_ptr<const expression_statement>&cond,const std::shared_ptr<const statement>&stat)
    :cond(cond),stat(stat){}
_for_::_for_(const std::shared_ptr<const single_statement>&init,const std::shared_ptr<const single_statement>&cond,const std::shared_ptr<const single_statement>&reinit,const std::shared_ptr<const statement>&stat)
    :init(init),cond(cond),reinit(reinit),stat(stat){}
_return_::_return_(const std::shared_ptr<const expression_statement>&value)
    :value(value){}
define_function::define_function(std::string name,const std::vector<std::string>&args,const std::shared_ptr<const compound>&com)
    :name(name),args(args),com(com){}
