#pragma once
#include "token.hpp"
#include <optional>
namespace lexicon {
    class lexer final {
    private:
        std::string src;
        std::string::size_type pos = 0;
        int line = 1, col = 1;
        bool skip_spaces();
        bool skip_comments();
        void skip_spaces_and_comments();

    public:
        lexer(const std::string &src);
        int get_line() const noexcept;
        int get_column() const noexcept;
        bool is_all_read();
        std::optional<numeric> consume_numeric();
        std::optional<identifier> consume_identifier();
        bool check_symbol(symbol::symid id);
        std::optional<symbol> consume_symbol_if(symbol::symid id);
        std::optional<symbol> consume_type_specifier();
    };
}
