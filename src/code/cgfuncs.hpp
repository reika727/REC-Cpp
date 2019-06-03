#pragma once
#include<string>
namespace code{
    std::string unique_label(const std::string&base,unsigned int serial);
    std::string address(int dis,const std::string&base,const std::string&ofs="",int scl=1);
    std::string address(int dis,const std::string&base,int scl);
    std::string address(const std::string&base,const std::string&ofs,int scl=1);
    std::string address(const std::string&base,int scl=1);
}
