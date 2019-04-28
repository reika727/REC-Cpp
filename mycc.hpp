#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<exception>
#include<cmath>
namespace mycc{
    enum{
	TK_NUM=0x100,
	TK_EQ,
	TK_NE,
	TK_LE,
	TK_GE,
	TK_IDENT,
	TK_EOF,
    };
    enum{
	ND_NUM=0x100,
	ND_IDENT,
    };
    class tokenizer{
	public:
	    struct token{
		int type;
		int value;
		char name;
		token(int type);
		token(int type,int value);
		token(int type,char name);
	    };
	private:
	    std::vector<token>tokens;
	public:
	    tokenizer(const std::string&s);
	    const token&operator()(int idx)const;
    };
    class abstract_syntax_tree{
	public:
	    struct node{
		int type;
		node*lhs,*rhs;
		int value;
		char name;
		node(node*left,int type,node*right);
		node(int value);
		node(char name);
	    };
	private:
	    const tokenizer&tk;
	    int pos_now;
	    std::vector<node*>stats;
	    bool consume(int type);
	    node*statement();
	    node*assign();
	    node*equality();
	    node*relational();
	    node*add();
	    node*mul();
	    node*unary();
	    node*term();
	public:
	    abstract_syntax_tree(tokenizer&_tk);
	    const std::vector<node*>&statements()const;
    };
    class assembly_source{
	private:
	    int indent;
	    std::string address(int dis,const std::string&base,const std::string&ofs="",int scl=1);
	    std::string address(int dis,const std::string&base,int scl);
	    std::string address(const std::string&base,const std::string&ofs,int scl=1);
	    std::string address(const std::string&base,int scl=1);
	    std::string p(const std::string&str);
	    void generate_lval(const abstract_syntax_tree::node*node);
	public:
	    assembly_source();
	    void write(const std::string&str);
	    void write(const std::string&inst,const std::string&reg1,const std::string&reg2);
	    void write(const std::string&inst,int arg,const std::string&reg);
	    void write(const std::string&inst,const std::string&reg);
	    void write(const std::string&inst,int arg);
	    void generate(const abstract_syntax_tree::node*node);
	    void enter(const std::string&func);
	    void leave();
    };
}
