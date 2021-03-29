#include "token.hpp"
#include <algorithm>
using namespace lexicon;
token::token(int line, int col)
    : line(line), col(col)
{
}
numeric::numeric(int value, int line, int col)
    : token(line, col), value(value)
{
}
identifier::identifier(const std::string &name, int line, int col)
    : token(line, col), name(name)
{
}
symbol::symbol(symbol::symid id, int line, int col)
    : token(line, col), id(id)
{
}
symbol &symbol::operator=(const symbol &)
{
    return *this;
}
std::optional<symbol::symid> symbol::lexeme_to_id(const std::string &lexeme)
{
    if (auto itr = std::ranges::find(lexeme_table, lexeme); itr != lexeme_table.end()) {
        return static_cast<symbol::symid>(itr - lexeme_table.begin());
    } else {
        return std::nullopt;
    }
}
std::string symbol::id_to_lexeme(symid id)
{
    return lexeme_table.at(static_cast<decltype(lexeme_table)::size_type>(id));
}
std::optional<symbol::symid> symbol::longest_forward_match(const std::string &src)
{
    auto src_contains = [src](const std::string &lexeme) {
        return src.starts_with(lexeme);
    };
    if (auto range = lexeme_table | std::views::filter(src_contains)) {
        return lexeme_to_id(*std::ranges::max_element(range, {}, &std::string::length));
    } else {
        return std::nullopt;
    }
}
