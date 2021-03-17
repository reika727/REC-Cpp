#pragma once
#include <sstream>
#include <string>
namespace code {
    class writer final {
    private:
        std::stringstream dst;

    public:
        void write(const std::string &str);
        void write(const std::string &inst, const std::string &reg1, const std::string &reg2);
        void write(const std::string &inst, int arg, const std::string &reg);
        void write(const std::string &inst, const std::string &reg);
        void write(const std::string &inst, int arg);
        std::string get_asm() const;
        static std::string get_unique_label(const std::string &base);
    };
}
