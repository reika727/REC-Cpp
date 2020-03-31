#pragma once
#include<string>
namespace syntax{
    class tree final{
        private:
            std::string assembly_source;
        public:
            tree(const std::string&src);
            std::string get_assembly()const noexcept;
    };
}
