#pragma once
#include<string>
#include<vector>
#include<map>
namespace code{
    class variable_manager{
	    std::vector<std::map<std::string,int>>offset;
	    int var_size;
	public:
	    variable_manager();
	    void enter_scope();
	    void leave_scope();
	    void set_offset(const std::string&name);
	    int get_offset(const std::string&name);
	    int get_var_size();
    };
}
