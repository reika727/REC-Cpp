#include<iostream>
#include<string>
#include<exception>
#include<cmath>
namespace mycc{
    enum{
	TK_NUM=0x100,
	TK_EQ,
	TK_NE,
	TK_LE,
	TK_GE,
	TK_EOF,
    };
    class tokenizer{
	private:
	    static const int max_token=256;
	    struct token{
		int type;
		int value;
	    }tokens[max_token];
	public:
	    tokenizer(const std::string&s)
	    {
		int idx=0;
		for(int i=0;i<s.length();++i){
		    if(isspace(s[i])){
			continue;
		    }else if(s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/'||s[i]=='('||s[i]==')'){
			tokens[idx++].type=s[i];
		    }else if(s[i]=='='){
			if(i!=s.length()-1&&s[i+1]=='='){
			    tokens[idx++].type=TK_EQ;
			    ++i;
			}else{
			    throw std::runtime_error("認識できないトークンが含まれます");
			}
		    }else if(s[i]=='!'){
			if(i!=s.length()-1&&s[i+1]=='='){
			    tokens[idx++].type=TK_NE;
			    ++i;
			}else{
			    throw std::runtime_error("認識できないトークンが含まれます");
			}
		    }else if(s[i]=='<'){
			if(i!=s.length()-1&&s[i+1]=='='){
			    tokens[idx++].type=TK_LE;
			    ++i;
			}else{
			    tokens[idx++].type='<';
			}
		    }else if(s[i]=='>'){
			if(i!=s.length()-1&&s[i+1]=='='){
			    tokens[idx++].type=TK_GE;
			    ++i;
			}else{
			    tokens[idx++].type='>';
			}
		    }else if(isdigit(s[i])){
			size_t sz;
			tokens[idx].type=TK_NUM;
			tokens[idx++].value=std::stoi(s.substr(i),&sz);
			i+=sz-1;
		    }else{
			throw std::runtime_error("無効な文字が含まれます");
		    }
		}
		tokens[idx].type=TK_EOF;
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
		node(node*left,int type,node*right):type(type),lhs(left),rhs(right){}
		node(int value):type(TK_NUM),value(value){}
	    };
	private:
	    const tokenizer&tk;
	    int pos_now;
	    node*rt;
	    bool consume(int type)
	    {
		if(tk(pos_now).type==type){
		    ++pos_now;
		    return true;
		}else{
		    return false;
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
		}
		return term();
	    }
	    node*term()
	    {
		if(consume('(')){
		    node*ret=add();
		    if(!consume(')')){
			throw std::runtime_error("括弧の対応が正しくありません");
		    }
		    return ret;
		}else if(tk(pos_now).type==TK_NUM){
		    return new node(tk(pos_now++).value);
		}else{
		    throw std::runtime_error("構文解析ができませんでした");
		}
	    }
	public:
	    abstract_syntax_tree(tokenizer&_tk):tk(_tk),pos_now(0),rt(equality()){}
	    const node*const root()
	    {
		return rt;
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
		if(str.back()==':')++indent;
	    }
	    void write(const std::string&inst,const std::string&reg1,const std::string&reg2)
	    {
		write(inst+" %"+reg1+", %"+reg2);
	    }
	    void write(const std::string&inst,int arg,const std::string&reg)
	    {
		write(inst+" $"+std::to_string(arg)+", %"+reg);
	    }
	    void write(const std::string&inst,const std::string&reg)
	    {
		write(inst+" %"+reg);
	    }
	    void write(const std::string&inst,int arg)
	    {
		write(inst+" $"+std::to_string(arg));
	    }
	    void generate(const abstract_syntax_tree::node*node)
	    {
		if(node->type==TK_NUM){
		    write("push",node->value);
		    return;
		}
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
    };
}
int main(int argc,char**argv)
{
    try{
	if(argc!=2)throw std::runtime_error("引数の個数が正しくありません");
	mycc::tokenizer tk(argv[1]);
	mycc::abstract_syntax_tree ast(tk);
	mycc::assembly_source as;
	as.write("main:");
	as.generate(ast.root());
	as.write("pop","rax");
	as.write("retq");
	return EXIT_SUCCESS;
    }
    catch(const std::exception&e){
	std::cout<<e.what()<<std::endl;
	return EXIT_FAILURE;
    }
}
