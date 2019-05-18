#pragma once
#include<string>
#include<fstream>
namespace code{
    class writer{
	    std::ofstream ofs;
	public:
	    writer(const std::string&str);
	    void write(const std::string&str);
	    void write(const std::string&inst,const std::string&reg1,const std::string&reg2);
	    void write(const std::string&inst,int arg,const std::string&reg);
	    void write(const std::string&inst,const std::string&reg);
	    void write(const std::string&inst,int arg);
    };
}
