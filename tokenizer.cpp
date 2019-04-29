#include"mycc.hpp"
using namespace mycc;
using token=tokenizer::token;
token::token(int type):type(type)
{

}
token::token(int type,int value):type(type),value(value)
{

}
token::token(int type,const std::string&name):type(type),name(name)
{

}
tokenizer::tokenizer(const std::string&s)
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
	}else if(s.substr(i,6)=="return"){
	    if(i+5!=s.length()-1&&isspace(s[i+6])){
		tokens.emplace_back(TK_RETURN);
		i+=5;
	    }
	}else if(isdigit(s[i])){
	    size_t sz;
	    tokens.emplace_back(TK_NUM,std::stoi(s.substr(i),&sz));
	    i+=sz-1;
	}else if(islower(s[i])){
	    auto beg=s.begin()+i;
	    auto len=find_if_not(beg,s.end(),[](char c){return isalpha(c);})-beg;
	    tokens.emplace_back(TK_IDENT,s.substr(i,len));
	    i+=len-1;
	}else{
	    throw std::runtime_error("無効な文字が含まれます");
	}
    }
    tokens.emplace_back(TK_EOF);
}
const token&tokenizer::operator()(int idx)const
{
    return tokens[idx];
}
