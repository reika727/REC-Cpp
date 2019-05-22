#include"code/writer.hpp"
using namespace code;
void writer::operator()(const std::string&str)
{
    ofs<<str<<std::endl;
}
void writer::operator()(const std::string&inst,const std::string&reg1,const std::string&reg2)
{
    operator()(inst+' '+reg1+','+reg2);
}
void writer::operator()(const std::string&inst,int arg,const std::string&reg)
{
    operator()(inst+" $"+std::to_string(arg)+','+reg);
}
void writer::operator()(const std::string&inst,const std::string&reg)
{
    operator()(inst+' '+reg);
}
void writer::operator()(const std::string&inst,int arg)
{
    operator()(inst+" $"+std::to_string(arg));
}
writer::writer(const std::string&filename):ofs(filename)
{

}
