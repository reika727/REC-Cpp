#include"code/generator.hpp"
using namespace code;
generator::generator(const std::string&filename):ofs(filename),var_size(0)
{
    write(".global main");
}
void generator::write(const std::string&str)
{
    ofs<<str<<std::endl;
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
int generator::get_offset(const std::string&name)
{
    return offset[name];
}
void generator::set_offset(const std::string&name)
{
    offset[name]=var_size+=8;
}
int generator::get_var_size()
{
    return var_size;
}
