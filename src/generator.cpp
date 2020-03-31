#include"code/generator.hpp"
#include"exception/compilation_error.hpp"
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
int generator::set_offset(const syntax::identifier&id)
{
    return set_offset(id,-(offset.back().size()+1)*8);
}
int generator::set_offset(const syntax::identifier&id,int off)
{
    if(offset.back().count(id.name))
        throw exception::compilation_error("多重定義されました: "+id.name,id.line,id.col);
    return offset.back()[id.name]=off;
}
int generator::get_offset(const syntax::identifier&id)
{
    auto itr=std::find_if(
            offset.rbegin(),offset.rend(),
            [id](const std::map<std::string,int>&mp){
                return mp.count(id.name)==1;
            }
        );
    if(itr==offset.rend())
        throw exception::compilation_error("未定義の変数です: "+id.name,id.line,id.col);
    return(*itr)[id.name];
}
const std::string&generator::get_break_label(const syntax::_break_&br)const
{
    if(break_labels.empty())
        throw exception::compilation_error("不適切なbreak文です",br.line,br.col);
    return break_labels.top();
}
const std::string&generator::get_continue_label(const syntax::_continue_&con)const
{
    if(continue_labels.empty())
        throw exception::compilation_error("不適切なcontinue文です",con.line,con.col);
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
