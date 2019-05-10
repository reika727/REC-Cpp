#pragma once
#include"statements.hpp"
#include"../tokenization/token_array.hpp"
namespace abstract_syntax_tree{
    class tree{
	    tokenization::token_array&ta;
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
	    tree(tokenization::token_array&ta);
	    ~tree();
	    compound*root();
    };
}
