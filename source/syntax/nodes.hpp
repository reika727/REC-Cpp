#pragma once
#include<string>
#include<vector>
#include"../code/generator.hpp"
namespace syntax{
    struct node{
	virtual ~node();
	virtual void to_asm(const code::generator&gen)=0;
    };
    struct numeric:public node{
	int value;
	numeric(int value);
	void to_asm(const code::generator&gen)override;
    };
    struct ident:public node{
	std::string name;
	ident(const std::string&name);
	void to_asm(const code::generator&gen)override;
	void refer(const code::generator&gen);
    };
    struct unopr:public node{
	node*arg;
	unopr(node*arg);
	virtual ~unopr();
    };
    struct uplus:public unopr{
	uplus(node*arg);
	void to_asm(const code::generator&gen)override;
    };
    struct uminus:public unopr{
	uminus(node*arg);
	void to_asm(const code::generator&gen)override;
    };
    struct preinc:public unopr{
	preinc(node*arg);
	void to_asm(const code::generator&gen)override;
    };
    struct predec:public unopr{
	predec(node*arg);
	void to_asm(const code::generator&gen)override;
    };
    struct biopr:public node{
	node*larg,*rarg;
	biopr(node*larg,node*rarg);
	virtual ~biopr();
    };
    struct plus:biopr{
	plus(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct minus:biopr{
	minus(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct multi:biopr{
	multi(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct divide:biopr{
	divide(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct remain:biopr{
	remain(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct equal:biopr{
	equal(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct nequal:biopr{
	nequal(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct less:biopr{
	less(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct greater:biopr{
	greater(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct leeq:biopr{
	leeq(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct greq:biopr{
	greq(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct assign:public biopr{
	assign(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct plasgn:public biopr{
	plasgn(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct miasgn:public biopr{
	miasgn(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct muasgn:public biopr{
	muasgn(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct diasgn:public biopr{
	diasgn(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
    struct rmasgn:public biopr{
	rmasgn(node*larg,node*rarg);
	void to_asm(const code::generator&gen)override;
    };
}
