#pragma once
#include<memory>
#include<string>
#include<queue>
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
    };
    class token_array final{
        private:
            std::queue<std::shared_ptr<const token>>tokens;
        public:
            token_array(const std::string&src);
            bool is_all_read()const noexcept;
            int get_line()const noexcept;
            int get_column()const noexcept;
            bool check_symbol(symbol::SYMBOL sym)const noexcept;
            std::shared_ptr<const numeric>consume_numeric()noexcept;
            std::shared_ptr<const identifier>consume_identifier()noexcept;
            std::shared_ptr<const symbol>consume_symbol(symbol::SYMBOL sym)noexcept;
            token_array&operator=(const token_array&)=delete;
            token_array&operator=(token_array&&)=delete;
    };
}
