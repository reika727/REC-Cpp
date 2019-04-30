#include"mycc.hpp"
using namespace mycc;
using token=tokenizer::token;
token::token(int type):type(type){}
token::token(int type,int value):type(type),value(value){}
token::token(int type,const std::string&name):type(type),name(name){}
tokenizer::tokenizer(const std::string&s)
{
    for(int i=0;i<s.length();++i){
	if(isspace(s[i]))continue;
	else if(s[i]=='+')tokens.emplace_back(PLUS);
	else if(s[i]=='-')tokens.emplace_back(MINUS);
	else if(s[i]=='*')tokens.emplace_back(MULTI);
	else if(s[i]=='/')tokens.emplace_back(DIVIDE);
	else if(s[i]=='(')tokens.emplace_back(OPARENT);
	else if(s[i]==')')tokens.emplace_back(CPARENT);
	else if(s[i]==';')tokens.emplace_back(SEPARATE);
	else if(s[i]=='='){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(EQUAL);
		++i;
	    }else{
		tokens.emplace_back(ASSIGN);
	    }
	}else if(s[i]=='!'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(NEQUAL);
		++i;
	    }else{
		throw std::runtime_error("認識できないトークンが含まれます");
	    }
	}else if(s[i]=='<'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(LEEQ);
		++i;
	    }else{
		tokens.emplace_back(LESS);
	    }
	}else if(s[i]=='>'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(GREQ);
		++i;
	    }else{
		tokens.emplace_back(GREAT);
	    }
	}else if(s.substr(i,6)=="return"){
	    if(i+5!=s.length()-1&&isspace(s[i+6])){
		tokens.emplace_back(RETURN);
		i+=5;
	    }
	}else if(isdigit(s[i])){
	    size_t sz;
	    tokens.emplace_back(NUMERIC,std::stoi(s.substr(i),&sz));
	    i+=sz-1;
	}else if(isalpha(s[i])||s[i]=='_'){
	    auto beg=s.begin()+i;
	    auto len=find_if_not(beg,s.end(),[](char c){return isalpha(c)||isdigit(c)||c=='_';})-beg;
	    tokens.emplace_back(IDENT,s.substr(i,len));
	    i+=len-1;
	}else{
	    throw std::runtime_error("無効な文字が含まれます");
	}
    }
    tokens.emplace_back(ENDT);
}
const token&tokenizer::operator()(int idx)const
{
    return tokens[idx];
}
