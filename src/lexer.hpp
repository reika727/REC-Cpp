#pragma once
#include"token.hpp"
#include<optional>
namespace lexicon{
    class lexer final{
        private:
            std::string src;
            int pos;
            int line,col;
            void skip_space_or_comment();
        public:
            lexer(const std::string&src);
            int get_line()const noexcept;
            int get_column()const noexcept;
            bool is_all_read();
            std::optional<numeric>consume_numeric();
            std::optional<identifier>consume_identifier();
            std::optional<symbol>consume_symbol();
            bool check_symbol(symbol::SYMBOL sym);
            std::optional<symbol>consume_symbol_if(symbol::SYMBOL sym);
            lexer&operator=(const lexer&)=delete;
            lexer&operator=(lexer&&)=delete;
    };
}
