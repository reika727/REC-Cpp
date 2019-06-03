#pragma once
#include"../semantics/analyzer.hpp"
#include"../code/variable_manager.hpp"
#include"../code/writer.hpp"
#include<string>
#include<vector>
#include<utility>
namespace syntax{
    class node{
	public:
	    virtual ~node()=default;
	    virtual void check(semantics::analyzer&analy)const=0;
	    virtual void to_asm(code::variable_manager&vm)const=0;
    };
    class expression:public node{
	public:
	    virtual ~expression()=default;
    };
    class numeric:public expression{
	public:
	    const int value;
	    numeric(int value);
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class ident:public expression{
	public:
	    const std::string name;
	    ident(const std::string&name);
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm)const override;
	    void refer(code::variable_manager&vm)const;
    };
    class fcall:public expression{
	public:
	    const expression*const func;
	    const std::vector<const expression*>*const vars;
	    fcall(const expression*func,decltype(vars)vars);
	    ~fcall()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class unopr:public expression{
	public:
	    const expression*const arg;
	    unopr(const expression*arg);
	    virtual ~unopr()override;
	    void check(semantics::analyzer&analy)const override;
    };
    class uplus:public unopr{
	public:
	    using unopr::unopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class uminus:public unopr{
	public:
	    using unopr::unopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class lognot:public unopr{
	public:
	    using unopr::unopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class unopr_l:public unopr{
	public:
	    using unopr::unopr;
	    void check(semantics::analyzer&analy)const override;
    };
    class preinc:public unopr_l{
	public:
	    using unopr_l::unopr_l;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class predec:public unopr_l{
	public:
	    using unopr_l::unopr_l;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class postinc:public unopr_l{
	public:
	    using unopr_l::unopr_l;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class postdec:public unopr_l{
	public:
	    using unopr_l::unopr_l;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class biopr:public expression{
	public:
	    const expression*const larg,*const rarg;
	    biopr(const expression*larg,const expression*rarg);
	    virtual ~biopr()override;
	    void check(semantics::analyzer&analy)const override;
    };
    class plus:public biopr{
	public:
	    using biopr::biopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class minus:public biopr{
	public:
	    using biopr::biopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class multi:public biopr{
	public:
	    using biopr::biopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class divide:public biopr{
	public:
	    using biopr::biopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class remain:public biopr{
	public:
	    using biopr::biopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class equal:public biopr{
	public:
	    using biopr::biopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class nequal:public biopr{
	public:
	    using biopr::biopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class less:public biopr{
	public:
	    using biopr::biopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class greater:public biopr{
	public:
	    using biopr::biopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class leeq:public biopr{
	public:
	    using biopr::biopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class greq:public biopr{
	public:
	    using biopr::biopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class logand:public biopr{
	public:
	    using biopr::biopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class logor:public biopr{
	public:
	    using biopr::biopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class comma:public biopr{
	public:
	    using biopr::biopr;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class biopr_l:public biopr{
	public:
	    using biopr::biopr;
	    void check(semantics::analyzer&analy)const override;
    };
    class assign:public biopr_l{
	public:
	    using biopr_l::biopr_l;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class plasgn:public biopr_l{
	public:
	    using biopr_l::biopr_l;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class miasgn:public biopr_l{
	public:
	    using biopr_l::biopr_l;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class muasgn:public biopr_l{
	public:
	    using biopr_l::biopr_l;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class diasgn:public biopr_l{
	public:
	    using biopr_l::biopr_l;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class rmasgn:public biopr_l{
	public:
	    using biopr_l::biopr_l;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class statement:public node{
	public:
	    virtual ~statement()=default;
    };
    class single:public statement{
	public:
	    const expression*const stat;
	    single(const expression*stat);
	    ~single()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class compound:public statement{
	public:
	    const std::vector<const statement*>*const stats;
	    compound(decltype(stats)stats);
	    ~compound()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class define_var:public statement{
	public:
	    const std::vector<std::pair<std::string,const expression*>>*vars;
	    define_var(decltype(vars)vars);
	    ~define_var()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class _if_else_:public statement{
	public:
	    const single*const cond;
	    const statement*const st1,*const st2;
	    _if_else_(const single*cond,const statement*st1,const statement*st2);
	    ~_if_else_()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class _while_:public statement{
	public:
	    const single*const cond;
	    const statement*const st;
	    _while_(const single*cond,const statement*st);
	    ~_while_()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class _for_:public statement{
	public:
	    const single*const init,*const cond,*const reinit;
	    const statement*const st;
	    _for_(const single*init,const single*cond,const single*reinit,const statement*st);
	    ~_for_()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class _break_:public statement{
	public:
	    _break_()=default;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class _continue_:public statement{
	public:
	    _continue_()=default;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class _return_:public statement{
	public:
	    const single*const val;
	    _return_(const single*val);
	    ~_return_();
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class function:public node{
	public:
	    const std::string name;
	    const std::vector<std::string>*const args;
	    const compound*const com;
	    function(std::string name,decltype(args)args,const compound*com);
	    ~function()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm)const override;
    };
    class prog:public node{
	public:
	    const std::vector<const function*>*const funcs;
	    prog(decltype(funcs)funcs);
	    ~prog();
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm)const override;
    };
}
