#include "utils.h"

#include <sstream>
#include <algorithm>

namespace utils
{
    std::vector<std::string> split(const std::string &str, char delim)
    {
        std::string token;
        std::stringstream stream;
        std::vector<std::string> tokens;
        size_t pos = 0;

        stream.str(str);
        while (std::getline(stream, token, delim)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    void to_lower(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), tolower);
    }

    bool is_digits(std::string str)
    {
        return std::all_of(str.begin(), str.end(), ::isdigit);
    }
}
