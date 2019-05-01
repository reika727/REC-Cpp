#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>
#include<exception>
#include<cmath>
namespace mycc{
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
	LEEQ,    // <=
	GREAT,   // >
	GREQ,    // >=
	RETURN,  // return
	SCOLON,  // ;
	OPARENT, // (
	CPARENT, // )
	NUMERIC, // 数値
	IDENT,   // 識別子
	ENDT,    // トークン列終端
    };
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
	/*キーワード*/
	RETURN,   // return
	/*その他*/
	NUMERIC,  // 数値リテラル
	IDENT,    // 識別子
    };
    class tokenizer{
	public:
	    struct token{
		TK type;
		int value;
		std::string name;
		token(TK type);
		token(TK type,int value);
		token(TK type,const std::string&name);
	    };
	private:
	    std::vector<token>tokens;
	public:
	    tokenizer(const std::string&s);
	    const token&operator[](int idx)const;
    };
    class abstract_syntax_tree{
	public:
	    struct node{
		ND type;
		node*lhs,*rhs;
		int value;
		std::string name;
		node(node*const left,ND type,node*const right);
		node(int value);
		node(const std::string&name);
	    };
	private:
	    const tokenizer&tk;
	    int pos_now;
	    std::vector<node*>stats;
	    bool consume(TK type);
	    node*statement();
	    node*assign();
	    node*equality();
	    node*relational();
	    node*add();
	    node*mul();
	    node*unary();
	    node*term();
	public:
	    abstract_syntax_tree(const tokenizer&_tk);
	    const std::vector<node*>&statements();
    };
    class assembly_source{
	private:
	    std::ofstream ofs;
	    int indent;
	    int var_size;
	    std::map<std::string,int>offset;
	    std::string p(const std::string&str);
	    std::string derefer(int dis,const std::string&base,const std::string&ofs="",int scl=1);
	    std::string derefer(int dis,const std::string&base,int scl);
	    std::string derefer(const std::string&base,const std::string&ofs,int scl=1);
	    std::string derefer(const std::string&base,int scl=1);
	    void enumerate_var(abstract_syntax_tree::node*const node);
	    void refer_var(abstract_syntax_tree::node*const node);
	    void RDP(abstract_syntax_tree::node*const node);
	public:
	    assembly_source(const std::string&filename);
	    void write(const std::string&str);
	    void write(const std::string&inst,const std::string&reg1,const std::string&reg2);
	    void write(const std::string&inst,int arg,const std::string&reg);
	    void write(const std::string&inst,const std::string&reg);
	    void write(const std::string&inst,int arg);
	    void eval(abstract_syntax_tree::node*const node);
	    void enter(const std::string&func);
	    void leave();
    };
}
