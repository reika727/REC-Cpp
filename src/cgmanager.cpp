#include"code/cgmanager.hpp"
#include<algorithm>
using namespace code;
void cgmanager::enter_scope()
{
    offset.emplace_back();
}
void cgmanager::leave_scope()
{
    int dec=offset.back().size()*8;
    var_size-=dec;
    write("add",dec,"%rsp");
    offset.pop_back();
}
void cgmanager::enter_break(const std::string&label)
{
    break_labels.push(label);
}
void cgmanager::leave_break()
{
    break_labels.pop();
}
void cgmanager::enter_continue(const std::string&label)
{
    continue_labels.push(label);
}
void cgmanager::leave_continue()
{
    continue_labels.pop();
}
void cgmanager::set_offset(const std::string&name)
{
    offset.back()[name]=var_size+=8;
}
int cgmanager::get_offset(const std::string&name)
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
int cgmanager::get_var_size()
{
    return var_size;
}
const std::string&cgmanager::get_break_label()
{
    return break_labels.top();
}
const std::string&cgmanager::get_continue_label()
{
    return continue_labels.top();
}
cgmanager::cgmanager(writer&write):write(write)
{

}
