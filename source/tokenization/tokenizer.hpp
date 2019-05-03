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
	    token*const operator[](int idx)const;
	    size_t size()const;
    };
}
