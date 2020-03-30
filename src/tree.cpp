#include"syntax/tree.hpp"
#include"code/generator.hpp"
using namespace syntax;
using TK=lexicon::TK;
tree::tree(const std::string&src):ta(src)
{
    while(!ta.is_all_read())funcs.push_back(syntax::define_function::get(ta));
}
std::string tree::to_asm()const
{
    code::generator gen;
    for(auto f:funcs)f->to_asm(gen);
    return gen.get_asm();
}
