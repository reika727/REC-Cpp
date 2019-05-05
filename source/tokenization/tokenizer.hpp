#pragma once
#include"token.hpp"
#include<vector>
namespace tokenization{
    class tokenizer{
	private:
	    std::vector<token*>tokens;
	public:
	    tokenizer(const std::string&s);
	    ~tokenizer();
	    std::vector<token*>::const_iterator begin()const;
	    std::vector<token*>::const_iterator end()const;
    };
}
