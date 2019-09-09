#pragma once
#include<string>
#include<fstream>
namespace code{
    class writer{
            std::ofstream ofs;
        public:
            writer(const std::string&str);
            void operator()(const std::string&str);
            void operator()(const std::string&inst,const std::string&reg1,const std::string&reg2);
            void operator()(const std::string&inst,int arg,const std::string&reg);
            void operator()(const std::string&inst,const std::string&reg);
            void operator()(const std::string&inst,int arg);
            writer&operator=(const writer&)=delete;
    };
}
