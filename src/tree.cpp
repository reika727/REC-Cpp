#include"syntax/tree.hpp"
#include"syntax/node.hpp"
#include"lexicon/token_array.hpp"
#include"code/generator.hpp"
using namespace syntax;
tree::tree(const std::string&src)
{
    lexicon::token_array ta(src);
    code::generator gen;
    while(!ta.is_all_read())define_function::get(ta)->to_asm(gen);
    assembly_source=gen.get_asm();
}
std::string tree::get_assembly()const noexcept
{
    return assembly_source;
}
