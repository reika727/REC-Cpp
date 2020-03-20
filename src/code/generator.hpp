#pragma once
#include<string>
#include<vector>
#include<map>
#include<stack>
#include<fstream>
namespace code{
    class generator{
            std::vector<std::map<std::string,int>>offset;
            int var_size=0;
            std::stack<std::string>break_labels,continue_labels;
            std::ofstream dst;
        public:
            generator(const std::string&dst);
            void enter_scope();
            void leave_scope();
            void enter_break(const std::string&label);
            void leave_break();
            void enter_continue(const std::string&label);
            void leave_continue();
            void set_offset(const std::string&name);
            int get_offset(const std::string&name);
            int get_var_size();
            const std::string&get_break_label();
            const std::string&get_continue_label();
            void write(const std::string&str);
            void write(const std::string&inst,const std::string&reg1,const std::string&reg2);
            void write(const std::string&inst,int arg,const std::string&reg);
            void write(const std::string&inst,const std::string&reg);
            void write(const std::string&inst,int arg);
            static std::string unique_label(const std::string&base);
            static std::string address(int dis,const std::string&base,const std::string&ofs="",int scl=1);
            static std::string address(int dis,const std::string&base,int scl);
            static std::string address(const std::string&base,const std::string&ofs,int scl=1);
            static std::string address(const std::string&base,int scl=1);
    };
}
