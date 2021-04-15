#include "lexer.hpp"
#include "compilation_error.hpp"
#include <algorithm>
#include <regex>
using namespace lexicon;
lexer::lexer(const std::string &src)
    : src(src)
{
}
bool lexer::skip_spaces()
{
    bool is_skipped = false;
    while (pos < src.length() && std::isspace(src[pos])) {
        is_skipped = true;
        if (src[pos++] == '\n') {
            ++line;
            col = 1;
        } else {
            ++col;
        }
    }
    return is_skipped;
}
bool lexer::skip_comments()
{
    auto seek_to_comment_close = [this] {
        const int line_original = line, col_original = col;
        while (pos < src.length()) {
            if (src.substr(pos).starts_with("*/")) {
                pos += 2;
                col += 2;
                return;
            } else if (src[pos++] == '\n') {
                ++line;
                col = 1;
            } else {
                ++col;
            }
        }
        throw exception::compilation_error("コメントが閉じられていません", line_original, col_original);
    };
    bool is_skipped = false;
    while (pos < src.length() && src.substr(pos).starts_with("/*")) {
        is_skipped = true;
        pos += 2;
        seek_to_comment_close();
    }
    return is_skipped;
}
void lexer::skip_spaces_and_comments()
{
    while (skip_spaces() || skip_comments()) {
        // PASS
    }
}
int lexer::get_line() const noexcept
{
    return line;
}
int lexer::get_column() const noexcept
{
    return col;
}
bool lexer::is_all_read()
{
    skip_spaces_and_comments();
    return pos >= src.length();
}
std::optional<numeric> lexer::consume_numeric()
{
    skip_spaces_and_comments();
    std::smatch m;
    auto unread = src.substr(pos);
    if (std::regex_search(unread, m, std::regex(R"(^\d+)"))) {
        pos += m.length();
        col += m.length();
        return numeric(std::stoi(m.str()), line, col);
    }
    return std::nullopt;
}
std::optional<identifier> lexer::consume_identifier()
{
    skip_spaces_and_comments();
    std::smatch m;
    auto unread = src.substr(pos);
    if (std::regex_search(unread, m, std::regex(R"(^[_A-z]+\w*)"))) {
        if (!symbol::lexeme_to_id(m.str())) {
            pos += m.length();
            col += m.length();
            return identifier(m.str(), line, col);
        }
    }
    return std::nullopt;
}
bool lexer::check_symbol(symbol::symid id)
{
    skip_spaces_and_comments();
    return id == symbol::longest_forward_match(src.substr(pos));
}
std::optional<symbol> lexer::consume_symbol_if(symbol::symid id)
{
    if (check_symbol(id)) {
        std::string lexeme = symbol::id_to_lexeme(id);
        pos += lexeme.length();
        col += lexeme.length();
        return symbol(id, line, col);
    }
    return std::nullopt;
}
std::optional<symbol> lexer::consume_type_specifier()
{
    return consume_symbol_if(symbol::symid::INT);
}
