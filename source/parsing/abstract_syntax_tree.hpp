#pragma once
#include"node.hpp"
#include"../tokenization/tokenizer.hpp"
#include<vector>
namespace parsing{
    class abstract_syntax_tree{
	private:
	    compound*prog;
	    const tokenization::tokenizer&tk;
	    int pos_now;
	    bool consume(tokenization::TK type);
	    statement*stat();
	    node*assign();
	    node*equality();
	    node*relational();
	    node*add();
	    node*mul();
	    node*unary();
	    node*term();
	public:
	    abstract_syntax_tree(const tokenization::tokenizer&_tk);
	    ~abstract_syntax_tree();
	    const std::vector<statement*>&statements();
    };
}
