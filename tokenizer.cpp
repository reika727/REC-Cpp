#include"mycc.hpp"
using namespace mycc;
using token=tokenizer::token;
token::token(int type):type(type)
{

}
token::token(int type,int value):type(type),value(value)
{

}
token::token(int type,char name):type(type),name(name)
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
const token&tokenizer::operator()(int idx)const
{
    return tokens[idx];
}
