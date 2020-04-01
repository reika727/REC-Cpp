#include"code/generator.hpp"
#include<algorithm>
#include<stdexcept>
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
    if(offset.back().count(name))
        throw std::runtime_error("double definition");
    return offset.back()[name]=off;
}
int generator::get_offset(const std::string&name)
{
    auto itr=std::find_if(
            offset.rbegin(),offset.rend(),
            [name](const std::map<std::string,int>&mp){
                return mp.count(name)==1;
            }
        );
    if(itr==offset.rend())
        throw std::runtime_error("unknown variable");
    return(*itr)[name];
}
const std::string&generator::get_break_label()const
{
    if(break_labels.empty())
        throw std::runtime_error("can't break");
    return break_labels.top();
}
const std::string&generator::get_continue_label()const
{
    if(continue_labels.empty())
        throw std::runtime_error("can't continue");
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
