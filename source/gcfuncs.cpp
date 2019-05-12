#include"code/gcfuncs.hpp"
#include<sstream>
namespace code{
    std::string unique_label(const std::string&base)
    {
	static unsigned int serial=0;
	return base+std::to_string(serial++);
    } 
    std::string address(int dis,const std::string&base,const std::string&ofs,int scl)
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
    std::string address(int dis,const std::string&base,int scl)
    {
	return address(dis,base,"",scl);
    }
    std::string address(const std::string&base,const std::string&ofs,int scl)
    {
	return address(0,base,ofs,scl);
    }
    std::string address(const std::string&base,int scl)
    {
	return address(0,base,"",scl);
    }
}
