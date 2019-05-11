#include"semantics/analyzer.hpp"
using namespace semantics;
analyzer::analyzer()
{

}
void analyzer::declare_var(const std::string&name)const
{
    vars.insert(name);
}
bool analyzer::declared(const std::string&name)const
{
    return vars.count(name)==1;
}

