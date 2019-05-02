#include"mycc.hpp"
using mycc::tokenizer;
using token  =tokenizer::token;
using numeric=tokenizer::numeric;
using ident  =tokenizer::ident;
using symbol =tokenizer::symbol;
token  ::~token()                                    {}
numeric::numeric(int value)             :value(value){}
ident  ::ident  (const std::string&name):name(name)  {}
symbol ::symbol (TK type)               :type(type)  {}
tokenizer::tokenizer(const std::string&s)
{
    for(int i=0;i<s.length();++i){
	if(isspace(s[i]))continue;
	else if(s[i]=='(')tokens.emplace_back(new symbol(TK::OPARENT));
	else if(s[i]==')')tokens.emplace_back(new symbol(TK::CPARENT));
	else if(s[i]=='{')tokens.emplace_back(new symbol(TK::OBRACE));
	else if(s[i]=='}')tokens.emplace_back(new symbol(TK::CBRACE));
	else if(s[i]==';')tokens.emplace_back(new symbol(TK::SCOLON));
	else if(s[i]=='+'){
	    if(i!=s.length()-1&&s[i+1]=='+'){
		tokens.emplace_back(new symbol(TK::PLPL));++i;
	    }else if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(new symbol(TK::PLEQ));++i;
	    }else{
		tokens.emplace_back(new symbol(TK::PLUS));
	    }
	}else if(s[i]=='-'){
	    if(i!=s.length()-1&&s[i+1]=='-'){
		tokens.emplace_back(new symbol(TK::MIMI));++i;
	    }else if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(new symbol(TK::MIEQ));++i;
	    }else{
		tokens.emplace_back(new symbol(TK::MINUS));
	    }
	}else if(s[i]=='*'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(new symbol(TK::ASEQ));
		++i;
	    }else{
		tokens.emplace_back(new symbol(TK::ASTER));
	    }
	}else if(s[i]=='/'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(new symbol(TK::SLEQ));
		++i;
	    }else{
		tokens.emplace_back(new symbol(TK::SLASH));
	    }
	}else if(s[i]=='%'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(new symbol(TK::PEEQ));
		++i;
	    }else{
		tokens.emplace_back(new symbol(TK::PERCENT));
	    }
	}else if(s[i]=='='){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(new symbol(TK::EQEQ));
		++i;
	    }else{
		tokens.emplace_back(new symbol(TK::EQUAL));
	    }
	}else if(s[i]=='!'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(new symbol(TK::EXEQ));
		++i;
	    }else{
		throw std::runtime_error("認識できないトークンが含まれます");
	    }
	}else if(s[i]=='<'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(new symbol(TK::LEEQ));
		++i;
	    }else{
		tokens.emplace_back(new symbol(TK::LESS));
	    }
	}else if(s[i]=='>'){
	    if(i!=s.length()-1&&s[i+1]=='='){
		tokens.emplace_back(new symbol(TK::GREQ));
		++i;
	    }else{
		tokens.emplace_back(new symbol(TK::GREAT));
	    }
	}else if(s.substr(i,2)=="if"&&i+2<s.length()&&(s[i+2]=='('||isspace(s[i+2]))){
	    tokens.emplace_back(new symbol(TK::IF));
	    ++i;
	}else if(isdigit(s[i])){
	    size_t sz;
	    tokens.emplace_back(new numeric(std::stoi(s.substr(i),&sz)));
	    i+=sz-1;
	}else if(isalpha(s[i])||s[i]=='_'){
	    auto beg=s.begin()+i;
	    auto len=find_if_not(beg,s.end(),[](char c){return isalpha(c)||isdigit(c)||c=='_';})-beg;
	    tokens.emplace_back(new ident(s.substr(i,len)));
	    i+=len-1;
	}else{
	    throw std::runtime_error("認識できないトークンが含まれます");
	}
    }
}
tokenizer::token*const tokenizer::operator[](int idx)const
{
    return tokens[idx];
}
size_t tokenizer::size()const
{
    return tokens.size();
}
