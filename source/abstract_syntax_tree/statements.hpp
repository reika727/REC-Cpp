#pragma once
#include<utility>
#include"nodes.hpp"
namespace abstract_syntax_tree{
    struct statement{
	virtual ~statement();
    };
    struct single:public statement{
	node*stat;
	single(node*stat);
	~single()override;
	bool is_nop();
    };
    struct compound:public statement{
	std::vector<statement*>stats;
	~compound()override;
    };
    struct declare:public statement{
	std::vector<std::pair<std::string,node*>>vars;
	~declare()override;
    };
    struct _if_:public statement{
	single*cond;
	statement*st;
	_if_(single*cond,statement*st);
	~_if_()override;
    };
    struct _else_:public statement{
	statement*st;
	_else_(statement*st);
	~_else_()override;
    };
    struct _while_:public statement{
	single*cond;
	statement*st;
	_while_(single*cond,statement*st);
	~_while_()override;
    };
    struct _for_:public statement{
	single*init,*cond,*reinit;
	statement*st;
	_for_(single*init,single*cond,single*reinit,statement*st);
	~_for_()override;
    };
}
