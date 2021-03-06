#ifndef UTILS
#define UTILS

#include <vector>
#include <string>

namespace utils
{
    std::vector<std::string> split(const std::string &str, char delim=' ');
    std::string join(const std::vector<std::string> &tokens, char delim = ' ');
    void to_lower(std::string str);
    bool is_digits(std::string str);
}

#endif
