#pragma once
#include"../semantics/analyzer.hpp"
#include"../code/generator.hpp"
#include<string>
namespace syntax{
    class node{
	public:
	    virtual ~node();
	    virtual void check(const semantics::analyzer&analy)const=0;
	    virtual void to_asm(const code::generator&gen)const=0;
    };
    class numeric:public node{
	    int value;
	public:
	    numeric(int value);
	    void check(const semantics::analyzer&analy)const override;
	    void to_asm(const code::generator&gen)const override;
    };
    class ident:public node{
	    std::string name;
	public:
	    ident(const std::string&name);
	    void check(const semantics::analyzer&analy)const override;
	    void to_asm(const code::generator&gen)const override;
	    void refer(const code::generator&gen)const;
    };
    class unopr:public node{
	public:
	    const node*const arg;
	    unopr(const node*arg);
	    virtual ~unopr();
	    void check(const semantics::analyzer&analy)const override;
    };
    class uplus:public unopr{
	public:
	    uplus(const node*arg);
	    void to_asm(const code::generator&gen)const override;
    };
    class uminus:public unopr{
	public:
	    uminus(const node*arg);
	    void to_asm(const code::generator&gen)const override;
    };
    class unopr_l:public unopr{
	public:
	    unopr_l(const node*arg);
	    void check(const semantics::analyzer&analy)const override;
    };
    class preinc:public unopr_l{
	public:
	    preinc(const node*arg);
	    void to_asm(const code::generator&gen)const override;
    };
    class predec:public unopr_l{
	public:
	    predec(const node*arg);
	    void to_asm(const code::generator&gen)const override;
    };
    class biopr:public node{
	public:
	    const node*const larg,*const rarg;
	    biopr(const node*larg,const node*rarg);
	    virtual ~biopr();
	    void check(const semantics::analyzer&analy)const override;
    };
    class plus:public biopr{
	public:
	    plus(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class minus:public biopr{
	public:
	    minus(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class multi:public biopr{
	public:
	    multi(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class divide:public biopr{
	public:
	    divide(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class remain:public biopr{
	public:
	    remain(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class equal:public biopr{
	public:
	    equal(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class nequal:public biopr{
	public:
	    nequal(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class less:public biopr{
	public:
	    less(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class greater:public biopr{
	public:
	    greater(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class leeq:public biopr{
	public:
	    leeq(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class greq:public biopr{
	public:
	    greq(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class biopr_l:public biopr{
	public:
	    biopr_l(const node*larg,const node*rarg);
	    void check(const semantics::analyzer&analy)const override;
    };
    class assign:public biopr_l{
	public:
	    assign(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class plasgn:public biopr_l{
	public:
	    plasgn(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class miasgn:public biopr_l{
	public:
	    miasgn(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class muasgn:public biopr_l{
	public:
	    muasgn(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class diasgn:public biopr_l{
	public:
	    diasgn(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class rmasgn:public biopr_l{
	public:
	    rmasgn(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
}
