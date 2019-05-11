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
	    const node*order15();
	    const node*order14();
	    const node*order07();
	    const node*order06();
	    const node*order04();
	    const node*order03();
	    const node*order02();
	    const node*order01();
	public:
	    tree(lexicon::token_array&ta);
	    ~tree();
	    const compound*root();
    };
}
