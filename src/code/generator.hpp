#pragma once
#include"../syntax/node.hpp"
#include<map>
#include<sstream>
#include<stack>
#include<string>
#include<vector>
namespace syntax{
    class identifier;
    class _break_;
    class _continue_;
}
namespace code{
    class generator final{
        private:
            std::vector<std::map<std::string,int>>offset;
            std::stack<std::string>break_labels,continue_labels;
            std::stringstream dst;
        public:
            void enter_scope();
            int leave_scope();
            void enter_break(const std::string&label);
            void leave_break();
            void enter_continue(const std::string&label);
            void leave_continue();
            int set_offset(const syntax::identifier&id);
            int set_offset(const syntax::identifier&id,int off);
            int get_offset(const syntax::identifier&id);
            const std::string&get_break_label(const syntax::_break_&br)const;
            const std::string&get_continue_label(const syntax::_continue_&con)const;
            void write(const std::string&str);
            void write(const std::string&inst,const std::string&reg1,const std::string&reg2);
            void write(const std::string&inst,int arg,const std::string&reg);
            void write(const std::string&inst,const std::string&reg);
            void write(const std::string&inst,int arg);
            std::string get_asm()const;
            static std::string get_unique_label(const std::string&base);
            generator&operator=(const generator&)=delete;
            generator&operator=(generator&&)=delete;
    };
}
