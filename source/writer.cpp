#include"assembly_source/writer.hpp"
#include<sstream>
using namespace assembly_source;
writer::writer(const std::string&filename):ofs(filename)
{
    write(".global main");
}
std::string writer::unique_label(const std::string&base)
{
    static unsigned int serial=0;
    return '.'+base+std::to_string(serial++);
}
void writer::write(const std::string&str)const
{
    ofs<<str<<std::endl;
}
void writer::write(const std::string&inst,const std::string&reg1,const std::string&reg2)const
{
    write(inst+' '+p(reg1)+", "+p(reg2));
}
void writer::write(const std::string&inst,int arg,const std::string&reg)const
{
    write(inst+" $"+std::to_string(arg)+", "+p(reg));
}
void writer::write(const std::string&inst,const std::string&reg)const
{
    write(inst+' '+p(reg));
}
void writer::write(const std::string&inst,int arg)const
{
    write(inst+" $"+std::to_string(arg));
} 
std::string writer::derefer(int dis,const std::string&base,const std::string&ofs,int scl)
{
    std::stringstream ss;
    if(dis!=0)ss<<dis;
    ss<<'(';
    if(base!="")ss<<'%'<<base;
    if(ofs!="")ss<<",%"<<ofs;
    if(scl!=1)ss<<','<<scl;
    ss<<')';
    return ss.str();
}
std::string writer::derefer(int dis,const std::string&base,int scl)
{
    return derefer(dis,base,"",scl);
}
std::string writer::derefer(const std::string&base,const std::string&ofs,int scl)
{
    return derefer(0,base,ofs,scl);
}
std::string writer::derefer(const std::string&base,int scl)
{
    return derefer(0,base,"",scl);
}
std::string writer::p(const std::string&str)
{
    return (str[0]=='('||str[0]=='.'?"":"%")+str;
}
