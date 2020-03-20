#pragma once
#include"node.hpp"
#include"../lexicon/token_array.hpp"
#include<string>
namespace syntax{
    class tree{
            lexicon::token_array ta;
            prog*root;
        private:
            const function*func();
            const statement*stat();
            const single*emptiable_single();
            const expression*order15();
            const expression*order14();
            const expression*order12();
            const expression*order11();
            const expression*order07();
            const expression*order06();
            const expression*order04();
            const expression*order03();
            const expression*order02();
            const expression*order01();
            const expression*order00();
        public:
            tree(const std::string&src);
            ~tree();
            const prog&get_root();
    };
}
