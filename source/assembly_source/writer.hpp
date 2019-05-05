#pragma once
#include<fstream>
#include<string>
namespace assembly_source{
    class writer{
	    std::ofstream ofs;
	private:
	    static std::string p(const std::string&str);
	public:
	    writer(const std::string&filename);
	    std::string unique_label(const std::string&base);
	    void write(const std::string&str);
	    void write(const std::string&inst,const std::string&reg1,const std::string&reg2);
	    void write(const std::string&inst,int arg,const std::string&reg);
	    void write(const std::string&inst,const std::string&reg);
	    void write(const std::string&inst,int arg);
	    static std::string derefer(int dis,const std::string&base,const std::string&ofs="",int scl=1);
	    static std::string derefer(int dis,const std::string&base,int scl);
	    static std::string derefer(const std::string&base,const std::string&ofs,int scl=1);
	    static std::string derefer(const std::string&base,int scl=1);
    };
}
