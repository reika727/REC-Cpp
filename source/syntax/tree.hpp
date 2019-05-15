#pragma once
#include"expressions.hpp"
#include"statements.hpp"
#include"../lexicon/token_array.hpp"
namespace syntax{
    class tree{
	    lexicon::token_array&ta;
	    compound*root;
	private:
	    const statement*stat();
	    const single*emptiable_single();
	    const expression*order15();
	    const expression*order14();
	    const expression*order07();
	    const expression*order06();
	    const expression*order04();
	    const expression*order03();
	    const expression*order02();
	    const expression*order01();
	    const expression*order00();
	public:
	    tree(lexicon::token_array&ta);
	    ~tree();
	    const compound*get_root();
    };
}
