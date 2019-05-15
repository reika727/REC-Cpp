#pragma once
#include"../semantics/analyzer.hpp"
#include"../code/generator.hpp"
#include<string>
#include<vector>
namespace syntax{
    class expression{
	public:
	    virtual ~expression();
	    virtual void check(semantics::analyzer&analy)const=0;
	    virtual void to_asm(code::generator&gen)const=0;
    };
    class numeric:public expression{
	public:
	    const int value;
	    numeric(int value);
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::generator&gen)const override;
    };
    class ident:public expression{
	public:
	    const std::string name;
	    ident(const std::string&name);
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::generator&gen)const override;
	    void refer(code::generator&gen)const;
    };
    class fcall:public expression{
	public:
	    const expression*const func;
	    const std::vector<const expression*>*const vars;
	    fcall(const expression*func,decltype(vars)vars);
	    ~fcall()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::generator&gen)const override;
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
	    void to_asm(code::generator&gen)const override;
    };
    class uminus:public unopr{
	public:
	    uminus(const expression*arg);
	    void to_asm(code::generator&gen)const override;
    };
    class unopr_l:public unopr{
	public:
	    unopr_l(const expression*arg);
	    void check(semantics::analyzer&analy)const override;
    };
    class preinc:public unopr_l{
	public:
	    preinc(const expression*arg);
	    void to_asm(code::generator&gen)const override;
    };
    class predec:public unopr_l{
	public:
	    predec(const expression*arg);
	    void to_asm(code::generator&gen)const override;
    };
    class postinc:public unopr_l{
	public:
	    postinc(const expression*arg);
	    void to_asm(code::generator&gen)const override;
    };
    class postdec:public unopr_l{
	public:
	    postdec(const expression*arg);
	    void to_asm(code::generator&gen)const override;
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
	    void to_asm(code::generator&gen)const override;
    };
    class minus:public biopr{
	public:
	    minus(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class multi:public biopr{
	public:
	    multi(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class divide:public biopr{
	public:
	    divide(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class remain:public biopr{
	public:
	    remain(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class equal:public biopr{
	public:
	    equal(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class nequal:public biopr{
	public:
	    nequal(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class less:public biopr{
	public:
	    less(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class greater:public biopr{
	public:
	    greater(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class leeq:public biopr{
	public:
	    leeq(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class greq:public biopr{
	public:
	    greq(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class comma:public biopr{
	public:
	    comma(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class biopr_l:public biopr{
	public:
	    biopr_l(const expression*larg,const expression*rarg);
	    void check(semantics::analyzer&analy)const override;
    };
    class assign:public biopr_l{
	public:
	    assign(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class plasgn:public biopr_l{
	public:
	    plasgn(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class miasgn:public biopr_l{
	public:
	    miasgn(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class muasgn:public biopr_l{
	public:
	    muasgn(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class diasgn:public biopr_l{
	public:
	    diasgn(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
    class rmasgn:public biopr_l{
	public:
	    rmasgn(const expression*larg,const expression*rarg);
	    void to_asm(code::generator&gen)const override;
    };
}
