#pragma once
#include<utility>
#include"nodes.hpp"
#include"../code/generator.hpp"
namespace syntax{
    class statement{
	public:
	    virtual ~statement();
	    virtual void eval(const code::generator&gen)const=0;
    };
    class single:public statement{
	public:
	    const node*const stat;
	    single(node*stat);
	    ~single()override;
	    void eval(const code::generator&gen)const override;
    };
    class compound:public statement{
	    std::vector<statement*>stats;
	public:
	    ~compound()override;
	    void eval(const code::generator&gen)const override;
	    void push_back_stat(statement*st);
    };
    class declare:public statement{
	    std::vector<std::pair<std::string,node*>>vars;
	public:
	    ~declare()override;
	    void eval(const code::generator&gen)const override;
	    void push_back_var(std::pair<std::string,node*>var);
    };
    class _if_else_:public statement{
	public:
	    const single*const cond;
	    const statement*const st1,*const st2;
	    _if_else_(single*cond,statement*st1,statement*st2);
	    ~_if_else_()override;
	    void eval(const code::generator&gen)const override;
    };
    class _while_:public statement{
	public:
	    const single*const cond;
	    const statement*const st;
	    _while_(single*cond,statement*st);
	    ~_while_()override;
	    void eval(const code::generator&gen)const override;
    };
    class _for_:public statement{
	public:
	    const single*const init,*const cond,*const reinit;
	    const statement*const st;
	    _for_(single*init,single*cond,single*reinit,statement*st);
	    ~_for_()override;
	    void eval(const code::generator&gen)const override;
    };
}
