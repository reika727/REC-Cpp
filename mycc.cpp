#include<iostream>
#include<string>
#include<exception>
#include<cmath>
namespace mycc{
    enum{
	TK_NUM=0x100,
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
		node*ret=term();
		while(true){
		    if(consume('*')){
			ret=new node(ret,'*',term());
		    }else if(consume('/')){
			ret=new node(ret,'/',term());
		    }else{
			return ret;
		    }
		}
	    }
	    node*term()
	    {
		if(consume('(')){
		    node*ret=add();
		    if(!consume(')'))throw std::runtime_error("括弧の対応が正しくありません");
		    return ret;
		}else if(tk(pos_now).type==TK_NUM){
		    return new node(tk(pos_now++).value);
		}else{
		    throw std::runtime_error("構文解析ができませんでした");
		}
	    }
	public:
	    abstract_syntax_tree(tokenizer&_tk):tk(_tk),pos_now(0),rt(add()){}
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
		}
		write("push","rax");
	    }
    };
}
int main(int argc,char**argv)
{
    auto digit=[](int n){return n==0?1:static_cast<int>(floor(log10(n)+1));};
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
