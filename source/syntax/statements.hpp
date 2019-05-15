#pragma once
#include"expressions.hpp"
#include"../semantics/analyzer.hpp"
#include"../code/generator.hpp"
#include<string>
#include<vector>
#include<utility>
namespace syntax{
    class statement{
	public:
	    virtual ~statement();
	    virtual void check(semantics::analyzer&analy)const=0;
	    virtual void to_asm(code::generator&gen)const=0;
    };
    class single:public statement{
	public:
	    const expression*const stat;
	    single(const expression*stat);
	    ~single()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::generator&gen)const override;
    };
    class compound:public statement{
	public:
	    const std::vector<const statement*>*const stats;
	    compound(decltype(stats)stats);
	    ~compound()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::generator&gen)const override;
    };
    class declare:public statement{
	public:
	    const std::vector<std::pair<std::string,const expression*>>*vars;
	    declare(decltype(vars)vars);
	    ~declare()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::generator&gen)const override;
    };
    class _if_else_:public statement{
	public:
	    const single*const cond;
	    const statement*const st1,*const st2;
	    _if_else_(const single*cond,const statement*st1,const statement*st2);
	    ~_if_else_()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::generator&gen)const override;
    };
    class _while_:public statement{
	public:
	    const single*const cond;
	    const statement*const st;
	    _while_(const single*cond,const statement*st);
	    ~_while_()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::generator&gen)const override;
    };
    class _for_:public statement{
	public:
	    const single*const init,*const cond,*const reinit;
	    const statement*const st;
	    _for_(const single*init,const single*cond,const single*reinit,const statement*st);
	    ~_for_()override;
	    void check(semantics::analyzer&analy)const override;
	    void to_asm(code::generator&gen)const override;
    };
}
