#pragma once
#include"nodes.hpp"
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
	    virtual void eval(code::generator&gen)const=0;
    };
    class single:public statement{
	public:
	    const node*const stat;
	    single(const node*stat);
	    ~single()override;
	    void check(semantics::analyzer&analy)const override;
	    void eval(code::generator&gen)const override;
    };
    class compound:public statement{
	    std::vector<const statement*>stats;
	public:
	    ~compound()override;
	    void check(semantics::analyzer&analy)const override;
	    void eval(code::generator&gen)const override;
	    void push_back_stat(const statement*st);
    };
    class declare:public statement{
	    std::vector<std::pair<std::string,const node*>>vars;
	public:
	    ~declare()override;
	    void check(semantics::analyzer&analy)const override;
	    void eval(code::generator&gen)const override;
	    void push_back_var(std::pair<std::string,const node*>var);
    };
    class _if_else_:public statement{
	public:
	    const single*const cond;
	    const statement*const st1,*const st2;
	    _if_else_(const single*cond,const statement*st1,const statement*st2);
	    ~_if_else_()override;
	    void check(semantics::analyzer&analy)const override;
	    void eval(code::generator&gen)const override;
    };
    class _while_:public statement{
	public:
	    const single*const cond;
	    const statement*const st;
	    _while_(const single*cond,const statement*st);
	    ~_while_()override;
	    void check(semantics::analyzer&analy)const override;
	    void eval(code::generator&gen)const override;
    };
    class _for_:public statement{
	public:
	    const single*const init,*const cond,*const reinit;
	    const statement*const st;
	    _for_(const single*init,const single*cond,const single*reinit,const statement*st);
	    ~_for_()override;
	    void check(semantics::analyzer&analy)const override;
	    void eval(code::generator&gen)const override;
    };
}
