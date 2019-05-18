#include"code/writer.hpp"
using namespace code;
writer::writer(const std::string&filename):ofs(filename)
{
    write(".global main");
}
void writer::write(const std::string&str)
{
    ofs<<str<<std::endl;
}
void writer::write(const std::string&inst,const std::string&reg1,const std::string&reg2)
{
    write(inst+' '+reg1+','+reg2);
}
void writer::write(const std::string&inst,int arg,const std::string&reg)
{
    write(inst+" $"+std::to_string(arg)+','+reg);
}
void writer::write(const std::string&inst,const std::string&reg)
{
    write(inst+' '+reg);
}
void writer::write(const std::string&inst,int arg)
{
    write(inst+" $"+std::to_string(arg));
}
