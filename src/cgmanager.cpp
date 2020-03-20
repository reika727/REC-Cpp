#include"code/cgmanager.hpp"
#include<algorithm>
#include<sstream>
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
std::string cgmanager::unique_label(const std::string&base)
{
    static unsigned int serial=0;
    return base+std::to_string(serial++);
}
std::string cgmanager::address(int dis,const std::string&base,const std::string&ofs,int scl)
{
    std::stringstream ss;
    if(dis!=0)ss<<dis;
    ss<<'(';
    if(base!="")ss<<base;
    if(ofs!="")ss<<','<<ofs;
    if(scl!=1)ss<<','<<scl;
    ss<<')';
    return ss.str();
}
std::string cgmanager::address(int dis,const std::string&base,int scl)
{
    return cgmanager::address(dis,base,"",scl);
}
std::string cgmanager::address(const std::string&base,const std::string&ofs,int scl)
{
    return cgmanager::address(0,base,ofs,scl);
}
std::string cgmanager::address(const std::string&base,int scl)
{
    return cgmanager::address(0,base,"",scl);
}
cgmanager::cgmanager(const std::string&dst):write(dst)
{

}
