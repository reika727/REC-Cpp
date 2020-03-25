#pragma once
#include"token.hpp"
#include<memory>
#include<string>
#include<vector>
namespace lexicon{
    class token_array final{
        private:
            std::vector<std::shared_ptr<const token>>tokens;
            std::vector<std::shared_ptr<const token>>::const_iterator itr;
        public:
            token_array(const std::string&s);
            std::shared_ptr<const token>consume(TK type)noexcept;
            bool is_all_read()const noexcept;
    };
}
