#pragma once
#include"statements.hpp"
#include"../lexicon/token_array.hpp"
namespace syntax{
    class tree{
	    lexicon::token_array&ta;
	    compound*rt;
	private:
	    statement*stat();
	    single*emptiable_single();
	    node*assign();
	    node*equality();
	    node*relational();
	    node*add();
	    node*mul();
	    node*unary();
	    node*term();
	public:
	    tree(lexicon::token_array&ta);
	    ~tree();
	    compound*root();
    };
}
