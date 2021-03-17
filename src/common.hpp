#include <cstdio>
#include <string>

// g++でstd::formatが使えるようになるまではこれ使う
template <typename... Args>
std::string format(const std::string &fmt, Args... args)
{
    size_t len = std::snprintf(nullptr, 0, fmt.c_str(), args...);
    auto buf = new char[len + 1];
    std::snprintf(buf, len + 1, fmt.c_str(), args...);
    auto result = std::string(buf, buf + len);
    delete buf;
    return result;
}
