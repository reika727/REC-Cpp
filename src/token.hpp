#pragma once
#include<memory>
#include<string>
#include<queue>
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
        VOID,     // void
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
            const int line,col;
            token(TK type,int line,int col);
            virtual ~token()=0;
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
            symbol(TK type,int line,int col);
    };
    class token_array final{
        private:
            std::queue<std::shared_ptr<const token>>tokens;
        public:
            token_array(const std::string&src);
            bool is_all_read()const noexcept;
            bool check(TK type)const noexcept;
            int get_line()const noexcept;
            int get_column()const noexcept;
            std::shared_ptr<const token>consume(TK type)noexcept;
            token_array&operator=(const token_array&)=delete;
            token_array&operator=(token_array&&)=delete;
    };
}
