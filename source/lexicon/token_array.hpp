#pragma once
#include"tokens.hpp"
#include<vector>
namespace lexicon{
    class token_array{
	    std::vector<token*>tv;
	    decltype(tv)::const_iterator itr;
	public:
	    token_array(const std::string&s);
	    ~token_array();
	    const decltype(itr)&pos();
	    bool consume(TK type);
	    const int*consume_num();
	    const std::string*consume_id();
	    bool is_all_read();
    };
}
