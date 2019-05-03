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
	GREAT,   // >
	LEEQ,    // <=
	GREQ,    // >=
	SCOLON,  // ;
	OPARENT, // (
	CPARENT, // )
	OBRACE,  // {
	CBRACE,  // }
	IF,      // if
	ELSE,    // else
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
    };
    class tokenizer{
	public:
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
	private:
	    std::vector<token*>tokens;
	public:
	    tokenizer(const std::string&s);
	    ~tokenizer();
	    token*const operator[](int idx)const;
	    size_t size()const;
    };
    class abstract_syntax_tree{
	public:
	    struct node{
		virtual ~node();
	    };
	    struct numeric:public node{
		int value;
		numeric(int value);
		~numeric()override;
	    };
	    struct ident:public node{
		std::string name;
		ident(const std::string&name);
		~ident()override;
	    };
	    struct unopr:public node{
		ND type;
		node*arg;
		unopr(ND type,node*const arg);
		~unopr()override;
	    };
	    struct biopr:public node{
		ND type;
		node*larg,*rarg;
		biopr(node*const left,ND type,node*const right);
		~biopr()override;
	    };
	    struct statement:public node{
		virtual ~statement();
	    };
	    struct single:public statement{
		node*stat;
		single(node*stat);
		~single()override;
	    };
	    struct compound:public statement{
		std::vector<statement*>stats;
		compound();
		~compound()override;
		void push_back(statement*const st);
	    };
	    struct _if_:public statement{
		single*cond;
		statement*st;
		_if_(single*const cond,statement*const st);
		~_if_()override;
	    };
	    struct _else_:public statement{
		statement*st;
		_else_(statement*const st);
		~_else_()override;
	    };
	private:
	    compound*prog;
	    const tokenizer&tk;
	    int pos_now;
	    bool consume(TK type);
	    statement*stat();
	    node*assign();
	    node*equality();
	    node*relational();
	    node*add();
	    node*mul();
	    node*unary();
	    node*term();
	public:
	    abstract_syntax_tree(const tokenizer&_tk);
	    ~abstract_syntax_tree();
	    const std::vector<statement*>&statements();
    };
    class assembly_source{
	private:
	    std::ofstream ofs;
	    int indent;
	    int var_size;
	    std::map<std::string,int>offset;
	    unsigned int serial;
	    void write(const std::string&str);
	    void write(const std::string&inst,const std::string&reg1,const std::string&reg2);
	    void write(const std::string&inst,int arg,const std::string&reg);
	    void write(const std::string&inst,const std::string&reg);
	    void write(const std::string&inst,int arg);
	    std::string p(const std::string&str);
	    std::string derefer(int dis,const std::string&base,const std::string&ofs="",int scl=1);
	    std::string derefer(int dis,const std::string&base,int scl);
	    std::string derefer(const std::string&base,const std::string&ofs,int scl=1);
	    std::string derefer(const std::string&base,int scl=1);
	    void eval(abstract_syntax_tree::statement*const st);
	    void eval(abstract_syntax_tree::single*const sg);
	    void enumerate_var(abstract_syntax_tree::node*const node);
	    void refer_var(abstract_syntax_tree::node*const node);
	    void RDP(abstract_syntax_tree::node*const node);
	    std::string label(const std::string&base);
	public:
	    assembly_source(const std::string&filename);
	    void eval(const std::vector<abstract_syntax_tree::statement*>&sv);
	    void enter(const std::string&func);
	    void leave();
    };
}
