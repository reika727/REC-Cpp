#pragma once
#include"token.hpp"
#include<string>
#include<vector>
namespace lexicon{
    class token_array final{
            std::vector<const token*>tv;
            decltype(tv)::const_iterator itr;
        public:
            token_array(const std::string&s);
            ~token_array();
            bool check(TK type)const noexcept;
            const token*consume(TK type)noexcept;
            bool is_all_read()const noexcept;
    };
}
