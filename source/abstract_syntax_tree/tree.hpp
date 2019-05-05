#pragma once
#include"node.hpp"
#include"../tokenization/reader.hpp"
#include<vector>
namespace abstract_syntax_tree{
    class tree{
	private:
	    tokenization::reader tr;
	    compound*rt;
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
	    tree(const tokenization::tokenizer&tk);
	    ~tree();
	    const std::vector<statement*>&root();
    };
}
