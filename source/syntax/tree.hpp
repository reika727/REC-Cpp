#pragma once
#include"nodes.hpp"
#include"statements.hpp"
#include"../lexicon/token_array.hpp"
namespace syntax{
    class tree{
	    lexicon::token_array&ta;
	    compound*rt;
	private:
	    const statement*stat();
	    const single*emptiable_single();
	    const node*asgn();
	    const node*equality();
	    const node*relational();
	    const node*add();
	    const node*mul();
	    const node*unary();
	    const node*term();
	public:
	    tree(lexicon::token_array&ta);
	    ~tree();
	    const compound*root();
    };
}
