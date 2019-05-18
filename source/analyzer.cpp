#include"semantics/analyzer.hpp"
#include<algorithm>
using namespace semantics;
analyzer::analyzer()
{

}
void analyzer::enter_func()
{
    vars.emplace();
}
void analyzer::leave_func()
{
    vars.pop();
}
void analyzer::enter_scope()
{
    vars.top().emplace_back();
}
void analyzer::leave_scope()
{
    vars.top().pop_back();
}
void analyzer::declare(const std::string&name)
{
    vars.top().back().insert(name);
}
bool analyzer::is_available(const std::string&name)
{
    return std::any_of(
	vars.top().begin(),
	vars.top().end(),
	[name](const std::set<std::string>&st){
	    return st.count(name)==1;
	}
    );
}
bool analyzer::is_declarable(const std::string&name)
{
    return vars.top().back().count(name)==0;
}
