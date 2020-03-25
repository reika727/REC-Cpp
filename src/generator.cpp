#include"code/generator.hpp"
#include<algorithm>
#include<sstream>
using namespace code;
void generator::enter_scope()
{
    offset.emplace_back();
}
void generator::leave_scope()
{
    int dec=offset.back().size()*8;
    var_size-=dec;
    write("add",dec,"%rsp");
    offset.pop_back();
}
void generator::enter_break(const std::string&label)
{
    break_labels.push(label);
}
void generator::leave_break()
{
    break_labels.pop();
}
void generator::enter_continue(const std::string&label)
{
    continue_labels.push(label);
}
void generator::leave_continue()
{
    continue_labels.pop();
}
void generator::set_offset(const std::string&name)
{
    offset.back()[name]=var_size+=8;
}
int generator::get_offset(const std::string&name)
{
    return (*
        std::find_if(
            offset.rbegin(),offset.rend(),
            [name](const std::map<std::string,int>&mp){
                return mp.count(name)==1;
            }
        )
    )[name];
}
int generator::get_var_size()const noexcept
{
    return var_size;
}
const std::string&generator::get_break_label()const
{
    return break_labels.top();
}
const std::string&generator::get_continue_label()const
{
    return continue_labels.top();
}
void generator::write(const std::string&str)
{
    dst<<str<<std::endl;
}
void generator::write(const std::string&inst,const std::string&reg1,const std::string&reg2)
{
    write(inst+' '+reg1+','+reg2);
}
void generator::write(const std::string&inst,int arg,const std::string&reg)
{
    write(inst+" $"+std::to_string(arg)+','+reg);
}
void generator::write(const std::string&inst,const std::string&reg)
{
    write(inst+' '+reg);
}
void generator::write(const std::string&inst,int arg)
{
    write(inst+" $"+std::to_string(arg));
}
std::string generator::get_unique_label(const std::string&base)
{
    static unsigned int serial=0;
    return base+std::to_string(serial++);
}
std::string generator::get_address(int dis,const std::string&base,const std::string&ofs,int scl)
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
std::string generator::get_address(int dis,const std::string&base,int scl)
{
    return generator::get_address(dis,base,"",scl);
}
std::string generator::get_address(const std::string&base,const std::string&ofs,int scl)
{
    return generator::get_address(0,base,ofs,scl);
}
std::string generator::get_address(const std::string&base,int scl)
{
    return generator::get_address(0,base,"",scl);
}
generator::generator(const std::string&dst)
    :dst(dst){}
