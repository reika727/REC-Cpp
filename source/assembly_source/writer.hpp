#pragma once
#include<fstream>
#include<string>
namespace assembly_source{
    class writer{
	    mutable std::ofstream ofs;
	public:
	    writer(const std::string&filename);
	    void write(const std::string&str)const;
	    void write(const std::string&inst,const std::string&reg1,const std::string&reg2)const;
	    void write(const std::string&inst,int arg,const std::string&reg)const;
	    void write(const std::string&inst,const std::string&reg)const;
	    void write(const std::string&inst,int arg)const;
	    static std::string derefer(int dis,const std::string&base,const std::string&ofs="",int scl=1);
	    static std::string derefer(int dis,const std::string&base,int scl);
	    static std::string derefer(const std::string&base,const std::string&ofs,int scl=1);
	    static std::string derefer(const std::string&base,int scl=1);
	    static std::string unique_label(const std::string&base);
    };
}
