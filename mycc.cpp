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
	private:
	    struct token{
		int type;
		int value;
		char name;
		token(int type):type(type)                       {}
		token(int type,int value):type(type),value(value){}
		token(int type,char name):type(type),name(name)  {}
	    };
	    std::vector<token>tokens;
	public:
	    tokenizer(const std::string&s)
	    {
		for(int i=0;i<s.length();++i){
		    if(isspace(s[i])){
			continue;
		    }else if(s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/'||s[i]=='('||s[i]==')'||s[i]==';'){
			tokens.emplace_back(s[i]);
		    }else if(s[i]=='='){
			if(i!=s.length()-1&&s[i+1]=='='){
			    tokens.emplace_back(TK_EQ);
			    ++i;
			}else{
			    tokens.emplace_back('=');
			}
		    }else if(s[i]=='!'){
			if(i!=s.length()-1&&s[i+1]=='='){
			    tokens.emplace_back(TK_NE);
			    ++i;
			}else{
			    throw std::runtime_error("認識できないトークンが含まれます");
			}
		    }else if(s[i]=='<'){
			if(i!=s.length()-1&&s[i+1]=='='){
			    tokens.emplace_back(TK_LE);
			    ++i;
			}else{
			    tokens.emplace_back('<');
			}
		    }else if(s[i]=='>'){
			if(i!=s.length()-1&&s[i+1]=='='){
			    tokens.emplace_back(TK_GE);
			    ++i;
			}else{
			    tokens.emplace_back('>');
			}
		    }else if(isdigit(s[i])){
			size_t sz;
			tokens.emplace_back(TK_NUM,std::stoi(s.substr(i),&sz));
			i+=sz-1;
		    }else if(islower(s[i])){
			tokens.emplace_back(TK_IDENT,s[i]);
		    }else{
			throw std::runtime_error("無効な文字が含まれます");
		    }
		}
		tokens.emplace_back(TK_EOF);
	    }
	    const token&operator()(int idx)const
	    {
		return tokens[idx];
	    }
    };
    class abstract_syntax_tree{
	public:
	    struct node{
		int type;
		node*lhs,*rhs;
		int value;
		char name;
		node(node*left,int type,node*right):type(type),lhs(left),rhs(right){}
		node(int value):type(ND_NUM),value(value){}
		node(char name):type(ND_IDENT),name(name){}
	    };
	private:
	    const tokenizer&tk;
	    int pos_now;
	    std::vector<node*>stats;
	    bool consume(int type)
	    {
		if(tk(pos_now).type==type){
		    ++pos_now;
		    return true;
		}else{
		    return false;
		}
	    }
	    node*statement()
	    {
		node*ret=assign();
		if(!consume(';')){
		    throw std::runtime_error("不正な区切り文字です");
		}
		return ret;
	    }
	    node*assign()
	    {
		node*ret=equality();
		while(true){
		    if(consume('=')){
			ret=new node(ret,'=',assign());
		    }else{
			return ret;
		    }
		}
	    }
	    node*equality()
	    {
		node*ret=relational();
		while(true){
		    if(consume(TK_EQ)){
			ret=new node(ret,TK_EQ,relational());
		    }else if(consume(TK_NE)){
			ret=new node(ret,TK_NE,relational());
		    }else{
			return ret;
		    }
		}
	    }
	    node*relational()
	    {
		node*ret=add();
		while(true){
		    if(consume('<')){
			ret=new node(ret,'<',add());
		    }else if(consume('>')){
			ret=new node(ret,'>',add());
		    }else if(consume(TK_LE)){
			ret=new node(ret,TK_LE,add());
		    }else if(consume(TK_GE)){
			ret=new node(ret,TK_GE,add());
		    }else{
			return ret;
		    }
		}
	    }
	    node*add()
	    {
		node*ret=mul();
		while(true){
		    if(consume('+')){
			ret=new node(ret,'+',mul());
		    }else if(consume('-')){
			ret=new node(ret,'-',mul());
		    }else{
			return ret;
		    }
		}
	    }
	    node*mul()
	    {
		node*ret=unary();
		while(true){
		    if(consume('*')){
			ret=new node(ret,'*',unary());
		    }else if(consume('/')){
			ret=new node(ret,'/',unary());
		    }else{
			return ret;
		    }
		}
	    }
	    node*unary()
	    {
		if(consume('+')){
		    return term();
		}else if(consume('-')){
		    return new node(new node(0),'-',term());
		}else{
		    return term();
		}
	    }
	    node*term()
	    {
		if(consume('(')){
		    node*ret=equality();
		    if(!consume(')')){
			throw std::runtime_error("括弧の対応が正しくありません");
		    }
		    return ret;
		}else if(tk(pos_now).type==TK_NUM){
		    return new node(tk(pos_now++).value);
		}else if(tk(pos_now).type==TK_IDENT){
		    return new node(tk(pos_now++).name);
		}else{
		    throw std::runtime_error("構文解析ができませんでした");
		}
	    }
	public:
	    abstract_syntax_tree(tokenizer&_tk):tk(_tk),pos_now(0)
	    {
		while(tk(pos_now).type!=TK_EOF)stats.push_back(statement());
	    }
	    const std::vector<node*>&statements()
	    {
		return stats;
	    }
    };
    class assembly_source{
	private:
	    int indent;
	public:
	    assembly_source()
	    {
		indent=0;
		std::cout<<".global main"<<std::endl;
	    }
	    void write(const std::string&str)
	    {
		std::cout<<std::string(indent,' ')<<str<<std::endl;
	    }
	    void write(const std::string&inst,const std::string&reg1,const std::string&reg2){write(inst+' '+p(reg1)+", "+p(reg2));}
	    void write(const std::string&inst,int arg,const std::string&reg)                {write(inst+" $"+std::to_string(arg)+", "+p(reg));}
	    void write(const std::string&inst,const std::string&reg)                        {write(inst+" "+p(reg));}
	    void write(const std::string&inst,int arg)                                      {write(inst+" $"+std::to_string(arg));} 
	    std::string address(int dis,const std::string&base,const std::string&ofs="",int scl=1)
	    {
		std::stringstream ss;
		if(dis!=0)ss<<dis;
		ss<<'(';
		if(base!="")ss<<'%'<<base;
		if(ofs!="")ss<<",%"<<ofs;
		if(scl!=1)ss<<','<<scl;
		ss<<')';
		return ss.str();
	    }
	    std::string address(int dis,const std::string&base,int scl)                {return address(dis,base,"",scl);}
	    std::string address(const std::string&base,const std::string&ofs,int scl=1){return address(0,base,ofs,scl);}
	    std::string address(const std::string&base,int scl=1)                      {return address(0,base,"",scl);}
	    std::string p(const std::string&str)
	    {
		return (str[0]=='('?"":"%")+str;
	    }
	    void generate_lval(const abstract_syntax_tree::node*node)
	    {
		if(node->type!=ND_IDENT){
		    throw std::runtime_error("右辺値への代入はできません");
		}else{
		    int offset=('z'-node->name+1)*8;
		    write("mov","rbp","rax");
		    write("sub",offset,"rax");
		    write("push","rax");
		}
	    }
	    void generate(const abstract_syntax_tree::node*node)
	    {
		if(node->type==ND_NUM){
		    write("push",node->value);
		    return;
		}else if(node->type==ND_IDENT){
		    generate_lval(node);
		    write("pop","rax");
		    write("mov",address("rax"),"rax");
		    write("push","rax");
		    return;
		}else if(node->type=='='){
		    generate_lval(node->lhs);
		    generate(node->rhs);
		    write("pop","rdi");
		    write("pop","rax");
		    write("mov","rdi",address("rax"));
		    write("push","rdi");
		    return;
		}else{
		    generate(node->lhs);
		    generate(node->rhs);
		    write("pop","rdi");
		    write("pop","rax");
		    switch(node->type){
			case '+':
			    write("add","rdi","rax");
			    break;
			case '-':
			    write("sub","rdi","rax");
			    break;
			case '*':
			    write("mul","rdi");
			    break;
			case '/':
			    write("mov",0,"rdx");
			    write("div","rdi");
			    break;
			case TK_EQ:
			    write("cmp","rdi","rax");
			    write("sete","al");
			    write("movzb","al","rax");
			    break;
			case TK_NE:
			    write("cmp","rdi","rax");
			    write("setne","al");
			    write("movzb","al","rax");
			    break;
			case '<':
			    write("cmp","rdi","rax");
			    write("setl","al");
			    write("movzb","al","rax");
			    break;
			case '>':
			    write("cmp","rdi","rax");
			    write("setg","al");
			    write("movzb","al","rax");
			    break;
			case TK_LE:
			    write("cmp","rdi","rax");
			    write("setle","al");
			    write("movzb","al","rax");
			    break;
			case TK_GE:
			    write("cmp","rdi","rax");
			    write("setge","al");
			    write("movzb","al","rax");
			    break;
		    }
		    write("push","rax");
		}
	    }
	    void enter(const std::string&func)
	    {
		write(func+':');
		++indent;
		write("push","rbp");
		write("mov","rsp","rbp");
	    }
	    void leave()
	    {
		write("mov","rbp","rsp");
		write("pop","rbp");
		write("retq");
		--indent;
	    }
    };
}
int main(int argc,char**argv)
{
    try{
	if(argc!=2)throw std::runtime_error("引数の個数が正しくありません");
	mycc::tokenizer tk(argv[1]);
	mycc::assembly_source as;
	as.enter("main");
	as.write("sub",26*8,"rsp");
	mycc::abstract_syntax_tree ast(tk);
	for(auto s:ast.statements()){
	    as.generate(s);
	    as.write("pop","rax");
	}
	as.leave();
	return EXIT_SUCCESS;
    }
    catch(const std::exception&e){
	std::cout<<e.what()<<std::endl;
	return EXIT_FAILURE;
    }
}
