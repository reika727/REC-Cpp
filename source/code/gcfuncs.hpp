#pragma once
#include<string>
namespace code{
    std::string unique_label(const std::string&base);
    std::string derefer(int dis,const std::string&base,const std::string&ofs="",int scl=1);
    std::string derefer(int dis,const std::string&base,int scl);
    std::string derefer(const std::string&base,const std::string&ofs,int scl=1);
    std::string derefer(const std::string&base,int scl=1);
}
