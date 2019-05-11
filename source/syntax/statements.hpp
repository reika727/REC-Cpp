#pragma once
#include<utility>
#include"nodes.hpp"
#include"../code/generator.hpp"
namespace syntax{
    struct statement{
	virtual ~statement();
	virtual void eval(const code::generator&gen);
    };
    struct single:public statement{
	node*stat;
	single(node*stat);
	~single()override;
	void eval(const code::generator&gen)override;
    };
    struct compound:public statement{
	std::vector<statement*>stats;
	~compound()override;
	void eval(const code::generator&gen)override;
    };
    struct declare:public statement{
	std::vector<std::pair<std::string,node*>>vars;
	~declare()override;
	void eval(const code::generator&gen)override;
    };
    struct _if_else_:public statement{
	single*cond;
	statement*st1,*st2;
	_if_else_(single*cond,statement*st1,statement*st2);
	~_if_else_()override;
	void eval(const code::generator&gen)override;
    };
    struct _while_:public statement{
	single*cond;
	statement*st;
	_while_(single*cond,statement*st);
	~_while_()override;
	void eval(const code::generator&gen)override;
    };
    struct _for_:public statement{
	single*init,*cond,*reinit;
	statement*st;
	_for_(single*init,single*cond,single*reinit,statement*st);
	~_for_()override;
	void eval(const code::generator&gen)override;
    };
}
