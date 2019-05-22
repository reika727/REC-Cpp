#pragma once
#include"token.hpp"
#include<string>
#include<vector>
namespace lexicon{
    class token_array{
	    std::vector<const token*>tv;
	    decltype(tv)::const_iterator itr;
	public:
	    token_array(const std::string&s);
	    ~token_array();
	    bool check(TK type);
	    const token*consume(TK type);
	    bool is_all_read();
	    token_array&operator=(const token_array&)=delete;
    };
}
