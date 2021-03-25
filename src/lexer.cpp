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
std::optional<numeric> lexer::get_matching_numeric(std::size_t *sz)
{
    skip_spaces_and_comments();
    std::smatch m;
    auto unread = src.substr(pos);
    if (std::regex_search(unread, m, std::regex(R"(^\d+)"))) {
        if (sz) {
            *sz = m.length();
        }
        return numeric(std::stoi(m.str()), line, col);
    }
    return std::nullopt;
}
std::optional<identifier> lexer::get_matching_identifier(std::size_t *sz)
{
    skip_spaces_and_comments();
    std::smatch m;
    auto unread = src.substr(pos);
    if (std::regex_search(unread, m, std::regex(R"(^[_A-z]+\w*)"))) {
        if (!symbol::lexeme_to_id(m.str())) {
            if (sz) {
                *sz = m.length();
            }
            return identifier(m.str(), line, col);
        }
    }
    return std::nullopt;
}
std::optional<symbol> lexer::get_matching_symbol(std::size_t *sz)
{
    skip_spaces_and_comments();
    auto lfm = symbol::longest_forward_match(src.substr(pos));
    if (auto id = symbol::lexeme_to_id(lfm)) {
        if (sz) {
            *sz = lfm.length();
        }
        return symbol(id.value(), line, col);
    }
    return std::nullopt;
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
    std::size_t sz;
    auto ms = get_matching_numeric(&sz);
    if (ms) {
        pos += sz;
        col += sz;
    }
    return ms;
}
std::optional<identifier> lexer::consume_identifier()
{
    std::size_t sz;
    auto ms = get_matching_identifier(&sz);
    if (ms) {
        pos += sz;
        col += sz;
    }
    return ms;
}
bool lexer::check_symbol(symbol::symid id)
{
    if (auto ms = get_matching_symbol()) {
        return ms->id == id;
    } else {
        return false;
    }
}
std::optional<symbol> lexer::consume_symbol_if(symbol::symid id)
{
    std::size_t sz;
    auto ms = get_matching_symbol(&sz);
    if (ms && ms->id == id) {
        pos += sz;
        col += sz;
        return ms;
    }
    return std::nullopt;
}
