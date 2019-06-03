#include"code/variable_manager.hpp"
#include<algorithm>
using namespace code;
void variable_manager::enter_scope()
{
    offset.emplace_back();
}
void variable_manager::leave_scope()
{
    int dec=offset.back().size()*8;
    var_size-=dec;
    write("add",dec,"%rsp");
    offset.pop_back();
}
void variable_manager::enter_break(const std::string&label)
{
    break_labels.push(label);
}
void variable_manager::leave_break()
{
    break_labels.pop();
}
void variable_manager::enter_continue(const std::string&label)
{
    continue_labels.push(label);
}
void variable_manager::leave_continue()
{
    continue_labels.pop();
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
const std::string&variable_manager::get_break_label()
{
    return break_labels.top();
}
const std::string&variable_manager::get_continue_label()
{
    return continue_labels.top();
}
variable_manager::variable_manager(writer&write):write(write)
{

}
