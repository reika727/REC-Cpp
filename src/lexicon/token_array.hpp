#pragma once
#include"token.hpp"
#include<memory>
#include<string>
#include<vector>
namespace lexicon{
    class token_array final{
        private:
            std::vector<std::shared_ptr<const token>>tv;
            std::vector<std::shared_ptr<const token>>::const_iterator itr;
        public:
            token_array(const std::string&s);
            bool check(TK type)const noexcept;
            std::shared_ptr<const token>consume(TK type)noexcept;
            bool is_all_read()const noexcept;
    };
}
