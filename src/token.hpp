#pragma once
#include<string>
namespace lexicon{
    class token{
        public:
            const int line,col;
            token(int line,int col);
    };
    class numeric final:public token{
        public:
            const int value;
            numeric(int value,int line,int col);
    };
    class identifier final:public token{
        public:
            const std::string name;
            identifier(const std::string&name,int line,int col);
    };
    class symbol final:public token{
        public:
            const enum SYMBOL{
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
            }sym;
            symbol(SYMBOL sym,int line,int col);
            symbol&operator=(const symbol&);
    };
}
