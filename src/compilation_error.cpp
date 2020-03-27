#include"exception/compilation_error.hpp"
using namespace exception;
compilation_error::compilation_error(const std::string&kind,const std::string&msg,int line,int col)
    :runtime_error(kind+": "+msg+" in line "+std::to_string(line)+", col "+std::to_string(col)){}
lexical_error::lexical_error(const std::string&msg,int line,int col)
    :compilation_error("字句解析フェイズでのエラー",msg,line,col){}
syntax_error::syntax_error(const std::string&msg,int line,int col)
    :compilation_error("構文解析フェイズでのエラー",msg,line,col){}
semantic_error::semantic_error(const std::string&msg,int line,int col)
    :compilation_error("意味解析フェイズでのエラー",msg,line,col){}
unsupported_error::unsupported_error(const std::string&msg,int line,int col)
    :compilation_error("未対応の機能",msg,line,col){}
compilation_error::~compilation_error(){}
