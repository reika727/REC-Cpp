#pragma once
#include"tokens.hpp"
#include<vector>
namespace tokenization{
    class token_array{
	    std::vector<token*>tv;
	    std::vector<token*>::const_iterator itr;
	public:
	    token_array(const std::string&s);
	    ~token_array();
	    const std::vector<token*>::const_iterator&pos();
	    bool consume(tokenization::TK type);
	    int*consume_num();
	    std::string*consume_id();
	    bool is_all_read();
    };
}
