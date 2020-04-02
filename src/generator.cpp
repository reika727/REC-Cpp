#include"code/generator.hpp"
using namespace code;
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
std::string generator::get_asm()const
{
    return dst.str();
}
std::string generator::get_unique_label(const std::string&base)
{
    static unsigned int serial=0;
    return base+std::to_string(serial++);
}
