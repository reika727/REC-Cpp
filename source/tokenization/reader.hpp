#pragma once
#include"tokenizer.hpp"
#include<vector>
namespace tokenization{
    class reader{
	private:
	    const tokenization::tokenizer&tk;
	public:
	    std::vector<tokenization::token*>::const_iterator pos;
	    reader(const tokenization::tokenizer&tk);
	    bool consume(tokenization::TK type);
    };
}
