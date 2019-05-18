#include"code/variable_manager.hpp"
#include<algorithm>
using namespace code;
variable_manager::variable_manager():var_size(0)
{

}
void variable_manager::enter_scope()
{
    offset.emplace_back();
}
void variable_manager::leave_scope()
{
    offset.pop_back();
}
void variable_manager::set_offset(const std::string&name)
{
    offset.back()[name]=var_size+=8;
}
int variable_manager::get_offset(const std::string&name)
{
    return (*
	std::find_if(
	    offset.rbegin(),
	    offset.rend(),
	    [name](const std::map<std::string,int>&mp){
		return mp.count(name)==1;
	    }
	)
    )[name];
}
int variable_manager::get_var_size()
{
    return var_size;
}
