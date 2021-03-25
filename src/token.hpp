#pragma once
#include <optional>
#include <string>
#include <vector>
namespace lexicon {
    class token {
    public:
        const int line, col;
        token(int line, int col);
    };
    class numeric final : public token {
    public:
        const int value;
        numeric(int value, int line, int col);
    };
    class identifier final : public token {
    public:
        const std::string name;
        identifier(const std::string &name, int line, int col);
    };
    class symbol final : public token {
    private:
        inline static std::vector<std::string> lexeme_table{
            "+",
            "-",
            "*",
            "/",
            "%",
            "<",
            ">",
            "!",
            "=",
            ",",
            ";",
            "(",
            ")",
            "{",
            "}",
            "&&",
            "||",
            "++",
            "--",
            "+=",
            "-=",
            "*=",
            "/=",
            "%=",
            "==",
            "!=",
            "<=",
            ">=",
            "int",
            "void",
            "if",
            "else",
            "while",
            "for",
            "break",
            "continue",
            "return",
        };

    public:
        const enum class symid {
            PLUS,     // +
            MINUS,    // -
            ASTER,    // *
            SLASH,    // /
            PERCENT,  // %
            LESS,     // <
            GREATER,  // >
            EXCLAM,   // !
            EQUAL,    // =
            COMMA,    // ,
            SCOLON,   // ;
            OPARENT,  // (
            CPARENT,  // )
            OBRACE,   // {
            CBRACE,   // }
            APAP,     // &&
            VBVB,     // ||
            PLPL,     // ++
            MIMI,     // --
            PLEQ,     // +=
            MIEQ,     // -=
            ASEQ,     // *=
            SLEQ,     // /=
            PEEQ,     // %=
            EQEQ,     // ==
            EXEQ,     // !=
            LEEQ,     // <=
            GREQ,     // >=
            INT,      // int
            VOID,     // void
            IF,       // if
            ELSE,     // else
            WHILE,    // while
            FOR,      // for
            BREAK,    // break
            CONTINUE, // continue
            RETURN,   // return
        } id;
        static std::optional<symid> lexeme_to_id(const std::string &lexeme);
        static std::string id_to_lexeme(symid id);
        static std::string longest_forward_match(const std::string &src);
        symbol(symid id, int line, int col);
        // TODO どうにかして失くしたい
        symbol &operator=(const symbol &);
    };
}
