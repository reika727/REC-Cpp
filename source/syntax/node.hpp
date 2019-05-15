#pragma once
#include"../semantics/analyzer.hpp"
#include"../code/generator.hpp"
namespace syntax{
    class node{
	public:
	    virtual ~node()=0;
	    virtual void check(semantics::analyzer&analy)const=0;
	    virtual void to_asm(code::generator&gen)const=0;
    };
}
