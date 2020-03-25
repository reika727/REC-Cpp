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
            std::vector<std::shared_ptr<const define_function>>funcs;
            std::shared_ptr<const define_function>get_function();
            std::shared_ptr<const statement>get_statement();
            std::shared_ptr<const single_statement>get_single();
            std::shared_ptr<const expression>get_order15();
            std::shared_ptr<const expression>get_order14();
            std::shared_ptr<const expression>get_order12();
            std::shared_ptr<const expression>get_order11();
            std::shared_ptr<const expression>get_order07();
            std::shared_ptr<const expression>get_order06();
            std::shared_ptr<const expression>get_order04();
            std::shared_ptr<const expression>get_order03();
            std::shared_ptr<const expression>get_order02();
            std::shared_ptr<const expression>get_order01();
            std::shared_ptr<const expression>get_order00();
        public:
            tree(const std::string&src);
            void check()const;
            void to_asm(const std::string&dst)const;
    };
}
