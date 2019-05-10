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
	GREAT,   // >
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
    struct token{
	virtual ~token();
    };
    struct numeric:public token{
	int value;
	numeric(int value);
    };
    struct ident:public token{
	std::string name;
	ident(const std::string&name);
    };
    struct symbol:public token{
	TK type;
	symbol(TK type);
    };
}
