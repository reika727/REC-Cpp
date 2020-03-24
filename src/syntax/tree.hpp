#pragma once
#include"node.hpp"
#include"../lexicon/token_array.hpp"
#include<memory>
#include<string>
#include<vector>
namespace syntax{
    class tree final{
        private:
            lexicon::token_array ta;
            std::vector<std::shared_ptr<const function>>funcs;
            std::shared_ptr<const function>func();
            std::shared_ptr<const statement>stat();
            std::shared_ptr<const single_statement>get_single();
            std::shared_ptr<const expression>order15();
            std::shared_ptr<const expression>order14();
            std::shared_ptr<const expression>order12();
            std::shared_ptr<const expression>order11();
            std::shared_ptr<const expression>order07();
            std::shared_ptr<const expression>order06();
            std::shared_ptr<const expression>order04();
            std::shared_ptr<const expression>order03();
            std::shared_ptr<const expression>order02();
            std::shared_ptr<const expression>order01();
            std::shared_ptr<const expression>order00();
        public:
            tree(const std::string&src);
            void check()const;
            void to_asm(const std::string&dst)const;
    };
}
