#pragma once
#include<string>
namespace lexicon{
    enum class TK{
	PLUS,    // +
	MINUS,   // -
	ASTER,   // *
	SLASH,   // /
	PERCENT, // %
	PLPL,    // ++
	MIMI,    // --
	EQUAL,   // =
	PLEQ,    // +=
	MIEQ,    // -=
	ASEQ,    // *=
	SLEQ,    // /=
	PEEQ,    // %=
	EQEQ,    // ==
	EXEQ,    // !=
	LESS,    // <
	GREATER, // >
	LEEQ,    // <=
	GREQ,    // >=
	SCOLON,  // ;
	OPARENT, // (
	CPARENT, // )
	OBRACE,  // {
	CBRACE,  // }
	COMMA,   // ,
	CHAR,    // char
	IF,      // if
	ELSE,    // else
	WHILE,   // while
	FOR,     // for
    };
    class token{
	public:
	    virtual ~token()=0;
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
	    const TK type;
	    symbol(TK type);
    };
}
