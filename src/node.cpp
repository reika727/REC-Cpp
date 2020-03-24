#include"syntax/node.hpp"
#include<stdexcept>
using namespace syntax;
void numeric::check(semantics::analyzer&analy)const noexcept
{
    return;
}
void ident::check(semantics::analyzer&analy)const
{
    if(!analy.is_available_var(name))throw std::runtime_error("未定義の変数です: "+name);
}
void fcall::check(semantics::analyzer&analy)const
{
    // TODO: 関数ポインタに対応する
    if(auto fp=std::dynamic_pointer_cast<const ident>(func)){
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
    st1->check(analy);
    st2->check(analy);
    analy.leave_scope();
}
void _while_::check(semantics::analyzer&analy)const
{
    analy.enter_scope();
    analy.enter_break();
    analy.enter_continue();
    cond->check(analy);
    st->check(analy);
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
    st->check(analy);
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
    val->check(analy);
}
void function::check(semantics::analyzer&analy)const
{
    if(!analy.is_definable_func(name))throw std::runtime_error("二重定義されました: "+name);
    analy.define_func(name,args.size());
    if(com){
        analy.enter_scope();
        for(auto a:args)analy.define_var(a);
        for(auto s:(com->stats))s->check(analy);
        analy.leave_scope();
    }
}
void numeric::to_asm(code::generator&cg)const
{
    cg.write("push",value);
}
void ident::to_asm(code::generator&cg)const
{
    cg.write("push",code::generator::address(-cg.get_offset(name),"%rbp"));
}
void ident::refer(code::generator&cg)const
{
    cg.write("lea",code::generator::address(-cg.get_offset(name),"%rbp"),"%rax");
    cg.write("push","%rax");
}
void fcall::to_asm(code::generator&cg)const
{
    int align=(16-cg.get_var_size()%16)%16;
    cg.write("sub",align,"%rsp");
    for(int i=vars.size()-1;i>=0;--i){
        vars[i]->to_asm(cg);
        switch(i){
            case 0 :cg.write("pop","%rdi");break;
            case 1 :cg.write("pop","%rsi");break;
            case 2 :cg.write("pop","%rdx");break;
            case 3 :cg.write("pop","%rcx");break;
            case 4 :cg.write("pop","%r8"); break;
            case 5 :cg.write("pop","%r9"); break;
        }
    }
    // TODO: 関数ポインタに対応する
    cg.write("call",std::dynamic_pointer_cast<const ident>(func)->name);
    if(vars.size()>6)cg.write("add",8*(vars.size()-6),"%rsp");
    cg.write("add",align,"%rsp");
    cg.write("push","%rax");
}
void uplus::to_asm(code::generator&cg)const
{
    arg->to_asm(cg);
}
void uminus::to_asm(code::generator&cg)const
{
    arg->to_asm(cg);
    cg.write("pop","%rax");
    cg.write("mov","%rax","%rdi");
    cg.write("mov",2,"%rsi");
    cg.write("mul","%rsi");
    cg.write("sub","%rax","%rdi");
    cg.write("push","%rdi");
}
void lognot::to_asm(code::generator&cg)const
{
    arg->to_asm(cg);
    cg.write("pop","%rax");
    cg.write("cmp",0,"%rax");
    cg.write("sete","%al");
    cg.write("movzb","%al","%rax");
    cg.write("push","%rax");
}
void preinc::to_asm(code::generator&cg)const
{
    arg->refer(cg);
    cg.write("pop","%rax");
    cg.write("add",1,code::generator::address("%rax"));
    cg.write("push",code::generator::address("%rax"));
}
void predec::to_asm(code::generator&cg)const
{
    arg->refer(cg);
    cg.write("pop","%rax");
    cg.write("sub",1,code::generator::address("%rax"));
    cg.write("push",code::generator::address("%rax"));
}
void postinc::to_asm(code::generator&cg)const
{
    arg->refer(cg);
    cg.write("pop","%rax");
    cg.write("push",code::generator::address("%rax"));
    cg.write("add",1,code::generator::address("%rax"));
}
void postdec::to_asm(code::generator&cg)const
{
    arg->refer(cg);
    cg.write("pop","%rax");
    cg.write("push",code::generator::address("%rax"));
    cg.write("sub",1,code::generator::address("%rax"));
}
void plus::to_asm(code::generator&cg)const
{
    larg->to_asm(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("add","%rdi","%rax");
    cg.write("push","%rax");
}
void minus::to_asm(code::generator&cg)const
{
    larg->to_asm(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("sub","%rdi","%rax");
    cg.write("push","%rax");
}
void multi::to_asm(code::generator&cg)const
{
    larg->to_asm(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("mul","%rdi");
    cg.write("push","%rax");
}
void divide::to_asm(code::generator&cg)const
{
    larg->to_asm(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("mov",0,"%rdx");
    cg.write("div","%rdi");
    cg.write("push","%rax");
}
void remain::to_asm(code::generator&cg)const
{
    larg->to_asm(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("mov",0,"%rdx");
    cg.write("div","%rdi");
    cg.write("push","%rdx");
}
void equal::to_asm(code::generator&cg)const
{
    larg->to_asm(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("cmp","%rdi","%rax");
    cg.write("sete","%al");
    cg.write("movzb","%al","%rax");
    cg.write("push","%rax");
}
void nequal::to_asm(code::generator&cg)const
{
    larg->to_asm(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("cmp","%rdi","%rax");
    cg.write("setne","%al");
    cg.write("movzb","%al","%rax");
    cg.write("push","%rax");
}
void less::to_asm(code::generator&cg)const
{
    larg->to_asm(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("cmp","%rdi","%rax");
    cg.write("setl","%al");
    cg.write("movzb","%al","%rax");
    cg.write("push","%rax");
}
void greater::to_asm(code::generator&cg)const
{
    larg->to_asm(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("cmp","%rdi","%rax");
    cg.write("setg","%al");
    cg.write("movzb","%al","%rax");
    cg.write("push","%rax");
}
void leeq::to_asm(code::generator&cg)const
{
    larg->to_asm(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("cmp","%rdi","%rax");
    cg.write("setle","%al");
    cg.write("movzb","%al","%rax");
    cg.write("push","%rax");
}
void greq::to_asm(code::generator&cg)const
{
    larg->to_asm(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("cmp","%rdi","%rax");
    cg.write("setge","%al");
    cg.write("movzb","%al","%rax");
    cg.write("push","%rax");
}
void logand::to_asm(code::generator&cg)const
{
    larg->to_asm(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("and","%rdi","%rax");
    cg.write("cmp",0,"%rax");
    cg.write("setne","%al");
    cg.write("movzb","%al","%rax");
    cg.write("push","%rax");
}
void logor::to_asm(code::generator&cg)const
{
    larg->to_asm(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("or","%rdi","%rax");
    cg.write("cmp",0,"%rax");
    cg.write("setne","%al");
    cg.write("movzb","%al","%rax");
    cg.write("push","%rax");
}
void comma::to_asm(code::generator&cg)const
{
    larg->to_asm(cg);
    cg.write("add",8,"%rsp");
    rarg->to_asm(cg);
}
void assign::to_asm(code::generator&cg)const
{
    larg->refer(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("mov","%rdi",code::generator::address("%rax"));
    cg.write("push",code::generator::address("%rax"));
}
void plasgn::to_asm(code::generator&cg)const
{
    larg->refer(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("add","%rdi",code::generator::address("%rax"));
    cg.write("push",code::generator::address("%rax"));
}
void miasgn::to_asm(code::generator&cg)const
{
    larg->refer(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("sub","%rdi",code::generator::address("%rax"));
    cg.write("push",code::generator::address("%rax"));
}
void muasgn::to_asm(code::generator&cg)const
{
    larg->refer(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("mov","%rax","%rsi");
    cg.write("mov",code::generator::address("%rax"),"%rax");
    cg.write("mul","%rdi");
    cg.write("mov","%rax",code::generator::address("%rsi"));
    cg.write("push","%rax");
}
void diasgn::to_asm(code::generator&cg)const
{
    larg->refer(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("mov","%rax","%rsi");
    cg.write("mov",code::generator::address("%rax"),"%rax");
    cg.write("mov",0,"%rdx");
    cg.write("div","%rdi");
    cg.write("mov","%rax",code::generator::address("%rsi"));
    cg.write("push","%rax");
}
void rmasgn::to_asm(code::generator&cg)const
{
    larg->refer(cg);
    rarg->to_asm(cg);
    cg.write("pop","%rdi");
    cg.write("pop","%rax");
    cg.write("mov","%rax","%rsi");
    cg.write("mov",code::generator::address("%rax"),"%rax");
    cg.write("mov",0,"%rdx");
    cg.write("div","%rdi");
    cg.write("mov","%rdx",code::generator::address("%rsi"));
    cg.write("push","%rdx");
}
void expression_statement::to_asm(code::generator&cg)const
{
    expr->to_asm(cg);
    cg.write("pop","%rax");
}
void null_statement::to_asm(code::generator&cg)const
{
    return;
}
void compound::to_asm(code::generator&cg)const
{
    cg.enter_scope();
    for(auto s:stats)s->to_asm(cg);
    cg.leave_scope();
}
void define_var::to_asm(code::generator&cg)const
{
    for(auto v:vars){
        cg.write("sub",8,"%rsp");
        cg.set_offset(v.first);
        if(v.second){
            v.second->to_asm(cg);
            cg.write("pop",code::generator::address("%rsp"));
        }
    }
}
void _if_else_::to_asm(code::generator&cg)const
{
    std::string lelse=code::generator::unique_label(".Lieelse");
    std::string lend=code::generator::unique_label(".Lieend");
    cg.enter_scope();
    cond->to_asm(cg);
    cg.write("cmp",0,"%rax");
    cg.write("je",lelse);
    st1->to_asm(cg);
    cg.write("jmp",lend);
    cg.write(lelse+':');
    st2->to_asm(cg);
    cg.write(lend+':');
    cg.leave_scope();
}
void _while_::to_asm(code::generator&cg)const
{
    std::string lbegin=code::generator::unique_label(".Lwbegin");
    std::string lend=code::generator::unique_label(".Lwend");
    cg.enter_scope();
    cg.enter_break(lend);
    cg.enter_continue(lbegin);
    cg.write(lbegin+':');
    cond->to_asm(cg);
    cg.write("cmp",0,"%rax");
    cg.write("je",lend);
    st->to_asm(cg);
    cg.write("jmp",lbegin);
    cg.write(lend+':');
    cg.leave_continue();
    cg.leave_break();
    cg.leave_scope();
}
void _for_::to_asm(code::generator&cg)const
{
    std::string lbegin=code::generator::unique_label(".Lfbegin");
    std::string lreini=code::generator::unique_label(".Lfreini");
    std::string lend=code::generator::unique_label(".Lfend");
    cg.enter_scope();
    cg.enter_break(lend);
    cg.enter_continue(lreini);
    init->to_asm(cg);
    cg.write(lbegin+':');
    cg.write("mov",1,"%rax");
    cond->to_asm(cg);
    cg.write("cmp",0,"%rax");
    cg.write("je",lend);
    st->to_asm(cg);
    cg.write(lreini+':');
    reinit->to_asm(cg);
    cg.write("jmp",lbegin);
    cg.write(lend+':');
    cg.leave_continue();
    cg.leave_break();
    cg.leave_scope();
}
void _break_::to_asm(code::generator&cg)const
{
    cg.write("jmp",cg.get_break_label());
}
void _continue_::to_asm(code::generator&cg)const
{
    cg.write("jmp",cg.get_continue_label());
}
void _return_::to_asm(code::generator&cg)const
{
    val->to_asm(cg);
    cg.write("mov","%rbp","%rsp");
    cg.write("pop","%rbp");
    cg.write("ret");
}
void function::to_asm(code::generator&cg)const
{
    if(com){
        cg.write(".globl "+name);
        cg.write(name+':');
        cg.write("push","%rbp");
        cg.write("mov","%rsp","%rbp");
        cg.enter_scope();
        cg.write("sub",8*args.size(),"%rsp");
        for(int i=0;i<args.size();++i){
            std::string dest=code::generator::address(i+1-args.size(),"%rsp");
            cg.set_offset(args[i]);
            switch(i){
                case 0 :cg.write("mov","%rdi",dest);break;
                case 1 :cg.write("mov","%rsi",dest);break;
                case 2 :cg.write("mov","%rdx",dest);break;
                case 3 :cg.write("mov","%rcx",dest);break;
                case 4 :cg.write("mov","%r8" ,dest);break;
                case 5 :cg.write("mov","%r9" ,dest);break;
                default:
                        cg.write("mov",code::generator::address(8*(i-6)+16,"%rbp"),"%rax");
                        cg.write("mov","%rax",dest);
                        break;
            }
        }
        for(auto s:com->stats)s->to_asm(cg);
        cg.leave_scope();
        //TODO: なんとかする
        cg.write("mov","%rbp","%rsp");
        cg.write("pop","%rbp");
        cg.write("ret");
    }
}
numeric::numeric(int value)
    :value(value){}
ident::ident(const std::string&name)
    :name(name){}
fcall::fcall(const std::shared_ptr<const expression>&func,const std::vector<std::shared_ptr<const expression>>&vars)
    :func(func),vars(vars){}
unopr::unopr(const std::shared_ptr<const expression>&arg)
    :arg(arg){}
unopr_l::unopr_l(const std::shared_ptr<const expression>&arg)
    :arg(std::dynamic_pointer_cast<const ident>(arg))
{
    if(!(this->arg)){
        throw std::runtime_error("右辺値を引数にとることはできません");
    }
}
biopr::biopr(const std::shared_ptr<const expression>&larg,const std::shared_ptr<const expression>&rarg)
    :larg(larg),rarg(rarg){}
biopr_l::biopr_l(const std::shared_ptr<const expression>&larg,const std::shared_ptr<const expression>&rarg)
    :larg(std::dynamic_pointer_cast<const ident>(larg)),rarg(rarg)
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
_if_else_::_if_else_(const std::shared_ptr<const expression_statement>&cond,const std::shared_ptr<const statement>&st1,const std::shared_ptr<const statement>&st2)
    :cond(cond),st1(st1),st2(st2){}
_while_::_while_(const std::shared_ptr<const expression_statement>&cond,const std::shared_ptr<const statement>&st)
    :cond(cond),st(st){}
_for_::_for_(const std::shared_ptr<const single_statement>&init,const std::shared_ptr<const single_statement>&cond,const std::shared_ptr<const single_statement>&reinit,const std::shared_ptr<const statement>&st)
    :init(init),cond(cond),reinit(reinit),st(st){}
_return_::_return_(const std::shared_ptr<const expression_statement>&val)
    :val(val){}
function::function(std::string name,const std::vector<std::string>&args,const std::shared_ptr<const compound>&com)
    :name(name),args(args),com(com){}
