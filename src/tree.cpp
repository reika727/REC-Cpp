#include"syntax/tree.hpp"
#include"semantics/analyzer.hpp"
#include"code/generator.hpp"
#include<stdexcept>
using namespace syntax;
using TK=lexicon::TK;
tree::tree(const std::string&src):ta(src)
{
    while(!ta.is_all_read())funcs.push_back(syntax::define_function::get(ta));
}
void tree::check()const
{
    semantics::analyzer analy;
    for(auto f:funcs)f->check(analy);
}
std::string tree::to_asm()const
{
    code::generator gen;
    for(auto f:funcs)f->to_asm(gen);
    return gen.get_asm();
}
