#pragma once
#include<string>
#include<fstream>
#include<map>
namespace code{
    class generator{
	    mutable std::ofstream ofs;
	    mutable std::map<std::string,int>ofst;
	    mutable int var_size;
	public:
	    generator(const std::string&filename);
	    void write(const std::string&str)const;
	    void write(const std::string&inst,const std::string&reg1,const std::string&reg2)const;
	    void write(const std::string&inst,int arg,const std::string&reg)const;
	    void write(const std::string&inst,const std::string&reg)const;
	    void write(const std::string&inst,int arg)const;
	    int offset(const std::string&name)const;
	    void set_offset(const std::string&name)const;
    };
}
