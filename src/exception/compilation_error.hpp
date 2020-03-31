#pragma once
#include<stdexcept>
#include<string>
namespace exception{
    class compilation_error:public std::runtime_error{
        public:
            compilation_error(const std::string&msg,int line,int col);
    };
}
