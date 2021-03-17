#include "compilation_error.hpp"
using namespace exception;
compilation_error::compilation_error(const std::string &msg, int line, int col)
    : runtime_error(msg + " in line " + std::to_string(line) + ", col " + std::to_string(col))
{
}
