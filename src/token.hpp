#pragma once
#include<optional>
#include<string>
#include<utility>
namespace lexicon{
    class token{
        public:
            const int line,col;
            token(int line,int col);
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
        using token::token;
        public:
            enum SYMBOL{
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
            };
            static std::optional<std::pair<SYMBOL,int>>match(const std::string&str,int pos);
            symbol&operator=(const symbol&);
    };
    class token_array final{
        private:
            std::string src;
            int pos;
            int line,col;
            void skip_space_or_comment();
        public:
            token_array(const std::string&src);
            int get_line()const noexcept;
            int get_column()const noexcept;
            bool is_all_read();
            std::optional<numeric>consume_numeric();
            std::optional<identifier>consume_identifier();
            bool check_symbol(symbol::SYMBOL sym);
            std::optional<symbol>consume_symbol(symbol::SYMBOL sym);
            token_array&operator=(const token_array&)=delete;
            token_array&operator=(token_array&&)=delete;
    };
}
