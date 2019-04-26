#include<bits/stdc++.h>
enum{
    TK_NUM=0x100,
    TK_EOF,
};
class invalid_tokens:public std::exception{
    private:
	std::string s;
    public:
	invalid_tokens(const std::string&tk)
	{
	    s="トークナイズできません: '"+tk+'\'';
	}
	const char*what()const noexcept
	{
	    return s.c_str();
	}
};
class unexpected_token:public std::exception{
    private:
	std::string s;
    public:
	unexpected_token(const std::string&tk)
	{
	    s="予期しないトークンです: '"+tk+'\'';
	}
	const char*what()const noexcept
	{
	    return s.c_str();
	}
};
class tokens{
    private:
	static const int max_token=256;
	struct token{
	    int ty;
	    int val;
	}tarray[max_token];
    public:
	tokens(const std::string&s)
	{
	    int idx=0;
	    for(int i=0;i<s.length();++i){
		if(isspace(s[i])){
		    continue;
		}else if(s[i]=='+'||s[i]=='-'){
		    tarray[idx].ty=s[i];
		    ++idx;
		}else if(isdigit(s[i])){
		    tarray[idx].ty=TK_NUM;
		    size_t sz;
		    tarray[idx].val=std::stoi(s.substr(i),&sz);
		    ++idx;
		    i+=sz-1;
		}else{
		    throw invalid_tokens(s.substr(i));
		}
	    }
	    tarray[idx].ty=TK_EOF;
	}
	int type(int i)
	{
	    return tarray[i].ty;
	}
	int val(int i)
	{
	    return tarray[i].val;
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
	void write(const std::string&inst,const std::string&src,const std::string&dst)
	{
	    write(inst+' '+src+", "+dst);
	}
	void write(const std::string&inst,int num,const std::string&reg)
	{
	    write(inst,'$'+std::to_string(num),'%'+reg);
	}
};
int digit(int n)
{
    return n==0?1:static_cast<int>(floor(log10(n)+1));
}
int main(int argc,char**argv)
{
    try{
	if(argc!=2)throw std::runtime_error("引数の個数が正しくありません");
	tokens tk(argv[1]);
	if(tk.type(0)!=TK_NUM)throw unexpected_token(argv[1]);
	assembly_source as;
	as.write("main:");
	as.write("mov",tk.val(0),"eax");
	int stridx=digit(tk.val(0));
	for(int i=0;tk.type(i)!=TK_EOF;++i){
	    if(tk.type(i)=='+'||tk.type(i)=='-'){
		++stridx;
		std::string inst(tk.type(i)=='+'?"add":"sub");
		if(tk.type(++i)!=TK_NUM){
		    throw unexpected_token(argv[1]+stridx);
		}
		as.write(inst,tk.val(i),"eax");
		stridx+=digit(tk.val(i));
	    }
	}
	as.write("retq");
	return EXIT_SUCCESS;
    }
    catch(const std::exception&e){
	std::cout<<e.what()<<std::endl;
	return EXIT_FAILURE;
    }
}
