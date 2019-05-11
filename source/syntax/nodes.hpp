#pragma once
#include<string>
#include<vector>
#include"../code/generator.hpp"
namespace syntax{
    class node{
	public:
	    virtual ~node();
	    virtual void to_asm(const code::generator&gen)const=0;
    };
    class numeric:public node{
	    int value;
	public:
	    numeric(int value);
	    void to_asm(const code::generator&gen)const override;
    };
    class ident:public node{
	    std::string name;
	public:
	    ident(const std::string&name);
	    void to_asm(const code::generator&gen)const override;
	    void refer(const code::generator&gen)const;
    };
    class unopr:public node{
	public:
	    const node*const arg;
	    unopr(const node*arg);
	    virtual ~unopr();
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
    class preinc:public unopr{
	public:
	    preinc(const node*arg);
	    void to_asm(const code::generator&gen)const override;
    };
    class predec:public unopr{
	public:
	    predec(const node*arg);
	    void to_asm(const code::generator&gen)const override;
    };
    class biopr:public node{
	public:
	    const node*const larg,*const rarg;
	    biopr(const node*larg,const node*rarg);
	    virtual ~biopr();
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
    class assign:public biopr{
	public:
	    assign(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class plasgn:public biopr{
	public:
	    plasgn(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class miasgn:public biopr{
	public:
	    miasgn(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class muasgn:public biopr{
	public:
	    muasgn(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class diasgn:public biopr{
	public:
	    diasgn(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class rmasgn:public biopr{
	public:
	    rmasgn(const node*larg,const node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
}
