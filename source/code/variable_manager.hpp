#pragma once
#include"writer.hpp"
#include<string>
#include<vector>
#include<map>
namespace code{
    class variable_manager{
	    std::vector<std::map<std::string,int>>offset;
	    int var_size;
	public:
	    writer&write;
	    variable_manager(writer&write);
	    void enter_scope();
	    void leave_scope();
	    void set_offset(const std::string&name);
	    int get_offset(const std::string&name);
	    int get_var_size();
    };
}
