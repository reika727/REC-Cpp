#pragma once
#include<string>
namespace lexicon{
    enum class TK{
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
        IF,       // if
        ELSE,     // else
        WHILE,    // while
        FOR,      // for
        BREAK,    // break
        CONTINUE, // continue
        RETURN,   // return
        NUMERIC,  // numeric
        IDENT,    // ident
    };
    class token{
        public:
            const TK type;
            token(TK type);
            virtual ~token()=default;
    };
    class numeric:public token{
        public:
            const int value;
            numeric(int value);
    };
    class ident:public token{
        public:
            const std::string name;
            ident(const std::string&name);
    };
    class symbol:public token{
        public:
            symbol(TK type);
    };
}
