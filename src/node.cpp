#include"syntax/node.hpp"
#include"code/cgfuncs.hpp"
#include<stdexcept>
#include<typeinfo>
using namespace syntax;
using code::address;
using code::unique_label;
void numeric::to_asm(code::cgmanager&cm)const
{
    cm.write("push",value);
}
void ident::to_asm(code::cgmanager&cm)const
{
    cm.write("push",address(-cm.get_offset(name),"%rbp"));
}
void ident::refer(code::cgmanager&cm)const
{
    cm.write("lea",address(-cm.get_offset(name),"%rbp"),"%rax");
    cm.write("push","%rax");
}
void fcall::to_asm(code::cgmanager&cm)const
{
    int align=(16-cm.get_var_size()%16)%16;
    cm.write("sub",align,"%rsp");
    for(int i=vars->size()-1;i>=0;--i){
        (*vars)[i]->to_asm(cm);
        switch(i){
            case 0 :cm.write("pop","%rdi");break;
            case 1 :cm.write("pop","%rsi");break;
            case 2 :cm.write("pop","%rdx");break;
            case 3 :cm.write("pop","%rcx");break;
            case 4 :cm.write("pop","%r8"); break;
            case 5 :cm.write("pop","%r9"); break;
        }
    }
    cm.write("call",dynamic_cast<const ident*>(func)->name);
    if(vars->size()>6){
        cm.write("add",8*(vars->size()-6),"%rsp");
    }
    cm.write("add",align,"%rsp");
    cm.write("push","%rax");
}
void uplus::to_asm(code::cgmanager&cm)const
{
    arg->to_asm(cm);
}
void uminus::to_asm(code::cgmanager&cm)const
{
    arg->to_asm(cm);
    cm.write("pop","%rax");
    cm.write("mov","%rax","%rdi");
    cm.write("mov",2,"%rsi");cm.write("mul","%rsi");
    cm.write("sub","%rax","%rdi");
    cm.write("push","%rdi");
}
void lognot::to_asm(code::cgmanager&cm)const
{
    arg->to_asm(cm);
    cm.write("pop","%rax");
    cm.write("cmp",0,"%rax");
    cm.write("sete","%al");
    cm.write("movzb","%al","%rax");
    cm.write("push","%rax");
}
void preinc::to_asm(code::cgmanager&cm)const
{
    dynamic_cast<const ident*>(arg)->refer(cm);
    cm.write("pop","%rax");
    cm.write("add",1,address("%rax"));
    cm.write("push",address("%rax"));
}
void predec::to_asm(code::cgmanager&cm)const
{
    dynamic_cast<const ident*>(arg)->refer(cm);
    cm.write("pop","%rax");
    cm.write("sub",1,address("%rax"));
    cm.write("push",address("%rax"));
}
void postinc::to_asm(code::cgmanager&cm)const
{
    dynamic_cast<const ident*>(arg)->refer(cm);
    cm.write("pop","%rax");
    cm.write("push",address("%rax"));
    cm.write("add",1,address("%rax"));
}
void postdec::to_asm(code::cgmanager&cm)const
{
    dynamic_cast<const ident*>(arg)->refer(cm);
    cm.write("pop","%rax");
    cm.write("push",address("%rax"));
    cm.write("sub",1,address("%rax"));
}
void plus::to_asm(code::cgmanager&cm)const
{
    larg->to_asm(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("add","%rdi","%rax");
    cm.write("push","%rax");
}
void minus::to_asm(code::cgmanager&cm)const
{
    larg->to_asm(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("sub","%rdi","%rax");
    cm.write("push","%rax");
}
void multi::to_asm(code::cgmanager&cm)const
{
    larg->to_asm(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("mul","%rdi");
    cm.write("push","%rax");
}
void divide::to_asm(code::cgmanager&cm)const
{
    larg->to_asm(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("mov",0,"%rdx");
    cm.write("div","%rdi");
    cm.write("push","%rax");
}
void remain::to_asm(code::cgmanager&cm)const
{
    larg->to_asm(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("mov",0,"%rdx");
    cm.write("div","%rdi");
    cm.write("push","%rdx");
}
void equal::to_asm(code::cgmanager&cm)const
{
    larg->to_asm(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("cmp","%rdi","%rax");
    cm.write("sete","%al");
    cm.write("movzb","%al","%rax");
    cm.write("push","%rax");
}
void nequal::to_asm(code::cgmanager&cm)const
{
    larg->to_asm(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("cmp","%rdi","%rax");
    cm.write("setne","%al");
    cm.write("movzb","%al","%rax");
    cm.write("push","%rax");
}
void less::to_asm(code::cgmanager&cm)const
{
    larg->to_asm(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("cmp","%rdi","%rax");
    cm.write("setl","%al");
    cm.write("movzb","%al","%rax");
    cm.write("push","%rax");
}
void greater::to_asm(code::cgmanager&cm)const
{
    larg->to_asm(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("cmp","%rdi","%rax");
    cm.write("setg","%al");
    cm.write("movzb","%al","%rax");
    cm.write("push","%rax");
}
void leeq::to_asm(code::cgmanager&cm)const
{
    larg->to_asm(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("cmp","%rdi","%rax");
    cm.write("setle","%al");
    cm.write("movzb","%al","%rax");
    cm.write("push","%rax");
}
void greq::to_asm(code::cgmanager&cm)const
{
    larg->to_asm(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("cmp","%rdi","%rax");
    cm.write("setge","%al");
    cm.write("movzb","%al","%rax");
    cm.write("push","%rax");
}
void logand::to_asm(code::cgmanager&cm)const
{
    larg->to_asm(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("and","%rdi","%rax");
    cm.write("cmp",0,"%rax");
    cm.write("setne","%al");
    cm.write("movzb","%al","%rax");
    cm.write("push","%rax");
}
void logor::to_asm(code::cgmanager&cm)const
{
    larg->to_asm(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("or","%rdi","%rax");
    cm.write("cmp",0,"%rax");
    cm.write("setne","%al");
    cm.write("movzb","%al","%rax");
    cm.write("push","%rax");
}
void comma::to_asm(code::cgmanager&cm)const
{
    larg->to_asm(cm);
    cm.write("add",8,"%rsp");
    rarg->to_asm(cm);
}
void assign::to_asm(code::cgmanager&cm)const
{
    dynamic_cast<const ident*>(larg)->refer(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("mov","%rdi",address("%rax"));
    cm.write("push",address("%rax"));
}
void plasgn::to_asm(code::cgmanager&cm)const
{
    dynamic_cast<const ident*>(larg)->refer(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("add","%rdi",address("%rax"));
    cm.write("push",address("%rax"));
}
void miasgn::to_asm(code::cgmanager&cm)const
{
    dynamic_cast<const ident*>(larg)->refer(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("sub","%rdi",address("%rax"));
    cm.write("push",address("%rax"));
}
void muasgn::to_asm(code::cgmanager&cm)const
{
    dynamic_cast<const ident*>(larg)->refer(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("mov","%rax","%rsi");
    cm.write("mov",address("%rax"),"%rax");
    cm.write("mul","%rdi");
    cm.write("mov","%rax",address("%rsi"));
    cm.write("push","%rax");
}
void diasgn::to_asm(code::cgmanager&cm)const
{
    dynamic_cast<const ident*>(larg)->refer(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("mov","%rax","%rsi");
    cm.write("mov",address("%rax"),"%rax");
    cm.write("mov",0,"%rdx");
    cm.write("div","%rdi");
    cm.write("mov","%rax",address("%rsi"));
    cm.write("push","%rax");
}
void rmasgn::to_asm(code::cgmanager&cm)const
{
    dynamic_cast<const ident*>(larg)->refer(cm);
    rarg->to_asm(cm);
    cm.write("pop","%rdi");
    cm.write("pop","%rax");
    cm.write("mov","%rax","%rsi");
    cm.write("mov",address("%rax"),"%rax");
    cm.write("mov",0,"%rdx");
    cm.write("div","%rdi");
    cm.write("mov","%rdx",address("%rsi"));
    cm.write("push","%rdx");
}
void single::to_asm(code::cgmanager&cm)const
{
    if(stat){
        stat->to_asm(cm);
        cm.write("pop","%rax");
    }
}
void compound::to_asm(code::cgmanager&cm)const
{
    cm.enter_scope();
    for(auto s:*stats)s->to_asm(cm);
    cm.leave_scope();
}
void define_var::to_asm(code::cgmanager&cm)const
{
    for(auto v:*vars){
        cm.write("sub",8,"%rsp");
        cm.set_offset(v.first);
        if(v.second){
            v.second->to_asm(cm);
            cm.write("pop",address("%rsp"));
        }
    }
}
void _if_else_::to_asm(code::cgmanager&cm)const
{
    static unsigned int serial=0;
    std::string lelse=unique_label(".Lieelse",serial),lend=unique_label(".Lieend",serial++);
    cm.enter_scope();
    cond->to_asm(cm);
    cm.write("cmp",0,"%rax");
    cm.write("je",lelse);
    st1->to_asm(cm);
    cm.write("jmp",lend);
    cm.write(lelse+':');
    st2->to_asm(cm);
    cm.write(lend+':');
    cm.leave_scope();
}
void _while_::to_asm(code::cgmanager&cm)const
{
    static unsigned int serial=0;
    std::string lbegin=unique_label(".Lwbegin",serial++),lend=unique_label(".Lwend",serial);
    cm.enter_scope();
    cm.enter_break(lend);
    cm.enter_continue(lbegin);
    cm.write(lbegin+':');
    cond->to_asm(cm);
    cm.write("cmp",0,"%rax");
    cm.write("je",lend);
    st->to_asm(cm);
    cm.write("jmp",lbegin);
    cm.write(lend+':');
    cm.leave_continue();
    cm.leave_break();
    cm.leave_scope();
}
void _for_::to_asm(code::cgmanager&cm)const
{
    static unsigned int serial=0;
    std::string lbegin=unique_label(".Lfbegin",serial++),lreini=unique_label(".Lfreini",serial),lend=unique_label(".Lfend",serial);
    cm.enter_scope();
    cm.enter_break(lend);
    cm.enter_continue(lreini);
    init->to_asm(cm);
    cm.write(lbegin+':');
    cm.write("mov",1,"%rax");
    cond->to_asm(cm);
    cm.write("cmp",0,"%rax");
    cm.write("je",lend);
    st->to_asm(cm);
    cm.write(lreini+':');
    reinit->to_asm(cm);
    cm.write("jmp",lbegin);
    cm.write(lend+':');
    cm.leave_continue();
    cm.leave_break();
    cm.leave_scope();
}
void _break_::to_asm(code::cgmanager&cm)const
{
    cm.write("jmp",cm.get_break_label());
}
void _continue_::to_asm(code::cgmanager&cm)const
{
    cm.write("jmp",cm.get_continue_label());
}
void _return_::to_asm(code::cgmanager&cm)const
{
    val->to_asm(cm);
    cm.write("mov","%rbp","%rsp");
    cm.write("pop","%rbp");
    cm.write("ret");
}
void function::to_asm(code::cgmanager&cm)const
{
    if(com){
        cm.write(".globl "+name);
        cm.write(name+':');
        cm.write("push","%rbp");
        cm.write("mov","%rsp","%rbp");
        cm.enter_scope();
        cm.write("sub",8*args->size(),"%rsp");
        for(int i=0;i<args->size();++i){
            std::string dest=address(i+1-args->size(),"%rsp");
            cm.set_offset((*args)[i]);
            switch(i){
                case 0 :cm.write("mov","%rdi",dest);break;
                case 1 :cm.write("mov","%rsi",dest);break;
                case 2 :cm.write("mov","%rdx",dest);break;
                case 3 :cm.write("mov","%rcx",dest);break;
                case 4 :cm.write("mov","%r8" ,dest);break;
                case 5 :cm.write("mov","%r9" ,dest);break;
                default:
                        cm.write("mov",address(8*(i-6)+16,"%rbp"),"%rax");
                        cm.write("mov","%rax",dest);
                        break;
            }
        }
        for(auto s:*(com->stats))s->to_asm(cm);
        cm.leave_scope();
        //強制return
        cm.write("mov","%rbp","%rsp");
        cm.write("pop","%rbp");
        cm.write("ret");
    }
}
void prog::to_asm(code::cgmanager&cm)const
{
    for(auto f:*funcs)f->to_asm(cm);
}
void numeric::check(semantics::analyzer&analy)const
{
    return;
}
void ident::check(semantics::analyzer&analy)const
{
    if(!analy.is_available_var(name))throw std::runtime_error("未定義の変数です: "+name);
}
void fcall::check(semantics::analyzer&analy)const
{
    if(auto fp=dynamic_cast<const ident*>(func)){
        if(!analy.is_available_func(fp->name,vars->size()))throw std::runtime_error("未定義の関数です: "+fp->name);
        for(auto v:*vars)v->check(analy);
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
    if(typeid(*arg)!=typeid(ident))throw std::runtime_error("右辺値への操作です");
    unopr::check(analy);
}
void biopr::check(semantics::analyzer&analy)const
{
    larg->check(analy);
    rarg->check(analy);
}
void biopr_l::check(semantics::analyzer&analy)const
{
    if(typeid(*larg)!=typeid(ident))throw std::runtime_error("右辺値への代入です");
    biopr::check(analy);
}
void single::check(semantics::analyzer&analy)const
{
    if(stat)stat->check(analy);
}
void compound::check(semantics::analyzer&analy)const
{
    analy.enter_scope();
    for(auto s:*stats)s->check(analy);
    analy.leave_scope();
}
void define_var::check(semantics::analyzer&analy)const
{
    for(auto v:*vars){
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
    if(!analy.is_breakable()){
        throw std::runtime_error("不適切なbreak文です");
    }
}
void _continue_::check(semantics::analyzer&analy)const
{
    if(!analy.is_continuable()){
        throw std::runtime_error("不適切なcontinue文です");
    }
}
void _return_::check(semantics::analyzer&analy)const
{
    val->check(analy);
}
void function::check(semantics::analyzer&analy)const
{
    if(!analy.is_definable_func(name))throw std::runtime_error("二重定義されました: "+name);
    analy.define_func(name,args->size());
    if(com){
        analy.enter_scope();
        for(auto a:*args)analy.define_var(a);
        for(auto s:*(com->stats))s->check(analy);
        analy.leave_scope();
    }
}
void prog::check(semantics::analyzer&analy)const
{
    for(auto f:*funcs)f->check(analy);
}
numeric::numeric(int value):value(value)
{

}
ident::ident(const std::string&name):name(name)
{

}
fcall::fcall(const expression*func,const std::vector<const expression*>*vars):func(func),vars(vars)
{

}
unopr::unopr(const expression*arg):arg(arg)
{

}
biopr::biopr(const expression*larg,const expression*rarg):larg(larg),rarg(rarg)
{

}
single::single(const expression*stat):stat(stat)
{

}
compound::compound(const std::vector<const statement*>*stats):stats(stats)
{

}
define_var::define_var(const std::vector<std::pair<std::string,const expression*>>*vars):vars(vars)
{

}
_if_else_::_if_else_(const single*cond,const statement*st1,const statement*st2):cond(cond),st1(st1),st2(st2)
{

}
_while_::_while_(const single*cond,const statement*st):cond(cond),st(st)
{

}
_for_::_for_(const single*init,const single*cond,const single*reinit,const statement*st):init(init),cond(cond),reinit(reinit),st(st)
{

}
_return_::_return_(const single*val):val(val)
{

}
function::function(std::string name,const std::vector<std::string>*args,const compound*com):name(name),args(args),com(com)
{

}
prog::prog(const std::vector<const function*>*funcs):funcs(funcs)
{

}
fcall::~fcall()
{
    delete func;
    for(auto v:*vars)delete v;
    delete vars;
}
unopr::~unopr()
{
    delete arg;
}
biopr::~biopr()
{
    delete larg;
    delete rarg;
}
single::~single()
{
    delete stat;
}
compound::~compound()
{
    for(auto s:*stats)delete s;
    delete stats;
}
define_var::~define_var()
{
    for(auto v:*vars)delete v.second;
    delete vars;
}
_if_else_::~_if_else_()
{
    delete cond;
    delete st1;
    delete st2;
}
_while_::~_while_()
{
    delete cond;
    delete st;
}
_for_::~_for_()
{
    delete init;
    delete cond;
    delete reinit;
    delete st;
}
_return_::~_return_()
{
    delete val;
}
function::~function()
{
    delete args;
    delete com;
}
prog::~prog()
{
    for(auto f:*funcs)delete f;
    delete funcs;
}
