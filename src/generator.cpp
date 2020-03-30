#include"code/generator.hpp"
#include<algorithm>
using namespace code;
void generator::enter_scope()
{
    offset.emplace_back();
}
int generator::leave_scope()
{
    int ret=offset.back().size()*8;
    offset.pop_back();
    return ret;
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
int generator::set_offset(const std::string&name)
{
    return set_offset(name,-(offset.back().size()+1)*8);
}
int generator::set_offset(const std::string&name,int off)
{
    return offset.back()[name]=off;
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
std::string generator::get_asm()const
{
    return dst.str();
}
std::string generator::to_address(int dis,const std::string&base,const std::string&ofs,int scl)
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
std::string generator::to_address(int dis,const std::string&base,int scl)
{
    return generator::to_address(dis,base,"",scl);
}
std::string generator::to_address(const std::string&base,const std::string&ofs,int scl)
{
    return generator::to_address(0,base,ofs,scl);
}
std::string generator::to_address(const std::string&base,int scl)
{
    return generator::to_address(0,base,"",scl);
}
generator::generator(){}
