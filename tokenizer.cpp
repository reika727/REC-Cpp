#include"mycc.hpp"
using namespace mycc;
using token=tokenizer::token;
token::token(TK type):type(type){}
token::token(TK type,int value):type(type),value(value){}
token::token(TK type,const std::string&name):type(type),name(name){}
tokenizer::tokenizer(const std::string&s)
{
    for(int i=0;i<s.length();++i){
	if(isspace(s[i]))continue;
	else if(s[i]=='(')tokens.emplace_back(TK::OPARENT);
	else if(s[i]==')')tokens.emplace_back(TK::CPARENT);
	else if(s[i]==';')tokens.emplace_back(TK::SCOLON);
	else if(s[i]=='+'){
	    if(i!=s.length()-1&&s[i+1]=='+'){
		tokens.emplace_back(TK::PLPL);++i;
	    }else if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(TK::PLEQ);++i;
	    }else{
		tokens.emplace_back(TK::PLUS);
	    }
	}else if(s[i]=='-'){
	    if(i!=s.length()-1&&s[i+1]=='-'){
		tokens.emplace_back(TK::MIMI);++i;
	    }else if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(TK::MIEQ);++i;
	    }else{
		tokens.emplace_back(TK::MINUS);
	    }
	}else if(s[i]=='*'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(TK::ASEQ);
		++i;
	    }else{
		tokens.emplace_back(TK::ASTER);
	    }
	}else if(s[i]=='/'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(TK::SLEQ);
		++i;
	    }else{
		tokens.emplace_back(TK::SLASH);
	    }
	}else if(s[i]=='%'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(TK::PEEQ);
		++i;
	    }else{
		tokens.emplace_back(TK::PERCENT);
	    }
	}else if(s[i]=='='){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(TK::EQEQ);
		++i;
	    }else{
		tokens.emplace_back(TK::EQUAL);
	    }
	}else if(s[i]=='!'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(TK::EXEQ);
		++i;
	    }else{
		throw std::runtime_error("認識できないトークンが含まれます");
	    }
	}else if(s[i]=='<'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(TK::LEEQ);
		++i;
	    }else{
		tokens.emplace_back(TK::LESS);
	    }
	}else if(s[i]=='>'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(TK::GREQ);
		++i;
	    }else{
		tokens.emplace_back(TK::GREAT);
	    }
	}else if(s.substr(i,6)=="return"){
	    if(i+5!=s.length()-1&&isspace(s[i+6])){
		tokens.emplace_back(TK::RETURN);
		i+=5;
	    }
	}else if(isdigit(s[i])){
	    size_t sz;
	    tokens.emplace_back(TK::NUMERIC,std::stoi(s.substr(i),&sz));
	    i+=sz-1;
	}else if(isalpha(s[i])||s[i]=='_'){
	    auto beg=s.begin()+i;
	    auto len=find_if_not(beg,s.end(),[](char c){return isalpha(c)||isdigit(c)||c=='_';})-beg;
	    tokens.emplace_back(TK::IDENT,s.substr(i,len));
	    i+=len-1;
	}else{
	    throw std::runtime_error("認識できないトークンが含まれます");
	}
    }
    tokens.emplace_back(TK::ENDT);
}
const token&tokenizer::operator[](int idx)const
{
    return tokens[idx];
}
