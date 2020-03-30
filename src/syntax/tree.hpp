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
        public:
            tree(const std::string&src);
            std::string to_asm()const;
    };
}
