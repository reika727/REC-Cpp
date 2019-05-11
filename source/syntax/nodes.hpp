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
	    unopr(node*arg);
	    virtual ~unopr();
    };
    class uplus:public unopr{
	public:
	    uplus(node*arg);
	    void to_asm(const code::generator&gen)const override;
    };
    class uminus:public unopr{
	public:
	    uminus(node*arg);
	    void to_asm(const code::generator&gen)const override;
    };
    class preinc:public unopr{
	public:
	    preinc(node*arg);
	    void to_asm(const code::generator&gen)const override;
    };
    class predec:public unopr{
	public:
	    predec(node*arg);
	    void to_asm(const code::generator&gen)const override;
    };
    class biopr:public node{
	public:
	    const node*const larg,*const rarg;
	    biopr(node*larg,node*rarg);
	    virtual ~biopr();
    };
    class plus:public biopr{
	public:
	    plus(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class minus:public biopr{
	public:
	    minus(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class multi:public biopr{
	public:
	    multi(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class divide:public biopr{
	public:
	    divide(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class remain:public biopr{
	public:
	    remain(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class equal:public biopr{
	public:
	    equal(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class nequal:public biopr{
	public:
	    nequal(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class less:public biopr{
	public:
	    less(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class greater:public biopr{
	public:
	    greater(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class leeq:public biopr{
	public:
	    leeq(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class greq:public biopr{
	public:
	    greq(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class assign:public biopr{
	public:
	    assign(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class plasgn:public biopr{
	public:
	    plasgn(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class miasgn:public biopr{
	public:
	    miasgn(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class muasgn:public biopr{
	public:
	    muasgn(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class diasgn:public biopr{
	public:
	    diasgn(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
    class rmasgn:public biopr{
	public:
	    rmasgn(node*larg,node*rarg);
	    void to_asm(const code::generator&gen)const override;
    };
}
