#include"semantics/analyzer.hpp"
#include<algorithm>
using namespace semantics;
void analyzer::enter_scope()
{
    vars.emplace_back();
}
void analyzer::leave_scope()
{
    vars.pop_back();
}
void analyzer::define_func(const std::string&name,int var_num)
{
    funcs.insert(name);
    signature[name]=var_num;
}
void analyzer::define_var(const std::string&name)
{
    vars.back().insert(name);
}
bool analyzer::is_available_func(const std::string&name,int var_num)
{
    return funcs.count(name)==1&&signature[name]==var_num;
}
bool analyzer::is_available_var(const std::string&name)
{
    return std::any_of(
	vars.begin(),
	vars.end(),
	[name](const std::set<std::string>&st){
	    return st.count(name)==1;
	}
    );
}
bool analyzer::is_definable_func(const std::string&name)
{
    return funcs.count(name)==0;
}
bool analyzer::is_definable_var(const std::string&name)
{
    return vars.back().count(name)==0;
}
