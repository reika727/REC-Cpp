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
	    virtual ~node()=0;
	    virtual void check(semantics::analyzer&analy)const=0;
	    virtual void to_asm(code::variable_manager&vm,code::writer&wr)const=0;
    };
    class expression:public node{
	public:
	    virtual ~expression()=0;
    };
    class numeric:public expression{
	public:
	    const int value;
	    numeric(int value);
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class ident:public expression{
	public:
	    const std::string name;
	    ident(const std::string&name);
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
	    void refer(code::variable_manager&vm,code::writer&wr)const;
    };
    class fcall:public expression{
	public:
	    const expression*const func;
	    const std::vector<const expression*>*const vars;
	    fcall(const expression*func,decltype(vars)vars);
	    ~fcall()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
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
	    uplus(const expression*arg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class uminus:public unopr{
	public:
	    uminus(const expression*arg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class unopr_l:public unopr{
	public:
	    unopr_l(const expression*arg);
	    void check(semantics::analyzer&analy)const override;
    };
    class preinc:public unopr_l{
	public:
	    preinc(const expression*arg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class predec:public unopr_l{
	public:
	    predec(const expression*arg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class postinc:public unopr_l{
	public:
	    postinc(const expression*arg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class postdec:public unopr_l{
	public:
	    postdec(const expression*arg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
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
	    plus(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class minus:public biopr{
	public:
	    minus(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class multi:public biopr{
	public:
	    multi(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class divide:public biopr{
	public:
	    divide(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class remain:public biopr{
	public:
	    remain(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class equal:public biopr{
	public:
	    equal(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class nequal:public biopr{
	public:
	    nequal(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class less:public biopr{
	public:
	    less(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class greater:public biopr{
	public:
	    greater(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class leeq:public biopr{
	public:
	    leeq(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class greq:public biopr{
	public:
	    greq(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class comma:public biopr{
	public:
	    comma(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class biopr_l:public biopr{
	public:
	    biopr_l(const expression*larg,const expression*rarg);
	    void check(semantics::analyzer&analy)const override;
    };
    class assign:public biopr_l{
	public:
	    assign(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class plasgn:public biopr_l{
	public:
	    plasgn(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class miasgn:public biopr_l{
	public:
	    miasgn(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class muasgn:public biopr_l{
	public:
	    muasgn(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class diasgn:public biopr_l{
	public:
	    diasgn(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class rmasgn:public biopr_l{
	public:
	    rmasgn(const expression*larg,const expression*rarg);
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class statement:public node{
	public:
	    virtual ~statement()=0;
    };
    class single:public statement{
	public:
	    const expression*const stat;
	    single(const expression*stat);
	    ~single()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class compound:public statement{
	public:
	    const std::vector<const statement*>*const stats;
	    compound(decltype(stats)stats);
	    ~compound()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class define:public statement{
	public:
	    const std::vector<std::pair<std::string,const expression*>>*vars;
	    define(decltype(vars)vars);
	    ~define()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class _if_else_:public statement{
	public:
	    const single*const cond;
	    const statement*const st1,*const st2;
	    _if_else_(const single*cond,const statement*st1,const statement*st2);
	    ~_if_else_()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class _while_:public statement{
	public:
	    const single*const cond;
	    const statement*const st;
	    _while_(const single*cond,const statement*st);
	    ~_while_()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class _for_:public statement{
	public:
	    const single*const init,*const cond,*const reinit;
	    const statement*const st;
	    _for_(const single*init,const single*cond,const single*reinit,const statement*st);
	    ~_for_()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
    class _return_:public statement{
	public:
	    const single*const val;
	    _return_(const single*val);
	    ~_return_();
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };

    class function:public node{
	public:
	    const std::string name;
	    const compound*const com;
	    const std::vector<std::string>*const args;
	    function(std::string name,decltype(args)args,const compound*com);
	    ~function()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::variable_manager&vm,code::writer&wr)const override;
    };
}
