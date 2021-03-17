#include "lexer.hpp"
#include "compilation_error.hpp"
#include <algorithm>
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
            if (src[pos] == '\n') {
                ++pos;
                ++line;
                col = 1;
            } else if (src.substr(pos, 2) == "*/") {
                pos += 2;
                col += 2;
                return;
            } else {
                ++pos;
                ++col;
            }
        }
        throw exception::compilation_error("コメントが閉じられていません", line_original, col_original);
    };
    bool is_skipped = false;
    while (pos < src.length() && src.substr(pos, 2) == "/*") {
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
    if (is_all_read() || !std::isdigit(src[pos])) {
        return std::nullopt;
    }
    std::size_t sz;
    int num = std::stoi(src.substr(pos), &sz);
    auto ret = numeric(num, line, col);
    pos += sz;
    col += sz;
    return ret;
}
std::optional<identifier> lexer::consume_identifier()
{
    auto get_identifier_name_from_string = [](const std::string &str, std::string::size_type pos) {
        std::string identifire_name;
        for (auto c : str.substr(pos)) {
            if (!std::isalpha(c) && !std::isdigit(c) && c != '_') {
                break;
            }
            identifire_name += c;
        }
        return identifire_name;
    };
    skip_spaces_and_comments();
    if (is_all_read() || (!std::isalpha(src[pos]) && src[pos] != '_')) {
        return std::nullopt;
    }
    auto name = get_identifier_name_from_string(src, pos);
    auto ret = identifier(name, line, col);
    pos += name.length();
    col += name.length();
    return ret;
}
std::optional<symbol> lexer::consume_symbol()
{
    skip_spaces_and_comments();
    if (is_all_read()) {
        return std::nullopt;
    }
    if (auto m = symbol::match(src, pos)) {
        auto ret = symbol(m->first, line, col);
        pos += m->second;
        col += m->second;
        return ret;
    } else {
        return std::nullopt;
    }
}
bool lexer::check_symbol(symbol::symid id)
{
    skip_spaces_and_comments();
    if (is_all_read()) {
        return false;
    }
    auto m = symbol::match(src, pos);
    return m.has_value() && m->first == id;
}
std::optional<symbol> lexer::consume_symbol_if(symbol::symid id)
{
    return check_symbol(id) ? consume_symbol() : std::nullopt;
}
