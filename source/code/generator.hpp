#pragma once
#include<string>
#include<vector>
#include<map>
#include<stack>
#include<fstream>
namespace code{
    class generator{
	    std::ofstream ofs;
	    std::stack<std::vector<std::map<std::string,int>>>offset;
	    std::stack<int>var_size;
	public:
	    generator(const std::string&filename);
	    void write(const std::string&str);
	    void write(const std::string&inst,const std::string&reg1,const std::string&reg2);
	    void write(const std::string&inst,int arg,const std::string&reg);
	    void write(const std::string&inst,const std::string&reg);
	    void write(const std::string&inst,int arg);
	    void enter_func();
	    void leave_func();
	    void enter_scope();
	    void leave_scope();
	    void set_offset(const std::string&name);
	    int get_offset(const std::string&name);
	    int get_var_size();
    };
}
