#include"code/gcfuncs.hpp"
#include<sstream>
namespace code{
    std::string unique_label(const std::string&base)
    {
	static unsigned int serial=0;
	return base+std::to_string(serial++);
    } 
    std::string derefer(int dis,const std::string&base,const std::string&ofs,int scl)
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
    std::string derefer(int dis,const std::string&base,int scl)
    {
	return derefer(dis,base,"",scl);
    }
    std::string derefer(const std::string&base,const std::string&ofs,int scl)
    {
	return derefer(0,base,ofs,scl);
    }
    std::string derefer(const std::string&base,int scl)
    {
	return derefer(0,base,"",scl);
    }
}
