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
    if (auto itr = std::find(lexeme_table.begin(), lexeme_table.end(), lexeme); itr != lexeme_table.end()) {
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
    std::string result = "";
    for (auto lexeme : lexeme_table) {
        if (src.substr(0, lexeme.length()) == lexeme) {
            if (lexeme.length() > result.length()) {
                result = lexeme;
            }
        }
    }
    return lexeme_to_id(result);
}
