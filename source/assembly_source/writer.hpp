#pragma once
#include"../parsing/node.hpp"
#include<fstream>
#include<map>
#include<string>
#include<vector>
namespace assembly_source{
    class writer{
	private:
	    std::ofstream ofs;
	    int indent;
	    int var_size;
	    std::map<std::string,int>offset;
	    unsigned int serial;
	    std::string label(const std::string&base);
	    std::string p(const std::string&str);
	    void write(const std::string&str);
	    void write(const std::string&inst,const std::string&reg1,const std::string&reg2);
	    void write(const std::string&inst,int arg,const std::string&reg);
	    void write(const std::string&inst,const std::string&reg);
	    void write(const std::string&inst,int arg);
	    std::string derefer(int dis,const std::string&base,const std::string&ofs="",int scl=1);
	    std::string derefer(int dis,const std::string&base,int scl);
	    std::string derefer(const std::string&base,const std::string&ofs,int scl=1);
	    std::string derefer(const std::string&base,int scl=1);
	    void enumerate_var(parsing::node*const node);
	    void refer_var(parsing::node*const node);
	    void RDP(parsing::node*const node);
	    void eval(parsing::statement*const st);
	    void eval(parsing::single*const sg);
	public:
	    writer(const std::string&filename);
	    void eval(const std::vector<parsing::statement*>&sv);
	    void enter(const std::string&func);
	    void leave();
    };
}
