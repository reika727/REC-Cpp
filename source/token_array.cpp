#include"lexicon/token_array.hpp"
#include<stdexcept>
#include<algorithm>
using namespace lexicon;
token_array::token_array(const std::string&s)
{
    for(int i=0;i<s.length();++i){
	     if(s.substr(i,4)=="char" &&i+4<s.length()&&              isspace(s[i+4]) &&(i+=3))tv.push_back(new symbol(TK::CHAR));
	else if(s.substr(i,2)=="if"   &&i+2<s.length()&&(s[i+2]=='('||isspace(s[i+2]))&&(i+=1))tv.push_back(new symbol(TK::IF));
	else if(s.substr(i,4)=="else" &&i+4<s.length()&&(s[i+4]=='('||isspace(s[i+4]))&&(i+=3))tv.push_back(new symbol(TK::ELSE));
	else if(s.substr(i,5)=="while"&&i+5<s.length()&&(s[i+5]=='('||isspace(s[i+5]))&&(i+=4))tv.push_back(new symbol(TK::WHILE));
	else if(s.substr(i,3)=="for"  &&i+3<s.length()&&(s[i+3]=='('||isspace(s[i+3]))&&(i+=2))tv.push_back(new symbol(TK::FOR));
	else if(isdigit(s[i])){
	    size_t sz;
	    tv.push_back(new numeric(std::stoi(s.substr(i),&sz)));
	    i+=sz-1;
	}else if(isalpha(s[i])||s[i]=='_'){
	    auto beg=s.begin()+i;
	    auto len=find_if_not(beg,s.end(),[](char c){return isalpha(c)||isdigit(c)||c=='_';})-beg;
	    tv.push_back(new ident(s.substr(i,len)));
	    i+=len-1;
	}else if(s.substr(i,2)=="/*"){
	    while(true){
		if(++i>=s.length())throw std::runtime_error("コメントが閉じられていません");
		else if(s.substr(i,2)=="*/"&&++i)break;
	    }
	}
	else if(s.substr(i,2)=="++"&&++i)                                                      tv.push_back(new symbol(TK::PLPL));
	else if(s.substr(i,2)=="--"&&++i)                                                      tv.push_back(new symbol(TK::MIMI));
	else if(s.substr(i,2)=="+="&&++i)                                                      tv.push_back(new symbol(TK::PLEQ));
	else if(s.substr(i,2)=="-="&&++i)                                                      tv.push_back(new symbol(TK::MIEQ));
	else if(s.substr(i,2)=="*="&&++i)                                                      tv.push_back(new symbol(TK::ASEQ));
	else if(s.substr(i,2)=="/="&&++i)                                                      tv.push_back(new symbol(TK::SLEQ));
	else if(s.substr(i,2)=="%="&&++i)                                                      tv.push_back(new symbol(TK::PEEQ));
	else if(s.substr(i,2)=="=="&&++i)                                                      tv.push_back(new symbol(TK::EQEQ));
	else if(s.substr(i,2)=="!="&&++i)                                                      tv.push_back(new symbol(TK::EXEQ));
	else if(s.substr(i,2)=="<="&&++i)                                                      tv.push_back(new symbol(TK::LEEQ));
	else if(s.substr(i,2)==">="&&++i)                                                      tv.push_back(new symbol(TK::GREQ));
	else if(s[i]=='+')                                                                     tv.push_back(new symbol(TK::PLUS));
	else if(s[i]=='-')                                                                     tv.push_back(new symbol(TK::MINUS));
	else if(s[i]=='*')                                                                     tv.push_back(new symbol(TK::ASTER));
	else if(s[i]=='/')                                                                     tv.push_back(new symbol(TK::SLASH));
	else if(s[i]=='%')                                                                     tv.push_back(new symbol(TK::PERCENT));
	else if(s[i]=='=')                                                                     tv.push_back(new symbol(TK::EQUAL));
	else if(s[i]=='<')                                                                     tv.push_back(new symbol(TK::LESS));
	else if(s[i]=='>')                                                                     tv.push_back(new symbol(TK::GREATER));
	else if(s[i]=='(')                                                                     tv.push_back(new symbol(TK::OPARENT));
	else if(s[i]==')')                                                                     tv.push_back(new symbol(TK::CPARENT));
	else if(s[i]=='{')                                                                     tv.push_back(new symbol(TK::OBRACE));
	else if(s[i]=='}')                                                                     tv.push_back(new symbol(TK::CBRACE));
	else if(s[i]==';')                                                                     tv.push_back(new symbol(TK::SCOLON));
	else if(s[i]==',')                                                                     tv.push_back(new symbol(TK::COMMA));
	else if(!isspace(s[i])){
	    throw std::runtime_error("認識できないトークンが含まれます");
	}
    }
    itr=tv.begin();
}
token_array::~token_array()
{
    for(auto t:tv)delete t;
}
const std::vector<token*>::const_iterator&token_array::pos()
{
    return itr;
}
bool token_array::consume(TK type)
{
    if(itr!=tv.end()){
	if(auto symp=dynamic_cast<symbol*>(*itr)){
	    if(symp->type==type){
		++itr;
		return true;
	    }
	}
    }
    return false;
}
const int*token_array::consume_num()
{
    if(itr!=tv.end()){
	if(auto ptr=dynamic_cast<numeric*>(*itr)){
	    ++itr;
	    return &ptr->value;
	}
    }
    return nullptr;
}
const std::string*token_array::consume_id()
{
    if(itr!=tv.end()){
	if(auto ptr=dynamic_cast<ident*>(*itr)){
	    ++itr;
	    return &ptr->name;
	}
    }
    return nullptr;
}
bool token_array::is_all_read()
{
    return itr==tv.end();
}
