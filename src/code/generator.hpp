#pragma once
#include<map>
#include<sstream>
#include<stack>
#include<string>
#include<vector>
namespace code{
    class generator final{
        private:
            std::vector<std::map<std::string,int>>offset;
            std::stack<std::string>break_labels,continue_labels;
            std::stringstream dst;
        public:
            generator();
            void enter_scope();
            int leave_scope();
            void enter_break(const std::string&label);
            void leave_break();
            void enter_continue(const std::string&label);
            void leave_continue();
            int set_offset(const std::string&name);
            int set_offset(const std::string&name,int off);
            int get_offset(const std::string&name);
            const std::string&get_break_label()const;
            const std::string&get_continue_label()const;
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
