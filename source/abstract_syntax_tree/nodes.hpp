#pragma once
#include<string>
#include<vector>
#include<utility>
namespace abstract_syntax_tree{
    enum class ND{
	/*算術演算子*/
	UPLUS,    // 単項プラス
	UMINUS,   // 単項マイナス
	PLUS,     // 加算
	MINUS,    // 減算
	MULTI,    // 乗算
	DIVIDE,   // 除算
	REMAIN,   // 剰余
	PREINC,   // 前置インクリメント
	PREDEC,   // 前置デクリメント
	ASSIGN,   // 単純代入
	PLASGN,   // 加算代入
	MIASGN,   // 減算代入
	MUASGN,   // 乗算代入
	DIASGN,   // 除算代入
	RMASGN,   // 剰余代入
	/*比較演算子*/
	EQUAL,    // 等価
	NEQUAL,   // 非等価
	LESS,     // 小なり
	GREAT,    // 大なり
	LEEQ,     // 小なりイコール
	GREQ,     // 大なりイコール
    };
    struct node{
	virtual ~node();
    };
    struct numeric:public node{
	int value;
	numeric(int value);
    };
    struct ident:public node{
	std::string name;
	ident(const std::string&name);
    };
    struct fcall:public node{
	std::string name;
	std::vector<node*>args;
	fcall(const std::string&name);
	~fcall()override;
    };
    struct unopr:public node{
	ND type;
	node*arg;
	unopr(ND type,node*arg);
	~unopr()override;
    };
    struct biopr:public node{
	ND type;
	node*larg,*rarg;
	biopr(node*left,ND type,node*right);
	~biopr()override;
    };
    struct statement:public node{
	virtual ~statement();
    };
    struct single:public statement{
	node*stat;
	single(node*stat);
	~single()override;
	bool is_nop();
    };
    struct compound:public statement{
	std::vector<statement*>stats;
	~compound()override;
    };
    struct declare:public statement{
	std::vector<std::pair<std::string,node*>>vars;
	~declare()override;
    };
    struct _if_:public statement{
	single*cond;
	statement*st;
	_if_(single*cond,statement*st);
	~_if_()override;
    };
    struct _else_:public statement{
	statement*st;
	_else_(statement*st);
	~_else_()override;
    };
    struct _while_:public statement{
	single*cond;
	statement*st;
	_while_(single*cond,statement*st);
	~_while_()override;
    };
    struct _for_:public statement{
	single*init,*cond,*reinit;
	statement*st;
	_for_(single*init,single*cond,single*reinit,statement*st);
	~_for_()override;
    };
}
