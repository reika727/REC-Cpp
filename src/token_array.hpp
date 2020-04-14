#pragma once
#include"token.hpp"
#include<optional>
#include<utility>
namespace lexicon{
    class token_array final{
        private:
            std::string src;
            int pos;
            int line,col;
            void skip_space_or_comment();
            std::optional<std::pair<symbol::SYMBOL,int>>match();
        public:
            token_array(const std::string&src);
            int get_line()const noexcept;
            int get_column()const noexcept;
            bool is_all_read();
            std::optional<numeric>consume_numeric();
            std::optional<identifier>consume_identifier();
            bool check_symbol(symbol::SYMBOL sym);
            std::optional<symbol>consume_symbol(symbol::SYMBOL sym);
            token_array&operator=(const token_array&)=delete;
            token_array&operator=(token_array&&)=delete;
    };
}
