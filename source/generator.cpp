#include"code/generator.hpp"
using namespace code;
generator::generator(const std::string&filename):ofs(filename),var_size(0)
{
    write(".global main");
}
void generator::write(const std::string&str)const
{
    ofs<<str<<std::endl;
}
void generator::write(const std::string&inst,const std::string&reg1,const std::string&reg2)const
{
    write(inst+' '+reg1+','+reg2);
}
void generator::write(const std::string&inst,int arg,const std::string&reg)const
{
    write(inst+" $"+std::to_string(arg)+','+reg);
}
void generator::write(const std::string&inst,const std::string&reg)const
{
    write(inst+' '+reg);
}
void generator::write(const std::string&inst,int arg)const
{
    write(inst+" $"+std::to_string(arg));
}
int generator::offset(const std::string&name)const
{
    return ofst[name];
}
void generator::set_offset(const std::string&name)const
{
    ofst[name]=var_size+=8;
}
