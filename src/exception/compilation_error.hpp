#pragma once
#include<stdexcept>
#include<string>
namespace exception{
    class compilation_error:public std::runtime_error{
        public:
            compilation_error(const std::string&kind,const std::string&msg,int line,int col);
            virtual ~compilation_error()=0;
    };
    class lexical_error final:public compilation_error{
        public:
            lexical_error(const std::string&msg,int line,int col);
    };
    class syntax_error final:public compilation_error{
        public:
            syntax_error(const std::string&msg,int line,int col);
    };
    class semantic_error final:public compilation_error{
        public:
            semantic_error(const std::string&msg,int line,int col);
    };
    class unsupported_error final:public compilation_error{
        public:
            unsupported_error(const std::string&msg,int line,int col);
    };
}
