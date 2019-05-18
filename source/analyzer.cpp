#include"semantics/analyzer.hpp"
#include<algorithm>
using namespace semantics;
analyzer::analyzer()
{

}
void analyzer::enter_scope()
{
    vars.emplace_back();
}
void analyzer::leave_scope()
{
    vars.pop_back();
}
void analyzer::define(const std::string&name)
{
    vars.back().insert(name);
}
bool analyzer::is_available(const std::string&name)
{
    return std::any_of(
	vars.begin(),
	vars.end(),
	[name](const std::set<std::string>&st){
	    return st.count(name)==1;
	}
    );
}
bool analyzer::is_definable(const std::string&name)
{
    return vars.back().count(name)==0;
}
