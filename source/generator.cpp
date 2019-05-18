#include"code/generator.hpp"
#include<algorithm>
using namespace code;
generator::generator(const std::string&filename):ofs(filename)
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
void generator::enter_func()
{
    offset.emplace();
    var_size.push(0);
}
void generator::leave_func()
{
    offset.pop();
    var_size.pop();
}
void generator::enter_scope()
{
    offset.top().emplace_back();
}
void generator::leave_scope()
{
    offset.top().pop_back();
}
void generator::set_offset(const std::string&name)
{
    offset.top().back()[name]=var_size.top()+=8;
}
int generator::get_offset(const std::string&name)
{
    return (*
	std::find_if(
	    offset.top().rbegin(),
	    offset.top().rend(),
	    [name](const std::map<std::string,int>&mp){
		return mp.count(name)==1;
	    }
	)
    )[name];
}
int generator::get_var_size()
{
    return var_size.top();
}
