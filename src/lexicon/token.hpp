#pragma once
#include<string>
namespace lexicon{
    enum class TK{
	PLUS,    // +
	MINUS,   // -
	ASTER,   // *
	SLASH,   // /
	PERCENT, // %
	EQUAL,   // =
	LESS,    // <
	GREATER, // >
	EXCLAM,  // !
	COMMA,   // ,
	SCOLON,  // ;
	OPARENT, // (
	CPARENT, // )
	OBRACE,  // {
	CBRACE,  // }
	PLPL,    // ++
	MIMI,    // --
	PLEQ,    // +=
	MIEQ,    // -=
	ASEQ,    // *=
	SLEQ,    // /=
	PEEQ,    // %=
	EQEQ,    // ==
	EXEQ,    // !=
	LEEQ,    // <=
	GREQ,    // >=
	APAP,    // &&
	VBVB,    // ||
	INT,     // int
	IF,      // if
	ELSE,    // else
	WHILE,   // while
	FOR,     // for
	RETURN,  // return
	NUMERIC, // numeric
	IDENT,   // ident
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
