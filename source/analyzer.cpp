#include"semantics/analyzer.hpp"
using namespace semantics;
analyzer::analyzer()
{

}
void analyzer::declare(const std::string&name)const
{
    vars.insert(name);
}
bool analyzer::is_declared(const std::string&name)const
{
    return vars.count(name)==1;
}

